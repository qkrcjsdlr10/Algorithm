"""Import the existing backup scripts as modules (they import Playwright at
module level, so loading is deferred until an adapter is actually used)."""

from __future__ import annotations

import importlib
import sys
from pathlib import Path
from types import ModuleType

from algo.adapters.base import Unavailable

SCRIPTS_DIR = Path(__file__).resolve().parents[3] / "scripts"


def load_script(name: str) -> ModuleType:
    if str(SCRIPTS_DIR) not in sys.path:
        sys.path.insert(0, str(SCRIPTS_DIR))
    try:
        return importlib.import_module(name)
    except ModuleNotFoundError as error:
        if error.name == "playwright" or (error.name or "").startswith("playwright"):
            raise Unavailable("playwright 패키지가 없습니다: .venv에 tools/requirements.txt를 설치하세요") from error
        raise
