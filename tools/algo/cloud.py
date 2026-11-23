"""``algo cloud-sync leetcode``: run inside GitHub Actions without the local PC.

Writes new accepted LeetCode files + metadata.json into the checkout; the
workflow then runs update_stats.py and commits. State is rebuilt from the
repository on every run (metadata.json is the durable record), so nothing
secret or stateful is stored besides the cookie in GitHub Secrets."""

from __future__ import annotations

import os
from datetime import datetime

from algo import db as dbm
from algo import engine, migrate
from algo.adapters.base import AccessLimited, AdapterError, LoginRequired, Unavailable
from algo.config import Config
from algo.repo import Repo


def _notice(kind: str, message: str) -> None:
    prefix = {"warning": "::warning::", "error": "::error::"}.get(kind, "") if os.environ.get("GITHUB_ACTIONS") else ""
    print(f"{prefix}{message}")


def sync_leetcode(config: Config) -> int:
    from algo.adapters.leetcode import LeetCodeAdapter

    database = dbm.Database(":memory:")
    repo = Repo(config.root, config.remote)
    migrate.baseline(config, database, repo)
    settings = config.site("leetcode")
    try:
        adapter = LeetCodeAdapter.over_http(settings.delay_seconds, config.root / "LeetCode")
    except Unavailable as error:
        _notice("warning", f"LeetCode 동기화를 건너뜁니다: {error}")
        return 0
    try:
        counts = engine.poll_site(config, database, adapter, datetime.now().astimezone())
    except LoginRequired as error:
        _notice("error", f"LeetCode 세션이 만료되었거나 계정이 다릅니다. Secrets의 쿠키를 새로 넣어 주세요: {error}")
        return 4
    except AccessLimited as error:
        _notice("error", f"LeetCode 접근 제한이 감지되어 중단했습니다 (우회하지 않음): {error}")
        return 3
    except AdapterError as error:
        _notice("error", f"LeetCode 동기화 실패: {error}")
        return 1
    finally:
        adapter.close()
    print(f"LeetCode 결과: {counts or '새 정답 제출 없음'}")
    for row in database.files_with_status(dbm.ACCEPTED):
        print(f"  + {row['path']}")
    for row in database.q("SELECT target_path, stored_path FROM candidates"):
        _notice("warning", f"기존 파일과 다른 정답 제출이 있어 덮어쓰지 않았습니다: {row['target_path']}")
    return 0
