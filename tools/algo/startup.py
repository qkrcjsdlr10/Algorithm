"""Windows Task Scheduler registration for starting the daemon at logon."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

from algo.config import Config

TASK_NAME = "ALGO Auto Sync"


def _pythonw() -> str:
    candidate = Path(sys.executable).with_name("pythonw.exe")
    return str(candidate if candidate.is_file() else Path(sys.executable))


def command(config: Config) -> list[str]:
    cli = config.root / "tools" / "algo_cli.py"
    action = f'"{_pythonw()}" "{cli}" daemon'
    return ["schtasks", "/Create", "/TN", TASK_NAME, "/SC", "ONLOGON", "/RL", "LIMITED", "/TR", action, "/F"]


def install(config: Config) -> str:
    result = subprocess.run(command(config), capture_output=True, text=True, errors="replace")
    if result.returncode != 0:
        return f"등록 실패: {result.stderr.strip() or result.stdout.strip()}"
    return f"로그인 시 자동 시작하도록 작업 스케줄러에 '{TASK_NAME}'을 등록했습니다"


def uninstall() -> str:
    result = subprocess.run(["schtasks", "/Delete", "/TN", TASK_NAME, "/F"], capture_output=True, text=True,
                            errors="replace")
    if result.returncode != 0:
        return f"삭제 실패: {result.stderr.strip() or result.stdout.strip()}"
    return f"작업 스케줄러에서 '{TASK_NAME}'을 삭제했습니다"


def status() -> str:
    result = subprocess.run(["schtasks", "/Query", "/TN", TASK_NAME], capture_output=True, text=True, errors="replace")
    return "등록됨" if result.returncode == 0 else "등록 안 됨"
