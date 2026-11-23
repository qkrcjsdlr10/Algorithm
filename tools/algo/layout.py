"""Repository layout rules: which folders hold which site's solutions and how
solution files are named. Mirrors the existing conventions in README.md and the
backup scripts; nothing here renames or moves existing files."""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import PurePosixPath

INVALID_FILENAME_CHARS = re.compile(r'[\\/:*?"<>|\x00-\x1f]')
WHITESPACE = re.compile(r"\s+")
UNDERSCORES = re.compile(r"_+")

SOURCE_EXTENSIONS = {".java", ".cpp", ".cc", ".c", ".sql", ".py", ".kt", ".js", ".ts"}
EXTENSION_LANGUAGE = {
    ".java": "Java",
    ".cpp": "CPP",
    ".cc": "CPP",
    ".c": "C",
    ".sql": "SQL",
    ".py": "Python",
    ".kt": "Kotlin",
    ".js": "JavaScript",
    ".ts": "TypeScript",
}
SQL_LANGUAGES = {"sql", "mysql", "oracle", "oraclesql", "mssql", "postgresql"}
INCOMPLETE = re.compile(r"(?:^|_)(?:미완성|incomplete)(?:_|$)", re.IGNORECASE)
VERSION_SUFFIX = re.compile(r"_v(\d+)$")


@dataclass(frozen=True)
class SiteLayout:
    name: str
    tiers: tuple[str, ...]
    sql_tiers: tuple[str, ...] = ()
    extra_tiers: tuple[str, ...] = ()

    def all_tiers(self) -> tuple[str, ...]:
        return self.tiers + self.extra_tiers


SITES: dict[str, SiteLayout] = {
    # Same tier folders that scripts/update_stats.py counts.
    "BOJ": SiteLayout("BOJ", ("Bronze", "Silver", "Gold", "Platinum")),
    "Programmers": SiteLayout(
        "Programmers",
        ("Lv0", "Lv1", "Lv2", "Lv3", "Lv4", "Lv5"),
        sql_tiers=("Lv1", "Lv2", "Lv3", "Lv4", "Lv5"),
    ),
    "SWEA": SiteLayout(
        "SWEA", ("D1", "D2", "D3", "D4", "D5", "D6"), extra_tiers=("Unclassified",)
    ),
    "LeetCode": SiteLayout(
        "LeetCode", ("Easy", "Medium", "Hard"), sql_tiers=("Easy", "Medium", "Hard")
    ),
}
# CodeTree is mirrored by GitHub Actions from a separate repository; the local
# agent never watches or writes it.
MANAGED_ELSEWHERE = {"CodeTree"}
SITE_KEYS = {name.casefold(): name for name in SITES}


def site_name(value: str) -> str | None:
    return SITE_KEYS.get(value.strip().casefold())


def sanitize_component(value: str) -> str:
    value = INVALID_FILENAME_CHARS.sub("_", value)
    value = WHITESPACE.sub("_", value.strip())
    value = UNDERSCORES.sub("_", value)
    return value.strip(" ._") or "Untitled"


def lang_key(language: str, extension: str) -> str:
    """Key that separates solutions of one problem: one file per language family."""
    extension = extension.casefold()
    if extension in EXTENSION_LANGUAGE:
        return EXTENSION_LANGUAGE[extension].casefold()
    return re.sub(r"[^a-z0-9+#]", "", language.casefold()) or extension.lstrip(".")


def is_sql(language: str, extension: str) -> bool:
    return extension.casefold() == ".sql" or language.casefold() in SQL_LANGUAGES


@dataclass(frozen=True)
class ParsedPath:
    site: str
    tier: str
    problem_id: str
    language: str
    extension: str
    is_sql: bool
    incomplete: bool

    @property
    def lang_key(self) -> str:
        return lang_key(self.language, self.extension)


def parse_path(relative: str) -> tuple[ParsedPath | None, str]:
    """Identify site / tier / problem / language from a repository-relative path.

    Returns ``(None, reason)`` instead of guessing when anything is ambiguous."""
    path = PurePosixPath(relative.replace("\\", "/"))
    parts = path.parts
    if not parts:
        return None, "빈 경로"
    if parts[0] in MANAGED_ELSEWHERE:
        return None, f"{parts[0]}는 GitHub Actions가 관리합니다"
    site = site_name(parts[0])
    if site is None:
        return None, "사이트 폴더(BOJ/Programmers/SWEA/LeetCode) 밖의 파일입니다"
    layout = SITES[site]
    extension = path.suffix.casefold()
    if extension not in SOURCE_EXTENSIONS:
        return None, f"지원하지 않는 확장자: {extension or '(없음)'}"
    sql = len(parts) == 4 and parts[1] == "SQL"
    if sql:
        tier = parts[2]
        if tier not in layout.sql_tiers:
            return None, f"{site}/SQL 아래의 알 수 없는 난이도 폴더: {tier}"
    elif len(parts) == 3:
        tier = parts[1]
        if tier not in layout.all_tiers():
            return None, f"{site} 아래의 알 수 없는 난이도 폴더: {tier}"
    else:
        return None, f"{site}/<난이도>/<파일> 구조가 아닙니다"
    stem = path.stem
    match = re.match(r"^(\d+)_", stem)
    if not match:
        return None, "파일명이 문제 번호로 시작하지 않습니다"
    language = EXTENSION_LANGUAGE[extension]
    tag_stem = VERSION_SUFFIX.sub("", stem)
    tag_stem = re.sub(r"_(?:미완성|incomplete)$", "", tag_stem, flags=re.IGNORECASE)
    tag = tag_stem.rsplit("_", 1)[-1]
    if extension == ".sql" and tag.casefold() in SQL_LANGUAGES:
        language = tag
    return (
        ParsedPath(
            site=site,
            tier=tier,
            problem_id=match.group(1),
            language=language,
            extension=extension,
            is_sql=sql or extension == ".sql",
            incomplete=bool(INCOMPLETE.search(stem)),
        ),
        "",
    )


def tier_for(site: str, tier: str, sql: bool) -> str:
    layout = SITES[site]
    allowed = layout.sql_tiers if sql else layout.all_tiers()
    if tier in allowed:
        return tier
    if site == "SWEA" and not sql:
        return "Unclassified"
    raise ValueError(f"{site}에서 사용할 수 없는 난이도: {tier}")


def build_path(
    site: str, tier: str, problem_id: str, title: str, language: str, extension: str
) -> str:
    sql = is_sql(language, extension)
    tier = tier_for(site, tier, sql)
    filename = f"{problem_id}_{sanitize_component(title)}_{sanitize_component(language)}{extension}"
    if len(filename) > 180:
        keep = 180 - len(problem_id) - len(language) - len(extension) - 2
        filename = (
            f"{problem_id}_{sanitize_component(title)[:keep].rstrip(' ._')}_"
            f"{sanitize_component(language)}{extension}"
        )
    folder = f"{site}/SQL/{tier}" if sql and SITES[site].sql_tiers else f"{site}/{tier}"
    return f"{folder}/{filename}"


def versioned_path(relative: str, version: int) -> str:
    path = PurePosixPath(relative)
    return str(path.with_name(f"{VERSION_SUFFIX.sub('', path.stem)}_v{version}{path.suffix}"))


def watched_roots() -> list[str]:
    return list(SITES)
