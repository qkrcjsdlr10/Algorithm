"""Settings from ``algo.toml`` (committed, no secrets) merged with optional
``.algo/algo.local.toml`` (ignored by Git) for per-PC overrides."""

from __future__ import annotations

import copy
import os
import tomllib
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

PUBLISH_MODES = {"immediate", "batch", "commit_only", "manual"}
DUPLICATE_POLICIES = {"keep", "add_as_new", "ask", "ignore"}
SITE_MODES = {"local", "cloud", "manual", "off"}

DEFAULTS: dict[str, Any] = {
    "general": {
        "branch": "main",
        "remote": "origin",
        "expected_remote": "github.com/qkrcjsdlr10/Algorithm",
        "scan_interval_seconds": 15,
        "extra_watch_dirs": [],
    },
    "publish": {"mode": "batch", "batch_minutes": 3, "auto_fast_forward": True},
    "duplicates": {"policy": "ask"},
    "browser": {"cdp_url": "http://127.0.0.1:9222", "edge_path": "", "profile_dir": ""},
    "sites": {
        "leetcode": {"mode": "cloud", "interval_minutes": 10, "max_problems_per_run": 20, "delay_seconds": 1.0},
        "programmers": {"mode": "local", "interval_minutes": 20, "max_problems_per_run": 10, "delay_seconds": 1.0},
        "swea": {"mode": "local", "interval_minutes": 60, "max_problems_per_run": 5, "delay_seconds": 1.5},
        "boj": {"mode": "manual", "interval_minutes": 10, "max_problems_per_run": 0, "delay_seconds": 2.0},
    },
}


def _merge(base: dict[str, Any], override: dict[str, Any]) -> dict[str, Any]:
    result = dict(base)
    for key, value in override.items():
        if isinstance(value, dict) and isinstance(result.get(key), dict):
            result[key] = _merge(result[key], value)
        else:
            result[key] = value
    return result


@dataclass
class SiteSettings:
    name: str
    mode: str
    interval_minutes: float
    max_problems_per_run: int
    delay_seconds: float


@dataclass
class Config:
    root: Path
    data: dict[str, Any] = field(default_factory=dict)

    @property
    def state_dir(self) -> Path:
        return self.root / ".algo"

    @property
    def db_path(self) -> Path:
        return self.state_dir / "state.sqlite3"

    @property
    def log_dir(self) -> Path:
        return self.state_dir / "logs"

    @property
    def branch(self) -> str:
        return str(self.data["general"]["branch"])

    @property
    def remote(self) -> str:
        return str(self.data["general"]["remote"])

    @property
    def expected_remote(self) -> str:
        return str(self.data["general"]["expected_remote"])

    @property
    def scan_interval(self) -> float:
        return float(self.data["general"]["scan_interval_seconds"])

    @property
    def extra_watch_dirs(self) -> list[Path]:
        return [Path(os.path.expandvars(str(p))).expanduser() for p in self.data["general"]["extra_watch_dirs"]]

    @property
    def publish_mode(self) -> str:
        return str(self.data["publish"]["mode"])

    @property
    def batch_minutes(self) -> float:
        return float(self.data["publish"]["batch_minutes"])

    @property
    def auto_fast_forward(self) -> bool:
        return bool(self.data["publish"]["auto_fast_forward"])

    @property
    def duplicate_policy(self) -> str:
        return str(self.data["duplicates"]["policy"])

    @property
    def cdp_url(self) -> str:
        return str(self.data["browser"]["cdp_url"])

    @property
    def edge_path(self) -> str:
        return str(self.data["browser"]["edge_path"])

    @property
    def profile_dir(self) -> Path:
        value = str(self.data["browser"]["profile_dir"])
        if value:
            return Path(os.path.expandvars(value)).expanduser()
        base = os.environ.get("LOCALAPPDATA") or str(Path.home() / "AppData" / "Local")
        return Path(base) / "algo-sync" / "edge-profile"

    def site(self, name: str) -> SiteSettings:
        raw = self.data["sites"][name]
        return SiteSettings(
            name=name,
            mode=str(raw["mode"]),
            interval_minutes=float(raw["interval_minutes"]),
            max_problems_per_run=int(raw["max_problems_per_run"]),
            delay_seconds=max(0.5, float(raw["delay_seconds"])),
        )

    def validate(self) -> None:
        if self.publish_mode not in PUBLISH_MODES:
            raise ValueError(f"publish.mode must be one of {sorted(PUBLISH_MODES)}")
        if self.duplicate_policy not in DUPLICATE_POLICIES:
            raise ValueError(f"duplicates.policy must be one of {sorted(DUPLICATE_POLICIES)}")
        for name in self.data["sites"]:
            if self.site(name).mode not in SITE_MODES:
                raise ValueError(f"sites.{name}.mode must be one of {sorted(SITE_MODES)}")
        if not self.cdp_url.startswith(("http://127.0.0.1:", "http://localhost:")):
            raise ValueError("browser.cdp_url must point to 127.0.0.1 or localhost")


def load(root: Path, overrides: dict[str, Any] | None = None) -> Config:
    data = copy.deepcopy(DEFAULTS)
    for path in (root / "algo.toml", root / ".algo" / "algo.local.toml"):
        if path.is_file():
            data = _merge(data, tomllib.loads(path.read_text(encoding="utf-8")))
    if overrides:
        data = _merge(data, overrides)
    config = Config(root=root, data=data)
    config.validate()
    return config


def find_root(start: Path | None = None) -> Path:
    current = (start or Path.cwd()).resolve()
    for candidate in (current, *current.parents):
        if (candidate / ".git").exists() and (candidate / "scripts" / "update_stats.py").is_file():
            return candidate
    here = Path(__file__).resolve().parents[2]
    if (here / "scripts" / "update_stats.py").is_file():
        return here
    raise SystemExit("Algorithm 저장소를 찾을 수 없습니다. 저장소 안에서 실행하세요.")
