"""Detect new or modified solution files (IDE work) via ``git status``.

Polling ``git status`` instead of relying only on file-system events means work
done while the agent was not running is still found on the next start. A
watchdog observer (daemon.py) merely triggers a scan sooner."""

from __future__ import annotations

from pathlib import Path

from algo import db as dbm
from algo import hashing, layout
from algo.config import Config
from algo.repo import Repo

ACTIVE = (dbm.DISCOVERED, dbm.PENDING, dbm.ACCEPTED, dbm.FAILED)


def _hashes(path: Path) -> tuple[str, str]:
    data = path.read_bytes()
    return hashing.raw_hash(data), hashing.normalized_hash(data)


def register(database: dbm.Database, relative: str, raw: str, normalized: str,
             external_source: str | None = None) -> str:
    """Create/refresh the files row for one changed file. Returns its status."""
    parsed, reason = layout.parse_path(relative)
    row = database.file(relative)
    if row is not None and row["raw_hash"] == raw and row["status"] in ACTIVE + (dbm.SKIPPED,):
        return row["status"]
    if row is not None and row["status"] == dbm.SYNCED and row["raw_hash"] == raw:
        return row["status"]

    fields: dict = dict(raw_hash=raw, normalized_hash=normalized, verification_method=None,
                        submission_id=None, commit_sha=None, accepted_at=None, external_source=external_source)
    if parsed is None:
        fields.update(status=dbm.PENDING, reason=f"확인 필요: {reason}", site=None, problem_id=None,
                      lang_key=None, language=None)
    elif parsed.incomplete:
        fields.update(status=dbm.SKIPPED, reason="미완성 파일은 자동 업로드하지 않습니다", site=parsed.site,
                      problem_id=parsed.problem_id, lang_key=parsed.lang_key, language=parsed.language)
    else:
        solution = database.solution_id(parsed.site, parsed.problem_id, parsed.lang_key)
        database.add_version(solution, raw, normalized, relative)
        fields.update(status=dbm.PENDING, reason="정답 제출 확인 대기", site=parsed.site,
                      problem_id=parsed.problem_id, lang_key=parsed.lang_key, language=parsed.language,
                      solution_id=solution)
    if row is None:
        database.upsert_file(relative, status=dbm.DISCOVERED, raw_hash=raw, normalized_hash=normalized)
    database.upsert_file(relative, **fields)
    return fields["status"]


def scan(config: Config, database: dbm.Database, repo: Repo) -> dict[str, int]:
    found = {"changed": 0, "removed": 0, "user_committed": 0}
    changed: set[str] = set()
    for code, relative in repo.status_entries(*layout.watched_roots()):
        if code.strip() == "D" or "D" in code:
            continue
        path = config.root / relative
        if not path.is_file() or path.suffix.casefold() not in layout.SOURCE_EXTENSIONS:
            continue
        changed.add(relative)
        raw, normalized = _hashes(path)
        before = database.file(relative)
        status = register(database, relative, raw, normalized)
        if before is None or before["raw_hash"] != raw:
            found["changed"] += 1
            database.event("INFO", f"변경 감지 → {status}", site=layout.parse_path(relative)[0].site
                           if layout.parse_path(relative)[0] else None, path=relative)

    # Rows that are no longer changed: deleted, or committed by the user.
    for row in database.files_with_status(*ACTIVE):
        relative = row["path"]
        if relative in changed or row["external_source"]:
            continue
        if row["status"] == dbm.ACCEPTED and row["commit_sha"]:
            continue
        path = config.root / relative
        if not path.exists():
            database.upsert_file(relative, status=dbm.SKIPPED, reason="파일이 삭제되었습니다")
            found["removed"] += 1
        elif repo.tracked(relative) and not repo.is_dirty_vs_head(relative):
            database.upsert_file(relative, status=dbm.SYNCED, verification_method=row["verification_method"]
                                 or "user-commit", reason="사용자가 직접 커밋했습니다",
                                 github_commit_sha=repo.last_commit_for(relative))
            found["user_committed"] += 1
    return found


def scan_external(config: Config, database: dbm.Database) -> int:
    """Files in extra watch folders (outside the repository). They are never
    identified automatically; ``algo approve`` copies them in after confirmation."""
    count = 0
    for folder in config.extra_watch_dirs:
        if not folder.is_dir():
            continue
        for path in folder.rglob("*"):
            if not path.is_file() or path.suffix.casefold() not in {".java", ".cpp", ".cc", ".c", ".sql"}:
                continue
            key = f"external:{path.resolve().as_posix()}"
            raw, normalized = _hashes(path)
            row = database.file(key)
            if row is not None and row["raw_hash"] == raw:
                continue
            database.upsert_file(key, status=dbm.PENDING, raw_hash=raw, normalized_hash=normalized,
                                 external_source=str(path.resolve()),
                                 reason="외부 폴더 파일: approve --site --tier --problem --title 로 등록하세요")
            count += 1
    return count
