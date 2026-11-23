"""One sync cycle: scan → known submissions → verify → poll due sites → publish."""

from __future__ import annotations

import logging
from contextlib import ExitStack
from dataclasses import dataclass, field
from datetime import datetime, timedelta
from typing import Any, Callable, Iterable

from algo import db as dbm
from algo import ingest, metadata, scanner
from algo.adapters.base import (AccessLimited, AdapterError, FetchContext, LoginRequired, SiteAdapter,
                                Unavailable)
from algo.config import Config
from algo.publisher import Publisher, RetryLater
from algo.repo import Repo

log = logging.getLogger("algo")

LOCAL_ADAPTER_SITES = {"leetcode": "LeetCode", "programmers": "Programmers", "swea": "SWEA"}
REMOTE_CHECK_MINUTES = 5
BACKOFF_MINUTES = {"network": [1, 5, 15, 30, 60], "login": [30], "limited": [360]}


@dataclass
class CycleReport:
    scanned: dict[str, int] = field(default_factory=dict)
    verified: int = 0
    fetched: dict[str, dict[str, int]] = field(default_factory=dict)
    site_errors: dict[str, str] = field(default_factory=dict)
    publish: str = ""


def fetch_context(database: dbm.Database, site: str, max_problems: int, delay: float) -> FetchContext:
    keys = database.known_submission_keys(site)
    known_problems = {r["problem_id"] for r in database.q(
        "SELECT DISTINCT problem_id FROM submissions WHERE site = ? AND verdict = 'ACCEPTED'", site)}
    known_problems |= {r["problem_id"] for r in database.q(
        "SELECT DISTINCT problem_id FROM files WHERE site = ? AND status = ?", site, dbm.SYNCED) if r["problem_id"]}
    pending = {r["problem_id"] for r in database.files_with_status(dbm.PENDING, dbm.DISCOVERED)
               if r["site"] == site and r["problem_id"]}
    latest = {r["problem_id"]: r["at"] for r in database.q(
        "SELECT problem_id, MAX(submitted_at) AS at FROM submissions WHERE site = ? GROUP BY problem_id", site)}
    return FetchContext(known_submission_keys=keys, known_problem_ids=known_problems,
                        pending_problem_ids=pending, latest_known_at=latest,
                        max_problems=max_problems, delay_seconds=delay)


def _due(database: dbm.Database, site: str, interval_minutes: float, now: datetime) -> bool:
    until = database.get(f"backoff_until:{site}")
    if until and datetime.fromisoformat(until) > now:
        return False
    last = database.get(f"last_poll:{site}")
    return not last or now - datetime.fromisoformat(last) >= timedelta(minutes=interval_minutes)


def _backoff(database: dbm.Database, site: str, kind: str, now: datetime) -> None:
    streak = int(database.get(f"fail_streak:{site}", "0") or 0) + 1
    steps = BACKOFF_MINUTES[kind]
    minutes = steps[min(streak, len(steps)) - 1]
    database.set(f"fail_streak:{site}", str(streak))
    database.set(f"backoff_until:{site}", (now + timedelta(minutes=minutes)).isoformat())


def poll_site(config: Config, database: dbm.Database, adapter: SiteAdapter, now: datetime) -> dict[str, int]:
    settings = config.site(adapter.config_name)
    context = fetch_context(database, adapter.site, settings.max_problems_per_run, settings.delay_seconds)
    counts: dict[str, int] = {}
    database.set(f"last_poll:{adapter.site}", now.isoformat())
    try:
        try:
            for fetched in adapter.fetch(context):
                outcome = ingest.ingest(config, database, fetched)
                counts[outcome] = counts.get(outcome, 0) + 1
        except OSError as error:  # network failures, timeouts
            raise AdapterError(f"네트워크 오류: {error}") from error
    except LoginRequired as error:
        _backoff(database, adapter.site, "login", now)
        database.set(f"site_status:{adapter.site}", f"로그인 필요: {error}")
        database.event("WARN", f"로그인이 필요합니다 (자동 재로그인하지 않음): {error}", site=adapter.site)
        raise
    except AccessLimited as error:
        _backoff(database, adapter.site, "limited", now)
        database.set(f"site_status:{adapter.site}", f"접근 제한 감지, 6시간 대기: {error}")
        database.event("WARN", f"접근 제한 감지 - 우회하지 않고 중단: {error}", site=adapter.site)
        raise
    except AdapterError as error:
        _backoff(database, adapter.site, "network", now)
        database.set(f"site_status:{adapter.site}", f"오류, 재시도 예정: {error}")
        database.event("ERROR", str(error), site=adapter.site)
        raise
    database.set(f"fail_streak:{adapter.site}", "0")
    database.set(f"backoff_until:{adapter.site}", None)
    database.set(f"site_status:{adapter.site}", f"정상 ({now:%H:%M})")
    database.set(f"last_success:{adapter.site}", now.isoformat())
    return counts


