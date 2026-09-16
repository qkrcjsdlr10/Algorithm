#!/usr/bin/env python3
"""Update the unique solved-problem counts in the repository README."""

from __future__ import annotations

import re
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
}


@dataclass
class SiteStats:
    by_tier: dict[str, set[str]]
    source_files: int
    duplicate_files: int
    unidentified: list[Path]

    @property
    def problem_ids(self) -> set[str]:
        return set().union(*self.by_tier.values()) if self.by_tier else set()


def is_incomplete(path: Path, tier_root: Path) -> bool:
    relative = path.relative_to(tier_root)
    return any(part.casefold() == "incomplete" for part in relative.parts[:-1]) or bool(
        INCOMPLETE_NAME.search(path.name)
    )


def collect_site(site: str, tiers: tuple[str, ...]) -> SiteStats:
    site_root = REPOSITORY_ROOT / site
    by_tier = {tier: set() for tier in tiers}
    identified_files = 0
    unidentified: list[Path] = []

    for tier in tiers:
        tier_root = site_root / tier
        if not tier_root.is_dir():
            continue
        for path in sorted(tier_root.rglob("*")):
            if not path.is_file() or path.suffix.casefold() not in SOURCE_EXTENSIONS:
                continue
            if is_incomplete(path, tier_root):
                continue
            match = PROBLEM_ID.match(path.name)
            if match is None:
                unidentified.append(path.relative_to(REPOSITORY_ROOT))
                continue
            identified_files += 1
            by_tier[tier].add(match.group(1))

    unique_ids = set().union(*by_tier.values()) if by_tier else set()
    return SiteStats(
        by_tier=by_tier,
        source_files=identified_files,
        duplicate_files=identified_files - len(unique_ids),
        unidentified=unidentified,
    )


def render_table(site: str, label: str, tiers: tuple[str, ...], stats: SiteStats) -> str:
    lines = [
        f"### {site}",
        "",
        f"| {label} | Problems |",
        "|---|---:|",
    ]
    lines.extend(f"| {tier} | {len(stats.by_tier[tier])} |" for tier in tiers)
    lines.append(f"| **Total** | **{len(stats.problem_ids)}** |")
    return "\n".join(lines)


def render_stats(all_stats: dict[str, SiteStats]) -> str:
    sections = [STATS_START, "## Solved Problems"]
    for site, (label, tiers) in SITE_TIERS.items():
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


def print_verification(all_stats: dict[str, SiteStats]) -> None:
    all_unidentified: list[Path] = []
    for site, (_, tiers) in SITE_TIERS.items():
        stats = all_stats[site]
        tier_summary = ", ".join(f"{tier}={len(stats.by_tier[tier])}" for tier in tiers)
        print(f"[{site}] Total={len(stats.problem_ids)} | {tier_summary}")
        print(f"[{site}] language-duplicate files excluded={stats.duplicate_files}")
        all_unidentified.extend(stats.unidentified)

    print(f"[VERIFY] unidentified files={len(all_unidentified)}")
    for path in sorted(all_unidentified):
        print(f"[UNIDENTIFIED] {path.as_posix()}")


def main() -> int:
    all_stats = {
        site: collect_site(site, tiers)
        for site, (_, tiers) in SITE_TIERS.items()
    }
    update_readme(render_stats(all_stats))
    print_verification(all_stats)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
