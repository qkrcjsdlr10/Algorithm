"""Commit and push accepted solution files without touching the user's work.

Commit (alternate index):
  1. refuse while a merge/rebase/cherry-pick is in progress, or off the target branch
  2. refuse when README.md has uncommitted edits (the stats update would overwrite them)
  3. GIT_INDEX_FILE=.git/algo-index, ``git read-tree HEAD``  -> index == HEAD
  4. add only the agent's files (+ metadata files the agent wrote)
  5. run scripts/update_stats.py --staged against that index and add README.md to it
  6. verify the staged set is exactly what was planned, then ``git commit``
     (the existing pre-commit hook still runs and sees the same alternate index)
  7. re-sync the user's real index for exactly those paths (``git reset -q -- <paths>``)
     so the next ordinary commit does not undo the agent's commit

Push:
  * only to ``origin`` and only commits carrying the ``Algo-Sync`` trailer;
    if the user has their own unpushed commits the push waits for them
  * when origin moved: rebase only when no tracked file is modified, abort on
    conflict and retry later; untracked files that would be overwritten are moved
    to .algo/backup only when identical to the incoming version
"""

from __future__ import annotations

import shutil
import subprocess
import sys
import time
from dataclasses import dataclass
from datetime import datetime, timedelta
from pathlib import Path

from algo import db as dbm
from algo import hashing
from algo.config import Config
from algo.repo import GitError, Repo

TRAILER = "Algo-Sync: auto"


class RetryLater(RuntimeError):
    """The operation is safe to retry later; nothing was changed."""


@dataclass
class PublishResult:
    committed: str | None = None
    pushed: bool = False
    message: str = ""


