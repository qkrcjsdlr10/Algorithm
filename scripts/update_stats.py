#!/usr/bin/env python3
"""Update the unique solved-problem counts in the repository README."""

from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
README_PATH = REPOSITORY_ROOT / "README.md"
STATS_START = "<!-- STATS_START -->"
STATS_END = "<!-- STATS_END -->"
PROBLEM_ID = re.compile(r"^(\d+)(?:_|$)")
INCOMPLETE_NAME = re.compile(r"(?:^|_)(?:미완성|incomplete)(?:_|\.|$)", re.IGNORECASE)

SOURCE_EXTENSIONS = {
    ".c",
    ".cc",
    ".cpp",
    ".cs",
    ".dart",
    ".ex",
    ".go",
    ".java",
    ".js",
    ".kt",
    ".php",
    ".py",
    ".rb",
    ".rkt",
    ".rs",
    ".scala",
    ".sql",
    ".swift",
    ".ts",
}

SITE_TIERS: dict[str, tuple[str, tuple[str, ...]]] = {
    "BOJ": ("Tier", ("Bronze", "Silver", "Gold", "Platinum")),
    "Programmers": ("Level", ("Lv0", "Lv1", "Lv2", "Lv3", "Lv4", "Lv5")),
    "SWEA": ("Difficulty", ("D1", "D2", "D3", "D4", "D5", "D6")),
    "LeetCode": ("Difficulty", ("Easy", "Medium", "Hard")),
    # SQL solutions live in their own folders and get separate tables,
    # so they never change the algorithm counts above.
    "Programmers/SQL": ("Level", ("Lv1", "Lv2", "Lv3", "Lv4", "Lv5")),
    "LeetCode/SQL": ("Difficulty", ("Easy", "Medium", "Hard")),
}

SITE_TITLES: dict[str, str] = {
    "Programmers/SQL": "Programmers SQL",
    "LeetCode/SQL": "LeetCode SQL",
}

EXTRA_TOTAL_FOLDERS: dict[str, tuple[str, ...]] = {
    "SWEA": ("Unclassified",),
}


@dataclass
class SiteStats:
    by_tier: dict[str, set[str]]
    extra_problem_ids: set[str]
    source_files: int
    duplicate_files: int
    unidentified: list[Path]

    @property
    def problem_ids(self) -> set[str]:
        tier_ids = set().union(*self.by_tier.values()) if self.by_tier else set()
        return tier_ids | self.extra_problem_ids


def is_incomplete(path: Path, tier_root: Path) -> bool:
    relative = path.relative_to(tier_root)
    return any(part.casefold() == "incomplete" for part in relative.parts[:-1]) or bool(
        INCOMPLETE_NAME.search(path.name)
    )


def staged_paths() -> list[Path]:
    result = subprocess.run(
        ["git", "ls-files", "--cached", "-z"],
        cwd=REPOSITORY_ROOT,
        stdout=subprocess.PIPE,
        check=True,
    )
    return [
        REPOSITORY_ROOT / os.fsdecode(name)
        for name in result.stdout.split(b"\0")
        if name
    ]


def collect_site(
    site: str, tiers: tuple[str, ...], indexed_paths: list[Path] | None = None
) -> SiteStats:
    site_root = REPOSITORY_ROOT / site
    by_tier = {tier: set() for tier in tiers}
    extra_problem_ids: set[str] = set()
    identified_files = 0
    unidentified: list[Path] = []

    folders = [(tier, False) for tier in tiers]
    folders.extend((folder, True) for folder in EXTRA_TOTAL_FOLDERS.get(site, ()))
    for folder, is_extra in folders:
        tier_root = site_root / folder
        if indexed_paths is None and not tier_root.is_dir():
            continue
        paths = (
            tier_root.rglob("*")
            if indexed_paths is None
            else (path for path in indexed_paths if path.is_relative_to(tier_root))
        )
        for path in sorted(paths):
            if indexed_paths is None and not path.is_file():
                continue
            if path.suffix.casefold() not in SOURCE_EXTENSIONS:
                continue
            if is_incomplete(path, tier_root):
                continue
            match = PROBLEM_ID.match(path.name)
            if match is None:
                unidentified.append(path.relative_to(REPOSITORY_ROOT))
                continue
            identified_files += 1
            if is_extra:
                extra_problem_ids.add(match.group(1))
            else:
                by_tier[folder].add(match.group(1))

    tier_ids = set().union(*by_tier.values()) if by_tier else set()
    unique_ids = tier_ids | extra_problem_ids
    return SiteStats(
        by_tier=by_tier,
        extra_problem_ids=extra_problem_ids,
        source_files=identified_files,
        duplicate_files=identified_files - len(unique_ids),
        unidentified=unidentified,
    )


def render_table(site: str, label: str, tiers: tuple[str, ...], stats: SiteStats) -> str:
    lines = [
        f"### {SITE_TITLES.get(site, site)}",
        "",
        f"| {label} | Problems |",
        "|---|---:|",
    ]
    lines.extend(f"| {tier} | {len(stats.by_tier[tier])} |" for tier in tiers)
    if stats.extra_problem_ids:
        folders = " / ".join(EXTRA_TOTAL_FOLDERS.get(site, ()))
        lines.append(
            f"| Other ({folders}) | {len(stats.extra_problem_ids)} |"
        )
    lines.append(f"| **Total** | **{len(stats.problem_ids)}** |")
    return "\n".join(lines)


