"""Fixtures: a throw-away working repository with a bare 'origin', containing
copies of the real update_stats.py, pre-commit hook and README.md."""

from __future__ import annotations

import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterator

import pytest

TOOLS = Path(__file__).resolve().parents[1]
REAL_ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))

from algo import config as cfg  # noqa: E402
from algo import db as dbm  # noqa: E402
from algo.adapters.base import SiteAdapter  # noqa: E402
from algo.ingest import FetchedSubmission  # noqa: E402
from algo.repo import Repo  # noqa: E402


def git(cwd: Path, *args: str, check: bool = True) -> str:
    result = subprocess.run(["git", "-c", "core.quotepath=false", *args], cwd=cwd, capture_output=True,
                            text=True, encoding="utf-8", errors="replace")
    if check and result.returncode != 0:
        raise AssertionError(f"git {' '.join(args)}: {result.stderr}")
    return result.stdout.strip()


def _identity(path: Path) -> None:
    git(path, "config", "user.name", "Test User")
    git(path, "config", "user.email", "test@example.invalid")
    git(path, "config", "core.autocrlf", "false")
    git(path, "config", "commit.gpgsign", "false")


@dataclass
class Env:
    root: Path
    remote: Path
    tmp: Path

    def write(self, relative: str, content: str | bytes) -> Path:
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(content.encode("utf-8") if isinstance(content, str) else content)
        return path

    def config(self, **publish) -> cfg.Config:
        overrides = {"general": {"expected_remote": ""}, "publish": {"mode": "immediate", **publish}}
        return cfg.load(self.root, overrides)

    def db(self) -> dbm.Database:
        return dbm.Database(self.root / ".algo" / "state.sqlite3")

    def repo(self) -> Repo:
        return Repo(self.root, "origin")

    def git(self, *args: str, check: bool = True) -> str:
        return git(self.root, *args, check=check)

    def other_clone(self, name: str = "other") -> Path:
        path = self.tmp / name
        if not path.exists():
            git(self.tmp, "clone", "-q", str(self.remote), str(path))
            _identity(path)
        else:
            git(path, "pull", "-q", "--rebase", "origin", "main")
        return path

    def remote_log(self) -> list[str]:
        return git(self.remote, "log", "--format=%s", "main").splitlines()

    def remote_files(self) -> set[str]:
        return set(git(self.remote, "ls-tree", "-r", "--name-only", "main").splitlines())

    def remote_show(self, path: str) -> str:
        return git(self.remote, "show", f"main:{path}")


@pytest.fixture()
def env(tmp_path: Path) -> Iterator[Env]:
    remote = tmp_path / "remote.git"
    git(tmp_path, "init", "-q", "--bare", "-b", "main", str(remote))
    root = tmp_path / "work"
    root.mkdir()
    git(root, "init", "-q", "-b", "main")
    _identity(root)
    (root / "scripts").mkdir()
    shutil.copy2(REAL_ROOT / "scripts" / "update_stats.py", root / "scripts" / "update_stats.py")
    shutil.copytree(REAL_ROOT / ".githooks", root / ".githooks")
    shutil.copy2(REAL_ROOT / "README.md", root / "README.md")
    (root / ".gitignore").write_text(".algo/\n", encoding="utf-8")
    for relative, text in {
        "BOJ/Silver/1000_A+B_CPP.cpp": "int main(){}\n",
        "Programmers/Lv1/12928_약수의_합_Java.java": "class Solution {}\n",
        "SWEA/D3/1206_View_CPP.cpp": "int main(){return 0;}\n",
        "LeetCode/Medium/3_Longest_Substring_Java.java": "class Solution {}\n",
    }.items():
        path = root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8", newline="\n")
    subprocess.run([sys.executable, "scripts/update_stats.py"], cwd=root, check=True, capture_output=True)
    git(root, "add", "-A")
    git(root, "commit", "-q", "-m", "initial")
    git(root, "remote", "add", "origin", str(remote))
    git(root, "push", "-q", "origin", "main")
    git(root, "branch", "--set-upstream-to=origin/main", "main")
    git(root, "config", "core.hooksPath", ".githooks")
    yield Env(root=root, remote=remote, tmp=tmp_path)


class FakeAdapter(SiteAdapter):
    """Yields prepared submissions or raises a prepared error."""

    def __init__(self, site: str, config_name: str, items=(), error: BaseException | None = None) -> None:
        self.site = site
        self.config_name = config_name
        self.items = list(items)
        self.error = error
        self.calls = 0

    def fetch(self, context):
        self.calls += 1
        if self.error is not None:
            raise self.error
        for item in self.items:
            if item.submission_key not in context.known_submission_keys:
                yield item


def factory(*adapters: SiteAdapter):
    def make(stack, sites):
        names = set(sites)
        return [a for a in adapters if a.config_name in names]

    return make


def submission(site="Programmers", key="1", problem="12940", title="최대공약수와 최소공배수", tier="Lv1",
               language="Java", extension=".java", code="class Solution {}\n", **extra) -> FetchedSubmission:
    return FetchedSubmission(site=site, submission_key=key, problem_id=problem, title=title, tier=tier,
                             language=language, extension=extension, code=code,
                             submitted_at="2026-11-23T10:00:00+09:00", extra=extra)
