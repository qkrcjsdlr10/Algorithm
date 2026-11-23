"""Background loop: scan on file changes (watchdog, if installed) or every
``scan_interval_seconds``, poll due sites, publish per policy. Single instance,
graceful stop via Ctrl+C or ``algo stop``."""

from __future__ import annotations

import ctypes
import json
import logging
import os
import threading
import time
from logging.handlers import RotatingFileHandler
from pathlib import Path

from algo import db as dbm
from algo import engine, layout
from algo.config import Config
from algo.repo import Repo

log = logging.getLogger("algo")


def setup_logging(config: Config, verbose: bool = False) -> None:
    config.log_dir.mkdir(parents=True, exist_ok=True)
    handler = RotatingFileHandler(config.log_dir / "algo.log", maxBytes=1_000_000, backupCount=5, encoding="utf-8")
    handler.setFormatter(logging.Formatter("%(asctime)s %(levelname)s %(message)s"))
    root = logging.getLogger("algo")
    root.setLevel(logging.DEBUG if verbose else logging.INFO)
    if not any(isinstance(h, RotatingFileHandler) for h in root.handlers):
        root.addHandler(handler)


def pid_alive(pid: int) -> bool:
    if pid <= 0:
        return False
    if os.name == "nt":
        # os.kill(pid, 0) would terminate the process on Windows; query instead.
        handle = ctypes.windll.kernel32.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
        if not handle:
            return False
        code = ctypes.c_ulong()
        ok = ctypes.windll.kernel32.GetExitCodeProcess(handle, ctypes.byref(code))
        ctypes.windll.kernel32.CloseHandle(handle)
        return bool(ok) and code.value == 259  # STILL_ACTIVE
    try:
        os.kill(pid, 0)
    except OSError:
        return False
    return True


class InstanceLock:
    """Lock file with PID; a stale lock from a killed process is recovered."""

    def __init__(self, path: Path) -> None:
        self.path = path

    def acquire(self) -> None:
        self.path.parent.mkdir(parents=True, exist_ok=True)
        if self.path.exists():
            try:
                pid = int(json.loads(self.path.read_text(encoding="utf-8")).get("pid", 0))
            except (ValueError, OSError):
                pid = 0
            if pid_alive(pid) and pid != os.getpid():
                raise SystemExit(f"algo daemon이 이미 실행 중입니다 (PID {pid})")
            log.warning("남아 있던 잠금 파일을 정리합니다 (PID %s)", pid)
            self.path.unlink(missing_ok=True)
        self.path.write_text(json.dumps({"pid": os.getpid(), "started": dbm.now()}), encoding="utf-8")

    def release(self) -> None:
        try:
            if json.loads(self.path.read_text(encoding="utf-8")).get("pid") == os.getpid():
                self.path.unlink(missing_ok=True)
        except (OSError, ValueError):
            pass


def running_pid(config: Config) -> int | None:
    path = config.state_dir / "daemon.lock"
    try:
        pid = int(json.loads(path.read_text(encoding="utf-8")).get("pid", 0))
    except (OSError, ValueError):
        return None
    return pid if pid_alive(pid) else None


def _start_watcher(config: Config, wake: threading.Event):
    try:
        from watchdog.events import FileSystemEventHandler
        from watchdog.observers import Observer
    except ModuleNotFoundError:
        log.info("watchdog 없음: %s초마다 git status로 확인합니다", config.scan_interval)
        return None

    class Handler(FileSystemEventHandler):
        def on_any_event(self, event):  # noqa: ANN001
            if Path(str(event.src_path)).suffix.casefold() in layout.SOURCE_EXTENSIONS:
                wake.set()

    observer = Observer()
    for site in layout.watched_roots():
        folder = config.root / site
        if folder.is_dir():
            observer.schedule(Handler(), str(folder), recursive=True)
    for folder in config.extra_watch_dirs:
        if folder.is_dir():
            observer.schedule(Handler(), str(folder), recursive=True)
    observer.daemon = True
    observer.start()
    return observer


def run(config: Config, once: bool = False) -> int:
    setup_logging(config)
    lock = InstanceLock(config.state_dir / "daemon.lock")
    lock.acquire()
    stop_file = config.state_dir / "stop"
    stop_file.unlink(missing_ok=True)
    wake = threading.Event()
    observer = _start_watcher(config, wake)
    database = dbm.Database(config.db_path)
    repo = Repo(config.root, config.remote)
    log.info("algo daemon 시작 (PID %s, publish=%s)", os.getpid(), config.publish_mode)
    try:
        if database.get("baseline_at") is None:
            from algo import migrate

            report = migrate.baseline(config, database, repo)
            log.info("기준점 등록: 추적 파일 %s개", report.tracked_files)
        while True:
            try:
                report = engine.cycle(config, database, repo)
                if report.scanned.get("changed") or report.verified or report.fetched or report.publish:
                    log.info("cycle scanned=%s verified=%s fetched=%s errors=%s publish=%s", report.scanned,
                             report.verified, report.fetched, report.site_errors, report.publish)
                for site, error in report.site_errors.items():
                    log.debug("%s: %s", site, error)
            except Exception:  # noqa: BLE001 - keep the daemon alive; next cycle retries
                log.exception("cycle 실패, 다음 주기에 다시 시도합니다")
            if once:
                return 0
            wake.wait(timeout=config.scan_interval)
            if wake.is_set():
                time.sleep(2)  # let the editor finish writing
                wake.clear()
            if stop_file.exists():
                log.info("stop 요청을 받아 종료합니다")
                return 0
    except KeyboardInterrupt:
        log.info("Ctrl+C로 종료합니다")
        return 0
    finally:
        if observer is not None:
            observer.stop()
        database.close()
        lock.release()
        stop_file.unlink(missing_ok=True)


def request_stop(config: Config) -> str:
    pid = running_pid(config)
    if pid is None:
        return "실행 중인 daemon이 없습니다"
    config.state_dir.mkdir(parents=True, exist_ok=True)
    (config.state_dir / "stop").write_text("stop", encoding="utf-8")
    return f"daemon(PID {pid})에 종료를 요청했습니다. 현재 작업을 마친 뒤 종료합니다"
