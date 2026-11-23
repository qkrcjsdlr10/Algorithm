"""Site adapters and their verified support levels."""

from __future__ import annotations

from algo.adapters.base import Support

# Capability table shown by ``algo status`` and the docs. Only what was
# confirmed in this repository's scripts is marked UNOFFICIAL; nothing here is
# an official public API.
SITE_SUPPORT: dict[str, dict[str, Support | str]] = {
    "LeetCode": {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNOFFICIAL,
        "verdict": Support.UNOFFICIAL,
        "source_code": Support.UNOFFICIAL,
        "automation": "자동 (GitHub Actions + 세션 쿠키, 또는 로컬 Edge)",
    },
    "Programmers": {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNOFFICIAL,
        "verdict": Support.UNOFFICIAL,
        "source_code": Support.UNOFFICIAL,
        "automation": "자동 (로컬 에이전트 + 로그인한 Edge 전용 프로필)",
    },
    "SWEA": {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNOFFICIAL,
        "verdict": Support.UNOFFICIAL,
        "source_code": Support.UNOFFICIAL,
        "automation": "자동 (로컬 에이전트, 느린 주기)",
    },
    "BOJ": {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNKNOWN,
        "verdict": Support.UNKNOWN,
        "source_code": Support.UNSUPPORTED,
        "automation": "수동 승인 (algo approve). 자동 조회는 이용 조건 확인 전까지 비활성",
    },
    "CodeTree": {
        "official_api": Support.UNKNOWN,
        "submission_list": Support.UNKNOWN,
        "verdict": Support.UNKNOWN,
        "source_code": Support.SUPPORTED,
        "automation": "자동 (CodeTree의 GitHub 연동 → sync-codetree.yml). 로컬 에이전트는 관여하지 않음",
    },
}
