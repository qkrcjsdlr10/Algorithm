"""Baseline: register what is already in the repository so the first run never
treats hundreds of past solutions as new uploads. Safe to run repeatedly."""

from __future__ import annotations

from dataclasses import dataclass, field

from algo import db as dbm
from algo import hashing, layout, metadata
from algo.config import Config
from algo.repo import Repo


@dataclass
class BaselineReport:
    tracked_files: int = 0
    newly_registered: int = 0
    metadata_submissions: int = 0
    linked: int = 0
    hash_mismatch: list[str] = field(default_factory=list)
    missing_files: list[str] = field(default_factory=list)
    unidentified: list[str] = field(default_factory=list)


def baseline(config: Config, database: dbm.Database, repo: Repo) -> BaselineReport:
    report = BaselineReport()
    for relative in repo.tracked(*layout.watched_roots()):
        path = config.root / relative
        if path.suffix.casefold() not in layout.SOURCE_EXTENSIONS or not path.is_file():
            continue
        report.tracked_files += 1
        if database.file(relative) is not None:
            continue
        data = path.read_bytes()
        parsed, _ = layout.parse_path(relative)
        fields = dict(status=dbm.SYNCED, verification_method="baseline", raw_hash=hashing.raw_hash(data),
                      normalized_hash=hashing.normalized_hash(data), reason=None,
                      github_commit_sha=repo.last_commit_for(relative))
        if parsed is not None:
            solution = database.solution_id(parsed.site, parsed.problem_id, parsed.lang_key, relative)
            database.add_version(solution, fields["raw_hash"], fields["normalized_hash"], relative)
            fields.update(site=parsed.site, problem_id=parsed.problem_id, lang_key=parsed.lang_key,
                          language=parsed.language, solution_id=solution)
        else:
            report.unidentified.append(relative)
            fields["site"] = relative.split("/", 1)[0]
        database.upsert_file(relative, **fields)
        report.newly_registered += 1

    for site in metadata.METADATA_SITES:
        for item in metadata.read(config.root, site):
            submission = metadata.to_submission(site, item)
            if submission is None:
                continue
            submission_id, created = database.add_submission(submission)
            report.metadata_submissions += int(created)
            relative = f"{site}/{item['file']}"
            path = config.root / relative
            if not path.is_file():
                report.missing_files.append(relative)
                continue
            if hashing.match_kind(path.read_bytes(), submission.raw_hash) is None:
                report.hash_mismatch.append(relative)
                row = database.file(relative)
                if row is not None and row["status"] == dbm.SYNCED:
                    database.upsert_file(relative, reason="기존 파일이 metadata의 제출 코드와 다릅니다 (변경하지 않음)")
                continue
            row = database.file(relative)
            if row is not None and row["submission_id"] is None:
                database.upsert_file(relative, submission_id=submission_id)
                report.linked += 1
            database.set(f"done:{site}:{submission.submission_key}", "baseline")
    database.set("baseline_at", dbm.now())
    return report
