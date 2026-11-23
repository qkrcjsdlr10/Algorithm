"""Thin Git CLI wrapper. Every Git call goes through ``Repo.git`` so the
forbidden operations below can never be issued by the agent."""

from __future__ import annotations

import os
import subprocess
from dataclasses import dataclass
from pathlib import Path


class GitError(RuntimeError):
    def __init__(self, args: list[str], code: int, stdout: str, stderr: str) -> None:
        super().__init__(f"git {' '.join(args)} failed ({code}): {stderr.strip() or stdout.strip()}")
        self.code = code
        self.stdout = stdout
        self.stderr = stderr


class ForbiddenGitOperation(RuntimeError):
    pass


def _check_allowed(args: list[str], remote: str) -> None:
    if not args:
        return
    command = args[0]
    flags = set(args[1:])
    if command == "push":
        if flags & {"--force", "-f", "--force-with-lease", "--mirror", "--delete", "-d", "--prune"}:
            raise ForbiddenGitOperation("force/delete push is forbidden")
        if any(a.startswith(("--force", "+")) for a in args[1:]):
            raise ForbiddenGitOperation("force push is forbidden")
        positional = [a for a in args[1:] if not a.startswith("-")]
        if not positional or positional[0] != remote:
            raise ForbiddenGitOperation(f"push is only allowed to '{remote}'")
    if command == "reset" and ("--hard" in flags or "--merge" in flags or "--keep" in flags):
        raise ForbiddenGitOperation("git reset --hard/--merge/--keep is forbidden")
    if command in {"clean", "filter-branch", "filter-repo", "gc"}:
        raise ForbiddenGitOperation(f"git {command} is forbidden")
    if command in {"checkout", "restore", "switch"}:
        raise ForbiddenGitOperation(f"git {command} can discard work and is forbidden")
    if command == "rm" and "--cached" not in flags:
        raise ForbiddenGitOperation("git rm without --cached is forbidden")


@dataclass
class Repo:
    root: Path
    remote: str = "origin"

    def git(
        self,
        *args: str,
        index_file: Path | None = None,
        check: bool = True,
        input_bytes: bytes | None = None,
        env: dict[str, str] | None = None,
    ) -> subprocess.CompletedProcess[str]:
        argv = list(args)
        _check_allowed(argv, self.remote)
        run_env = dict(os.environ)
        run_env.pop("GIT_INDEX_FILE", None)
        run_env["GIT_TERMINAL_PROMPT"] = "0"
        run_env.setdefault("LC_ALL", "C.UTF-8")
        if index_file is not None:
            run_env["GIT_INDEX_FILE"] = str(index_file)
        if env:
            run_env.update(env)
        result = subprocess.run(
            ["git", "-c", "core.quotepath=false", *argv],
            cwd=self.root,
            env=run_env,
            input=input_bytes.decode("utf-8") if input_bytes is not None else None,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        )
        if check and result.returncode != 0:
            raise GitError(argv, result.returncode, result.stdout, result.stderr)
        return result

    def out(self, *args: str, index_file: Path | None = None) -> str:
        return self.git(*args, index_file=index_file).stdout.strip()

    # -- state queries -----------------------------------------------------
    @property
    def git_dir(self) -> Path:
        value = Path(self.out("rev-parse", "--git-dir"))
        return value if value.is_absolute() else self.root / value

    def current_branch(self) -> str:
        return self.git("symbolic-ref", "--quiet", "--short", "HEAD", check=False).stdout.strip()

    def head(self) -> str:
        return self.out("rev-parse", "HEAD")

    def has_head(self) -> bool:
        return self.git("rev-parse", "--verify", "--quiet", "HEAD", check=False).returncode == 0

    def remote_url(self) -> str:
        return self.git("remote", "get-url", self.remote, check=False).stdout.strip()

    def operation_in_progress(self) -> str | None:
        git_dir = self.git_dir
        for marker, label in (
            ("MERGE_HEAD", "merge"),
            ("rebase-merge", "rebase"),
            ("rebase-apply", "rebase/am"),
            ("CHERRY_PICK_HEAD", "cherry-pick"),
            ("REVERT_HEAD", "revert"),
            ("BISECT_LOG", "bisect"),
        ):
            if (git_dir / marker).exists():
                return label
        return None

    def tracked_changes(self) -> list[str]:
        """Tracked files with staged or unstaged changes (untracked excluded)."""
        output = self.git("status", "--porcelain=v1", "-z", "--untracked-files=no").stdout
        return [entry[3:] for entry in output.split("\0") if len(entry) > 3]

    def is_dirty_vs_head(self, path: str) -> bool:
        if not self.has_head():
            return (self.root / path).exists()
        if not self.tracked(path):
            return (self.root / path).exists()  # untracked = differs from HEAD
        return self.git("diff", "--quiet", "HEAD", "--", path, check=False).returncode != 0

    def untracked(self, *paths: str) -> list[str]:
        output = self.git("ls-files", "--others", "--exclude-standard", "-z", "--", *paths).stdout
        return [p for p in output.split("\0") if p]

    def tracked(self, *paths: str) -> list[str]:
        output = self.git("ls-files", "-z", "--", *paths).stdout
        return [p for p in output.split("\0") if p]

    def status_entries(self, *paths: str) -> list[tuple[str, str]]:
        """(XY, path) entries including untracked; renames report the new path."""
        output = self.git("status", "--porcelain=v1", "-z", "--untracked-files=all", "--", *paths).stdout
        items = output.split("\0")
        result: list[tuple[str, str]] = []
        index = 0
        while index < len(items):
            entry = items[index]
            index += 1
            if len(entry) < 4:
                continue
            code, path = entry[:2], entry[3:]
            if code[0] in "RC":
                index += 1  # skip the original path that follows a rename
            result.append((code, path))
        return result

    def rev_list(self, spec: str) -> list[str]:
        output = self.git("rev-list", spec, check=False).stdout
        return [line for line in output.splitlines() if line]

    def commit_message(self, sha: str) -> str:
        return self.out("log", "-1", "--format=%B", sha)

    def changed_between(self, base: str, other: str) -> list[str]:
        output = self.git("diff", "--name-only", "-z", f"{base}...{other}").stdout
        return [p for p in output.split("\0") if p]

    def blob(self, rev: str, path: str) -> bytes | None:
        result = subprocess.run(["git", "show", f"{rev}:{path}"], cwd=self.root, capture_output=True)
        return result.stdout if result.returncode == 0 else None

    def last_commit_for(self, path: str) -> str:
        return self.git("log", "-1", "--format=%H", "--", path, check=False).stdout.strip()

    def hooks_active(self) -> bool:
        hooks_path = self.git("config", "core.hooksPath", check=False).stdout.strip()
        hook = (self.root / hooks_path / "pre-commit") if hooks_path else (self.git_dir / "hooks" / "pre-commit")
        return hook.is_file()
