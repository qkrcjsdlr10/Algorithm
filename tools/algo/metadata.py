"""Read/write the existing per-site ``metadata.json`` files in the same format
the backup scripts use, so the scripts and the agent stay interchangeable."""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any
from urllib.parse import quote

from algo import layout
from algo.db import Submission

METADATA_SITES = ("LeetCode", "Programmers", "SWEA")


def metadata_path(root: Path, site: str) -> Path:
    return root / site / "metadata.json"


def read(root: Path, site: str) -> list[dict[str, Any]]:
    path = metadata_path(root, site)
    if site not in METADATA_SITES or not path.is_file():
        return []
    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, list):
        raise ValueError(f"{path} must contain a JSON array")
    return [item for item in data if isinstance(item, dict)]


def entry_key(item: dict[str, Any]) -> str:
    return f"{item.get('id')}|{str(item.get('language')).casefold()}"


def submission_key(site: str, item: dict[str, Any]) -> str:
    for field in ("submissionId", "contestHistoryId"):
        value = item.get(field)
        if value not in (None, ""):
            return str(value)
    return f"meta:{item.get('id')}:{item.get('language')}:{str(item.get('sha256') or '')[:16]}"


def to_submission(site: str, item: dict[str, Any]) -> Submission | None:
    file = str(item.get("file") or "")
    problem_id = str(item.get("id") or "")
    language = str(item.get("language") or "")
    if not (problem_id and language and file):
        return None
    extension = Path(file).suffix
    return Submission(
        site=site,
        submission_key=submission_key(site, item),
        problem_id=problem_id,
        language=language,
        lang_key=layout.lang_key(language, extension),
        verdict="ACCEPTED",
        source="metadata",
        title=str(item.get("title") or ""),
        tier=str(item.get("level") or item.get("difficulty") or ""),
        raw_hash=str(item.get("sha256") or "") or None,
        submitted_at=str(item.get("submittedAt") or ""),
    )


def _sort_key(site: str):
    if site == "LeetCode":
        order = {"Easy": 0, "Medium": 1, "Hard": 2}
        return lambda item: (
            order.get(str(item.get("difficulty")), 3),
            int(item["id"]) if str(item.get("id", "")).isdigit() else 10**12,
            str(item.get("id")),
            str(item.get("language")),
        )
    return lambda item: (
        item.get("level") == "Unclassified",
        str(item.get("level") or ""),
        int(item["id"]) if str(item.get("id", "")).isdigit() else 0,
        str(item.get("language") or ""),
    )


def upsert(root: Path, site: str, entry: dict[str, Any]) -> Path:
    """Insert or replace one ``id|language`` entry; returns the metadata path."""
    entries = {entry_key(item): item for item in read(root, site)}
    entries[entry_key(entry)] = entry
    ordered = sorted(entries.values(), key=_sort_key(site))
    path = metadata_path(root, site)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(".json.tmp")
    temporary.write_bytes((json.dumps(ordered, ensure_ascii=False, indent=2) + "\n").encode("utf-8"))
    temporary.replace(path)
    if site == "Programmers":
        write_programmers_readme(root, ordered)
    return path


def write_programmers_readme(root: Path, entries: list[dict[str, Any]]) -> None:
    """Same listing that scripts/programmers_backup.py writes."""
    lines = [
        "# Programmers Solutions",
        "",
        "프로그래머스에서 통과한 개인 제출 코드의 백업 목록입니다.",
        "",
        "| 문제 | 난이도 | 언어 | 파일 |",
        "|---|---:|---|---|",
    ]
    for item in entries:
        title = str(item.get("title") or "").replace("|", "\\|")
        level = str(item.get("level") or "Unclassified")
        language = str(item.get("language") or "Unknown")
        relative = str(item.get("file") or "")
        link = quote(relative, safe="/._-()")
        lines.append(f"| {title} | {level} | {language} | [{relative}]({link}) |")
    lines.append("")
    (root / "Programmers" / "README.md").write_bytes("\n".join(lines).encode("utf-8"))


def build_entry(site: str, fetched: Any, relative_to_site: str, raw_sha: str) -> dict[str, Any]:
    """Metadata entry in each site's existing schema."""
    extra = dict(fetched.extra or {})
    if site == "LeetCode":
        return {
            "id": fetched.problem_id,
            "title": fetched.title,
            "slug": extra.get("slug", ""),
            "difficulty": fetched.tier,
            "language": fetched.language,
            "file": relative_to_site,
            "submittedAt": fetched.submitted_at,
            "url": f"https://leetcode.com/problems/{extra.get('slug', '')}/",
            "submissionId": fetched.submission_key,
            "result": "Accepted",
            "sha256": raw_sha,
        }
    if site == "Programmers":
        return {
            "id": fetched.problem_id,
            "title": fetched.title,
            "level": fetched.tier,
            "language": fetched.language,
            "sourceLanguage": extra.get("sourceLanguage"),
            "file": relative_to_site,
            "submittedAt": fetched.submitted_at,
            "accepted": True,
            "score": extra.get("score"),
            "perfectScore": extra.get("perfectScore"),
            "submissionId": _maybe_int(fetched.submission_key),
            "url": f"https://school.programmers.co.kr/learn/courses/30/lessons/{fetched.problem_id}",
            "sha256": raw_sha,
            "lastAction": "NEW",
        }
    if site == "SWEA":
        return {
            "id": fetched.problem_id,
            "title": fetched.title,
            "level": fetched.tier,
            "language": fetched.language,
            "file": relative_to_site,
            "submittedAt": fetched.submitted_at,
            "result": "Accepted",
            "siteResult": "Pass",
            "contestProbId": extra.get("contestProbId", ""),
            "contestHistoryId": fetched.submission_key,
            "sha256": raw_sha,
        }
    raise ValueError(f"{site} has no metadata file")


def _maybe_int(value: str) -> Any:
    return int(value) if str(value).isdigit() else value