CODETREE = "CodeTree"
CODETREE_AFTER = "LeetCode"
CODETREE_TRAIL = re.compile(r"^trail(\d+)$", re.IGNORECASE)


def collect_codetree(
    indexed_paths: list[Path] | None = None,
) -> tuple[tuple[str, ...], SiteStats]:
    """Count CodeTree solutions laid out as CodeTree/trailN/<problem>/<slug>.<ext>.

    CodeTree file names carry no problem number, so the solution file's slug
    (e.g. ``height-of-friends``) identifies the problem. A problem that appears
    in several trails is counted in each trail row but only once in the total.
    """
    site_root = REPOSITORY_ROOT / CODETREE
    if indexed_paths is None:
        paths = site_root.rglob("*") if site_root.is_dir() else iter(())
    else:
        paths = (path for path in indexed_paths if path.is_relative_to(site_root))
    by_tier: dict[str, set[str]] = {}
    identified_files = 0
    unidentified: list[Path] = []
    for path in sorted(paths):
        if indexed_paths is None and not path.is_file():
            continue
        if path.suffix.casefold() not in SOURCE_EXTENSIONS:
            continue
        parts = path.relative_to(site_root).parts
        if len(parts) != 3 or not CODETREE_TRAIL.match(parts[0]):
            unidentified.append(path.relative_to(REPOSITORY_ROOT))
            continue
        if is_incomplete(path, site_root / parts[0]):
            continue
        identified_files += 1
        by_tier.setdefault(parts[0], set()).add(path.stem.casefold())
    tiers = tuple(sorted(by_tier, key=lambda t: int(CODETREE_TRAIL.match(t).group(1))))
    unique_ids = set().union(*by_tier.values()) if by_tier else set()
    stats = SiteStats(
        by_tier=by_tier,
        extra_problem_ids=set(),
        source_files=identified_files,
        duplicate_files=identified_files - len(unique_ids),
        unidentified=unidentified,
    )
    return tiers, stats


def render_stats(
    all_stats: dict[str, SiteStats],
    site_tiers: dict[str, tuple[str, tuple[str, ...]]] = SITE_TIERS,
) -> str:
    sections = [STATS_START, "## Solved Problems"]
    for site, (label, tiers) in site_tiers.items():
        sections.extend(("", render_table(site, label, tiers, all_stats[site])))
    sections.extend(("", STATS_END))
    return "\n".join(sections)


def update_readme(block: str) -> None:
    original = README_PATH.read_text(encoding="utf-8")
    marker_pattern = re.compile(
        rf"{re.escape(STATS_START)}.*?{re.escape(STATS_END)}", re.DOTALL
    )
    if marker_pattern.search(original):
        updated = marker_pattern.sub(lambda _: block, original, count=1)
    else:
        insertion_point = original.find("\n## Languages")
        if insertion_point >= 0:
            updated = original[:insertion_point] + "\n\n" + block + original[insertion_point:]
        else:
            updated = original.rstrip() + "\n\n" + block + "\n"
    if updated != original:
        temporary = README_PATH.with_suffix(".md.tmp")
        temporary.write_text(updated, encoding="utf-8", newline="\n")
        temporary.replace(README_PATH)


def print_verification(
    all_stats: dict[str, SiteStats],
    site_tiers: dict[str, tuple[str, tuple[str, ...]]] = SITE_TIERS,
) -> None:
    all_unidentified: list[Path] = []
    for site, (_, tiers) in site_tiers.items():
        stats = all_stats[site]
        tier_summary = ", ".join(f"{tier}={len(stats.by_tier[tier])}" for tier in tiers)
        print(f"[{site}] Total={len(stats.problem_ids)} | {tier_summary}")
        print(f"[{site}] language-duplicate files excluded={stats.duplicate_files}")
        all_unidentified.extend(stats.unidentified)

    print(f"[VERIFY] unidentified files={len(all_unidentified)}")
    for path in sorted(all_unidentified):
        print(f"[UNIDENTIFIED] {path.as_posix()}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--staged", action="store_true", help="count files in the Git index"
    )
    args = parser.parse_args()
    indexed_paths = staged_paths() if args.staged else None
    codetree_tiers, codetree_stats = collect_codetree(indexed_paths)
    site_tiers: dict[str, tuple[str, tuple[str, ...]]] = {}
    all_stats: dict[str, SiteStats] = {}
    for site, (label, tiers) in SITE_TIERS.items():
        site_tiers[site] = (label, tiers)
        all_stats[site] = collect_site(site, tiers, indexed_paths)
        if site == CODETREE_AFTER:
            site_tiers[CODETREE] = ("Trail", codetree_tiers)
            all_stats[CODETREE] = codetree_stats
    update_readme(render_stats(all_stats, site_tiers))
    print_verification(all_stats, site_tiers)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
