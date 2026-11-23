"""Dedicated Edge profile with a localhost-only DevTools port.

* never the user's normal Edge profile
* profile lives outside the repository (%LOCALAPPDATA%\\algo-sync\\edge-profile)
* the port is checked to be bound to 127.0.0.1 only
* login is always done by the user in that window; cookies are never exported
"""

from __future__ import annotations

import json
import os
import subprocess
import time
from pathlib import Path
from urllib.parse import urlparse

from algo.config import Config

EDGE_CANDIDATES = (
    r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe",
    r"C:\Program Files\Microsoft\Edge\Application\msedge.exe",
)
START_PAGES = (
    "https://school.programmers.co.kr/",
    "https://swexpertacademy.com/main/main.do",
    "https://leetcode.com/",
)


def _port(cdp_url: str) -> int:
    return urlparse(cdp_url).port or 9222


def cdp_reachable(cdp_url: str, timeout: float = 2.0) -> bool:
    try:
        import httpx

        return httpx.get(f"{cdp_url.rstrip('/')}/json/version", timeout=timeout).status_code == 200
    except Exception:  # noqa: BLE001
        return False


def exposed_bindings(port: int) -> list[str]:
    """Listening addresses for the port other than loopback (Windows netstat)."""
    try:
        output = subprocess.run(["netstat", "-ano", "-p", "TCP"], capture_output=True, text=True,
                                errors="replace").stdout
    except OSError:
        return []
    exposed = []
    for line in output.splitlines():
        parts = line.split()
        if len(parts) >= 4 and parts[0].upper() == "TCP" and "LISTEN" in parts[3].upper():
            local = parts[1]
            if local.endswith(f":{port}") and not local.startswith(("127.0.0.1:", "[::1]:")):
                exposed.append(local)
    return exposed


def _default_profile_roots() -> list[Path]:
    base = os.environ.get("LOCALAPPDATA")
    return [Path(base) / "Microsoft" / "Edge" / "User Data"] if base else []


def edge_path(config: Config) -> Path:
    if config.edge_path:
        return Path(config.edge_path)
    for candidate in EDGE_CANDIDATES:
        if Path(candidate).is_file():
            return Path(candidate)
    raise SystemExit("msedge.exe를 찾지 못했습니다. algo.toml의 browser.edge_path를 설정하세요.")


def start(config: Config) -> str:
    profile = config.profile_dir.resolve()
    for default in _default_profile_roots():
        if profile == default.resolve() or default.resolve() in profile.parents:
            raise SystemExit("일반 Edge 프로필은 사용할 수 없습니다. 전용 profile_dir을 지정하세요.")
    if config.root.resolve() in (profile, *profile.parents):
        raise SystemExit("브라우저 프로필을 저장소 안에 둘 수 없습니다.")
    port = _port(config.cdp_url)
    if cdp_reachable(config.cdp_url):
        return f"이미 실행 중입니다 (포트 {port})"
    profile.mkdir(parents=True, exist_ok=True)
    process = subprocess.Popen([
        str(edge_path(config)),
        f"--remote-debugging-port={port}",
        f"--user-data-dir={profile}",
        "--no-first-run",
        "--no-default-browser-check",
        *START_PAGES,
    ])
    (config.state_dir).mkdir(parents=True, exist_ok=True)
    (config.state_dir / "browser.json").write_text(json.dumps({"pid": process.pid, "port": port}), encoding="utf-8")
    for _ in range(30):
        if cdp_reachable(config.cdp_url):
            break
        time.sleep(0.5)
    exposed = exposed_bindings(port)
    if exposed:
        stop(config)
        raise SystemExit(f"디버그 포트가 외부 주소에 열려 있어 브라우저를 닫았습니다: {exposed}")
    return (f"Edge 전용 프로필을 열었습니다 (127.0.0.1:{port}, 프로필 {profile}).\n"
            "열린 창에서 프로그래머스·SWEA·LeetCode에 직접 로그인하세요. 비밀번호는 저장하지 않습니다.")


def stop(config: Config) -> str:
    info_path = config.state_dir / "browser.json"
    if not info_path.is_file():
        return "algo가 연 브라우저 기록이 없습니다"
    info = json.loads(info_path.read_text(encoding="utf-8"))
    pid = int(info.get("pid") or 0)
    if pid:
        # Graceful close (no /F) so Edge saves the profile normally.
        subprocess.run(["taskkill", "/PID", str(pid), "/T"], capture_output=True)
    info_path.unlink(missing_ok=True)
    return "Edge 전용 브라우저 종료를 요청했습니다"


def check(config: Config) -> str:
    port = _port(config.cdp_url)
    reachable = cdp_reachable(config.cdp_url)
    exposed = exposed_bindings(port) if reachable else []
    if not reachable:
        return f"디버그 브라우저 없음 (포트 {port})"
    if exposed:
        return f"경고: 포트 {port}가 외부 주소에 열려 있습니다 {exposed}"
    return f"실행 중, 127.0.0.1:{port} 전용"
