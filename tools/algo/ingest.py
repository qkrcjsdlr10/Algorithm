"""Turn site submissions into repository files and verification results.

Rules
-----
* Only ``ACCEPTED`` submissions are used.
* A submission is processed once per (site, submission_key).
* A local file is verified only when its bytes equal the accepted code exactly
  (``raw``) or after the conservative normalization in hashing.py.
* An existing file is never overwritten. When the accepted code differs from the
  stored file the duplicate policy decides: keep / add_as_new / ask / ignore.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from algo import db as dbm
from algo import hashing, layout, metadata
from algo.config import Config


@dataclass
class FetchedSubmission:
    site: str
    submission_key: str
    problem_id: str
    title: str
    tier: str
    language: str
    extension: str
    code: str
    submitted_at: str = ""
    accepted: bool = True
    extra: dict[str, Any] = field(default_factory=dict)

    @property
    def code_bytes(self) -> bytes:
        return self.code.encode("utf-8")

    @property
    def lang_key(self) -> str:
        return layout.lang_key(self.language, self.extension)


def _local_candidates(root: Path, site: str, problem_id: str, lang: str) -> list[str]:
    """Existing files for the same problem + language family anywhere under the site."""
    site_root = root / site
    if not site_root.is_dir():
        return []
    result = []
    for path in site_root.rglob(f"{problem_id}_*"):
        if not path.is_file():
            continue
        relative = path.relative_to(root).as_posix()
        parsed, _ = layout.parse_path(relative)
        if parsed and parsed.problem_id == problem_id and parsed.lang_key == lang and not parsed.incomplete:
            result.append(relative)
    return sorted(result)


def _accept(database: dbm.Database, relative: str, data: bytes, submission_id: int, method: str,
            parsed: layout.ParsedPath | None) -> None:
    row = database.file(relative)
    fields = dict(status=dbm.ACCEPTED, verification_method=method, submission_id=submission_id,
                  raw_hash=hashing.raw_hash(data), normalized_hash=hashing.normalized_hash(data),
                  accepted_at=dbm.now(), last_checked_at=dbm.now(), reason=None)
    if parsed is not None:
        solution = database.solution_id(parsed.site, parsed.problem_id, parsed.lang_key, relative)
        database.add_version(solution, fields["raw_hash"], fields["normalized_hash"], relative)
        fields.update(site=parsed.site, problem_id=parsed.problem_id, lang_key=parsed.lang_key,
                      language=parsed.language, solution_id=solution)
    if row is None:
        database.upsert_file(relative, status=dbm.DISCOVERED)
    database.upsert_file(relative, **fields)


def ingest(config: Config, database: dbm.Database, fetched: FetchedSubmission,
           write_metadata: bool = True) -> str:
    """Returns one of: seen, rejected, verified, new, identical, kept, added, candidate, ignored."""
    if not fetched.accepted:
        database.add_submission(dbm.Submission(
            site=fetched.site, submission_key=fetched.submission_key, problem_id=fetched.problem_id,
            language=fetched.language, lang_key=fetched.lang_key, verdict="REJECTED", source="adapter",
            title=fetched.title, tier=fetched.tier, submitted_at=fetched.submitted_at))
        return "rejected"

    code = fetched.code_bytes
    submission_id, created = database.add_submission(dbm.Submission(
        site=fetched.site, submission_key=fetched.submission_key, problem_id=fetched.problem_id,
        language=fetched.language, lang_key=fetched.lang_key, verdict="ACCEPTED", source="adapter",
        title=fetched.title, tier=fetched.tier, raw_hash=hashing.raw_hash(code),
        normalized_hash=hashing.normalized_hash(code), submitted_at=fetched.submitted_at))
    if not created and database.get(f"done:{fetched.site}:{fetched.submission_key}"):
        return "seen"

    outcome = _place(config, database, fetched, submission_id, write_metadata)
    database.set(f"done:{fetched.site}:{fetched.submission_key}", outcome)
    database.event("INFO", f"{fetched.site} {fetched.problem_id} {fetched.language} 제출 {fetched.submission_key}: {outcome}",
                   site=fetched.site)
    return outcome


def _place(config: Config, database: dbm.Database, fetched: FetchedSubmission, submission_id: int,
           write_metadata: bool) -> str:
    root = config.root
    code = fetched.code_bytes
    site_raw = hashing.raw_hash(code)
    site_norm = hashing.normalized_hash(code)
    candidates = _local_candidates(root, fetched.site, fetched.problem_id, fetched.lang_key)

    for relative in candidates:
        data = (root / relative).read_bytes()
        kind = hashing.match_kind(data, site_raw, site_norm)
        if kind is None:
            continue
        row = database.file(relative)
        _record_metadata(config, database, fetched, relative, site_raw, write_metadata, only_if_missing=True)
        # The engine scans before ingesting, so a file without a row is a clean
        # tracked file that is already in the repository.
        if row is not None and row["status"] in (dbm.DISCOVERED, dbm.PENDING, dbm.FAILED):
            _accept(database, relative, data, submission_id, f"auto-hash-{kind}", layout.parse_path(relative)[0])
            return "verified"
        return "identical"

    if not candidates:
        relative = layout.build_path(fetched.site, fetched.tier, fetched.problem_id, fetched.title,
                                     fetched.language, fetched.extension)
        target = root / relative
        if target.exists():
            # Name taken by a file the parser could not attribute: never overwrite.
            return _differs(config, database, fetched, submission_id, [relative])
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(code)
        _accept(database, relative, code, submission_id, "site-download", layout.parse_path(relative)[0])
        _record_metadata(config, database, fetched, relative, site_raw, write_metadata)
        return "new"

    return _differs(config, database, fetched, submission_id, candidates)


def _differs(config: Config, database: dbm.Database, fetched: FetchedSubmission, submission_id: int,
             candidates: list[str]) -> str:
    policy = config.duplicate_policy
    reason = f"사이트 정답 코드(제출 {fetched.submission_key})와 내용이 다릅니다"
    for relative in candidates:
        row = database.file(relative)
        if row is not None and row["status"] in (dbm.DISCOVERED, dbm.PENDING):
            database.upsert_file(relative, reason=reason, last_checked_at=dbm.now())

    if policy == "ignore":
        return "ignored"
    if policy == "keep":
        return "kept"
    if policy == "add_as_new":
        base = candidates[0]
        version = 2
        while (config.root / layout.versioned_path(base, version)).exists():
            version += 1
        relative = layout.versioned_path(base, version)
        (config.root / relative).write_bytes(fetched.code_bytes)
        _accept(database, relative, fetched.code_bytes, submission_id, "site-download",
                layout.parse_path(relative)[0])
        return "added"
    # ask: keep the file, store the site version for ``algo approve --use-site``
    store = config.state_dir / "candidates" / fetched.site / f"{fetched.submission_key}{fetched.extension}"
    store.parent.mkdir(parents=True, exist_ok=True)
    store.write_bytes(fetched.code_bytes)
    with database.tx() as c:
        c.execute("INSERT OR IGNORE INTO candidates (submission_id, target_path, stored_path, status, created_at) "
                  "VALUES (?, ?, ?, 'OPEN', ?)", (submission_id, candidates[0], str(store), dbm.now()))
    return "candidate"


def _record_metadata(config: Config, database: dbm.Database, fetched: FetchedSubmission, relative: str,
                     site_raw: str, enabled: bool, only_if_missing: bool = False) -> None:
    if not enabled or fetched.site not in metadata.METADATA_SITES:
        return
    existing = {metadata.entry_key(item): item for item in metadata.read(config.root, fetched.site)}
    key = f"{fetched.problem_id}|{fetched.language.casefold()}"
    if only_if_missing and key in existing:
        return
    site_relative = relative.split("/", 1)[1]
    entry = metadata.build_entry(fetched.site, fetched, site_relative, site_raw)
    path = metadata.upsert(config.root, fetched.site, entry)
    database.add_aux(path.relative_to(config.root).as_posix())
    if fetched.site == "Programmers":
        database.add_aux("Programmers/README.md")


def verify_pending(config: Config, database: dbm.Database) -> int:
    """Match pending local files against accepted submissions already known
    (from adapters or existing metadata). Returns the number verified."""
    verified = 0
    for row in database.files_with_status(dbm.PENDING, dbm.DISCOVERED):
        if not row["site"] or row["external_source"]:
            continue
        path = config.root / row["path"]
        if not path.is_file():
            continue
        data = path.read_bytes()
        for sub in database.accepted_submissions(row["site"], row["problem_id"], row["lang_key"]):
            kind = hashing.match_kind(data, sub["raw_hash"], sub["normalized_hash"])
            if kind:
                _accept(database, row["path"], data, int(sub["id"]), f"auto-hash-{kind}",
                        layout.parse_path(row["path"])[0])
                verified += 1
                break
        else:
            known = database.accepted_submissions(row["site"], row["problem_id"], row["lang_key"])
            reason = ("사이트 정답 제출과 코드가 다릅니다 (approve로 직접 확인)" if known
                      else "정답 제출 기록을 아직 찾지 못했습니다")
            database.upsert_file(row["path"], reason=reason, last_checked_at=dbm.now())
    return verified


def ingest_metadata(config: Config, database: dbm.Database) -> int:
    """Register accepted submissions recorded in metadata.json (by the backup
    scripts or the LeetCode cloud workflow). Idempotent."""
    added = 0
    for site in metadata.METADATA_SITES:
        for item in metadata.read(config.root, site):
            sub = metadata.to_submission(site, item)
            if sub is None:
                continue
            _, created = database.add_submission(sub)
            added += int(created)
    return added