AdapterFactory = Callable[[ExitStack, list[str]], Iterable[SiteAdapter]]


def default_adapters(config: Config) -> AdapterFactory:
    """Open the Playwright CDP connection once per cycle, only for due sites."""

    def factory(stack: ExitStack, sites: list[str]) -> Iterable[SiteAdapter]:
        from algo import browser

        if not browser.cdp_reachable(config.cdp_url):
            raise Unavailable("Edge 전용 브라우저가 실행 중이 아닙니다 (algo browser)")
        try:
            from playwright.sync_api import sync_playwright
        except ModuleNotFoundError as error:
            raise Unavailable("playwright 패키지가 없습니다: .venv에 tools/requirements.txt를 설치하세요") from error

        playwright = stack.enter_context(sync_playwright())
        for name in sites:
            settings = config.site(name)
            output = config.root / LOCAL_ADAPTER_SITES[name]
            if name == "leetcode":
                from algo.adapters.leetcode import LeetCodeAdapter

                yield LeetCodeAdapter.over_cdp(playwright, config.cdp_url, settings.delay_seconds, output,
                                               config.state_dir / "leetcode_cdp_state.json")
            elif name == "programmers":
                from algo.adapters.programmers import ProgrammersAdapter

                yield ProgrammersAdapter.over_cdp(playwright, config.cdp_url, settings.delay_seconds, output)
            elif name == "swea":
                from algo.adapters.swea import SweaAdapter

                yield SweaAdapter.over_cdp(playwright, config.cdp_url, settings.delay_seconds, output)

    return factory


def due_sites(config: Config, database: dbm.Database, now: datetime, only: str | None = None,
              force: bool = False) -> list[str]:
    result = []
    for name, site in LOCAL_ADAPTER_SITES.items():
        settings = config.site(name)
        if settings.mode != "local":
            continue
        if only and only.casefold() not in {name, site.casefold()}:
            continue
        if force or _due(database, site, settings.interval_minutes, now):
            result.append(name)
    return result


def cycle(config: Config, database: dbm.Database, repo: Repo, *, now: datetime | None = None,
          adapters: AdapterFactory | None = None, only_site: str | None = None, force_poll: bool = False,
          publish: bool = True, publish_mode: str | None = None) -> CycleReport:
    now = now or datetime.now().astimezone()
    report = CycleReport()
    publisher = Publisher(config, database, repo)

    last_remote = database.get("last_remote_check")
    remote_due = not last_remote or now - datetime.fromisoformat(last_remote) >= timedelta(minutes=REMOTE_CHECK_MINUTES)
    if publish and config.auto_fast_forward and remote_due:
        database.set("last_remote_check", now.isoformat())
        try:
            publisher.integrate_remote()  # brings in commits made by the cloud workflows
            database.set("last_remote_sync", f"최신 ({now:%H:%M})")
        except RetryLater as error:
            database.set("last_remote_sync", f"보류: {error}")

    report.scanned = scanner.scan(config, database, repo)
    scanner.scan_external(config, database)
    ingest.ingest_metadata(config, database)
    report.verified += ingest.verify_pending(config, database)

    sites = due_sites(config, database, now, only_site, force_poll)
    if sites:
        factory = adapters or default_adapters(config)
        with ExitStack() as stack:
            try:
                for adapter in factory(stack, sites):
                    stack.callback(adapter.close)
                    try:
                        report.fetched[adapter.site] = poll_site(config, database, adapter, now)
                    except AdapterError as error:
                        report.site_errors[adapter.site] = str(error)
            except Unavailable as error:
                for name in sites:
                    site = LOCAL_ADAPTER_SITES[name]
                    report.site_errors[site] = str(error)
                    database.set(f"site_status:{site}", f"대기: {error}")
                    database.set(f"last_poll:{site}", now.isoformat())  # retry at the next interval
        report.scanned.update(scanner.scan(config, database, repo))
        report.verified += ingest.verify_pending(config, database)

    if publish:
        try:
            result = publisher.run(publish_mode)
            report.publish = (f"commit {result.committed[:10]}" if result.committed else "") + (
                " pushed" if result.pushed else "")
            database.set("last_publish_error", None)
        except RetryLater as error:
            report.publish = f"보류: {error}"
            database.set("last_publish_error", str(error))
            for row in database.files_with_status(dbm.ACCEPTED):
                database.upsert_file(row["path"], attempts=int(row["attempts"]) + 1, reason=f"업로드 보류: {error}")
            database.event("WARN", f"업로드 보류: {error}")
    return report


def known_metadata_sites() -> tuple[str, ...]:
    return metadata.METADATA_SITES