class Publisher:
    def __init__(self, config: Config, database: dbm.Database, repo: Repo) -> None:
        self.config = config
        self.db = database
        self.repo = repo

    # ------------------------------------------------------------------
    def uncommitted(self) -> list:
        return [r for r in self.db.files_with_status(dbm.ACCEPTED) if not r["commit_sha"]]

    def unpushed(self) -> list:
        return [r for r in self.db.files_with_status(dbm.ACCEPTED) if r["commit_sha"]]

    def _preflight(self) -> None:
        operation = self.repo.operation_in_progress()
        if operation:
            raise RetryLater(f"Git {operation} 작업이 진행 중입니다")
        branch = self.repo.current_branch()
        if branch != self.config.branch:
            raise RetryLater(f"현재 브랜치가 {self.config.branch}가 아닙니다 ({branch or 'detached HEAD'})")
        url = self.repo.remote_url()
        if self.config.expected_remote and self.config.expected_remote.casefold() not in url.casefold():
            raise RetryLater(f"{self.config.remote} 주소가 예상과 다릅니다: {url or '(없음)'}")

    def _message(self, rows: list) -> str:
        names = [f"{r['site']} {r['problem_id']} ({r['language']})" for r in rows]
        if len(rows) == 1:
            subject = f"sync: {names[0]}"
        else:
            subject = f"sync: {len(rows)} solutions"
        body = "\n".join(f"- {r['path']}" for r in rows)
        return f"{subject}\n\n{body}\n\n{TRAILER}\n"

    # ------------------------------------------------------------------
    def commit(self, rows: list | None = None) -> str | None:
        rows = list(rows if rows is not None else self.uncommitted())
        if not rows:
            return None
        self._preflight()
        if self.repo.is_dirty_vs_head("README.md"):
            raise RetryLater("README.md에 커밋되지 않은 변경이 있어 문제 수 갱신을 미룹니다")

        ready: list = []
        for row in rows:
            path = self.repo.root / row["path"]
            if not path.is_file():
                self.db.upsert_file(row["path"], status=dbm.FAILED, reason="파일이 사라졌습니다")
                continue
            data = path.read_bytes()
            if hashing.raw_hash(data) != row["raw_hash"]:
                self.db.upsert_file(row["path"], status=dbm.PENDING, reason="정답 확인 뒤 파일이 수정되어 다시 확인합니다",
                                    raw_hash=hashing.raw_hash(data), normalized_hash=hashing.normalized_hash(data),
                                    verification_method=None, submission_id=None)
                continue
            ready.append(row)
        if not ready:
            return None

        paths = [r["path"] for r in ready]
        aux = [p for p in self.db.aux() if (self.repo.root / p).exists() and self.repo.is_dirty_vs_head(p)]
        index = self.repo.git_dir / "algo-index"
        index.unlink(missing_ok=True)
        try:
            if self.repo.has_head():
                self.repo.git("read-tree", "HEAD", index_file=index)
            else:
                self.repo.git("read-tree", "--empty", index_file=index)
            self.repo.git("add", "--", *paths, *aux, index_file=index)
            self._update_stats(index)
            planned = set(paths) | set(aux) | {"README.md"}
            staged = set(
                p for p in self.repo.git("diff", "--cached", "--name-only", "-z", index_file=index).stdout.split("\0") if p
            ) if self.repo.has_head() else set(paths) | set(aux) | {"README.md"}
            unexpected = staged - planned
            if unexpected:
                raise RetryLater(f"예상하지 못한 파일이 커밋에 포함될 뻔했습니다: {sorted(unexpected)}")
            if not staged & set(paths) and not staged & set(aux):
                for row in ready:
                    self._mark_synced_without_commit(row)
                self.db.clear_aux(aux)
                return None
            self.repo.git("commit", "--quiet", "-F", "-", index_file=index,
                          input_bytes=self._message(ready).encode("utf-8"))
            sha = self.repo.head()
        except GitError as error:
            raise RetryLater(f"커밋 실패: {error}") from error
        finally:
            index.unlink(missing_ok=True)

        # Bring the user's real index in line with HEAD for exactly these paths.
        sync_paths = [p for p in [*paths, *aux, "README.md"] if self.repo.tracked(p) or (self.repo.root / p).exists()]
        self.repo.git("reset", "-q", "--", *sync_paths, check=False)
        for row in ready:
            self.db.upsert_file(row["path"], commit_sha=sha, reason=None)
        self.db.clear_aux(aux)
        self.db.event("INFO", f"커밋 {sha[:10]}: {len(ready)}개 풀이", path=", ".join(paths))
        return sha

    def _mark_synced_without_commit(self, row) -> None:
        self.db.upsert_file(row["path"], status=dbm.SYNCED, reason="이미 HEAD와 같은 내용입니다",
                            github_commit_sha=self.repo.last_commit_for(row["path"]))

    def _update_stats(self, index: Path) -> None:
        script = self.repo.root / "scripts" / "update_stats.py"
        if not script.is_file():
            return
        result = subprocess.run(
            [sys.executable, str(script), "--staged"],
            cwd=self.repo.root,
            env={**_env_without_index(), "GIT_INDEX_FILE": str(index)},
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        )
        if result.returncode != 0:
            raise RetryLater(f"README 집계 실패: {result.stderr.strip() or result.stdout.strip()}")
        self.repo.git("add", "--", "README.md", index_file=index)

    # ------------------------------------------------------------------
    def fetch(self) -> None:
        try:
            self.repo.git("fetch", "--quiet", self.config.remote, self.config.branch)
        except GitError as error:
            raise RetryLater(f"원격 저장소에 연결하지 못했습니다: {error}") from error

    def _remote_ref(self) -> str:
        return f"{self.config.remote}/{self.config.branch}"

    def _move_identical_untracked(self) -> None:
        incoming = set(self.repo.changed_between("HEAD", self._remote_ref()))
        if not incoming:
            return
        collisions = [p for p in self.repo.untracked() if p in incoming]
        if not collisions:
            return
        stamp = datetime.now().strftime("%Y%m%d-%H%M%S")
        for path in collisions:
            remote_blob = self.repo.blob(self._remote_ref(), path)
            local = (self.repo.root / path).read_bytes()
            if remote_blob is None or hashing.normalize(remote_blob) != hashing.normalize(local):
                self.db.upsert_file(path, status=dbm.PENDING, reason="원격 저장소에 다른 내용의 같은 파일이 있습니다")
                raise RetryLater(f"원격에 같은 이름의 다른 파일이 있습니다: {path}")
        for path in collisions:
            backup = self.config.state_dir / "backup" / stamp / path
            backup.parent.mkdir(parents=True, exist_ok=True)
            shutil.move(str(self.repo.root / path), str(backup))
            self.db.event("INFO", f"원격과 동일한 미추적 파일을 백업 후 원격 버전을 받습니다: {backup}", path=path)
            if self.db.file(path):
                self.db.upsert_file(path, status=dbm.SYNCED, verification_method="remote-identical",
                                    reason="원격 저장소에 같은 내용이 이미 있습니다")

    def integrate_remote(self) -> bool:
        """Bring origin's new commits in. Returns True when HEAD now contains origin.

        Only fast-forwards, or rebases commits the agent made itself; the user's
        own commits are never rewritten."""
        self._preflight()
        self.fetch()
        behind = self.repo.rev_list(f"HEAD..{self._remote_ref()}")
        if not behind:
            return True
        if self.repo.tracked_changes():
            raise RetryLater("작업 트리에 수정 중인 파일이 있어 원격 변경을 받지 않았습니다")
        ahead = self.repo.rev_list(f"{self._remote_ref()}..HEAD")
        foreign = [sha for sha in ahead if TRAILER not in self.repo.commit_message(sha)]
        if foreign:
            raise RetryLater(f"직접 만든 커밋 {len(foreign)}개가 있어 원격 변경 통합을 사용자에게 맡깁니다")
        self._move_identical_untracked()
        try:
            if ahead:
                self.repo.git("rebase", "--quiet", self._remote_ref())
            else:
                self.repo.git("merge", "--ff-only", "--quiet", self._remote_ref())
        except GitError as error:
            if self.repo.operation_in_progress():
                self.repo.git("rebase", "--abort", check=False)
            raise RetryLater(f"원격 변경과 충돌하여 되돌렸습니다: {error}") from error
        return True

    def push(self) -> bool:
        self._preflight()
        for attempt in range(3):
            self.integrate_remote()
            ahead = self.repo.rev_list(f"{self._remote_ref()}..HEAD")
            if not ahead:
                self._mark_pushed()
                return False
            foreign = [sha for sha in ahead if TRAILER not in self.repo.commit_message(sha)]
            if foreign:
                raise RetryLater(f"직접 만든 커밋 {len(foreign)}개가 아직 푸시되지 않아 자동 푸시를 멈췄습니다. "
                                 "직접 푸시한 뒤 다시 시도됩니다")
            result = self.repo.git("push", "--quiet", self.config.remote, f"HEAD:refs/heads/{self.config.branch}",
                                   check=False)
            if result.returncode == 0:
                self._mark_pushed()
                self.db.event("INFO", f"푸시 완료: 커밋 {len(ahead)}개")
                return True
            if "rejected" not in result.stderr and "fetch first" not in result.stderr:
                raise RetryLater(f"푸시 실패: {result.stderr.strip()}")
            time.sleep(1 + attempt)
        raise RetryLater("원격 저장소가 계속 바뀌어 푸시하지 못했습니다")

    def _mark_pushed(self) -> None:
        """Mark files SYNCED once the commit holding them is on origin. Commit SHAs
        may have changed through a rebase, so look the commit up by path."""
        for row in self.unpushed():
            sha = self.repo.last_commit_for(row["path"])
            if not sha:
                continue
            on_remote = self.repo.git("merge-base", "--is-ancestor", sha, self._remote_ref(),
                                      check=False).returncode == 0
            if on_remote:
                self.db.upsert_file(row["path"], status=dbm.SYNCED, github_commit_sha=sha,
                                    commit_sha=sha, reason=None, attempts=0)

    # ------------------------------------------------------------------
    def due(self, rows: list, mode: str) -> bool:
        if not rows:
            return False
        if mode == "immediate":
            return True
        if mode == "batch":
            oldest = min(datetime.fromisoformat(r["accepted_at"] or r["updated_at"]) for r in rows)
            return datetime.now(oldest.tzinfo) - oldest >= timedelta(minutes=self.config.batch_minutes)
        return mode == "commit_only"

    def run(self, mode: str | None = None, force: bool = False) -> PublishResult:
        """Apply the publish policy. ``force`` publishes now (used by approve/push)."""
        mode = mode or self.config.publish_mode
        result = PublishResult()
        waiting = self.uncommitted()
        if waiting and (force or (mode != "manual" and self.due(waiting, mode))):
            result.committed = self.commit(waiting)
        if (force and mode != "commit_only") or mode in {"immediate", "batch"}:
            if self.unpushed() or self._ahead_with_trailer():
                result.pushed = self.push()
        return result

    def _ahead_with_trailer(self) -> bool:
        if not self.repo.has_head():
            return False
        ahead = self.repo.rev_list(f"{self._remote_ref()}..HEAD")
        return bool(ahead) and all(TRAILER in self.repo.commit_message(sha) for sha in ahead)


def _env_without_index() -> dict[str, str]:
    import os

    env = dict(os.environ)
    env.pop("GIT_INDEX_FILE", None)
    return env
