"""LeetCode adapter. Reuses scripts/leetcode_backup.py (queries, login checks,
403/429/CAPTCHA stop rules) with one of two transports:

* ``cdp``  – the dedicated, signed-in Edge profile (local agent)
* ``http`` – session cookie from environment variables (GitHub Actions)

Both read only the signed-in user's own accepted submissions."""

from __future__ import annotations

import json
import os
from typing import Any, Iterator

from algo.adapters.base import (AccessLimited, AdapterError, FetchContext, LoginRequired, SiteAdapter,
                                Support, Unavailable, runner_args, translate_errors)
from algo.adapters.scripts import load_script
from algo.ingest import FetchedSubmission

SESSION_ENV = "LEETCODE_SESSION"
CSRF_ENV = "LEETCODE_CSRFTOKEN"


def make_http_runner(module: Any, output: Any, delay: float, session: str, csrf: str) -> Any:
    import httpx

    class HttpRunner(module.BackupRunner):  # type: ignore[misc, name-defined]
        """Same runner, but GraphQL goes over HTTPS with the user's own cookie."""

        def __init__(self) -> None:
            self.args = runner_args("", delay, output)
            self.output = output
            self.state = None
            self.client = httpx.Client(
                timeout=30,
                headers={
                    "Content-Type": "application/json",
                    "Referer": "https://leetcode.com/",
                    "Origin": "https://leetcode.com",
                    "x-csrftoken": csrf,
                    "User-Agent": "algo-sync (personal backup of own submissions)",
                },
                cookies={"LEETCODE_SESSION": session, "csrftoken": csrf},
            )

        def navigate(self, url: str) -> None:  # no page to load over plain HTTP
            return None

        def graphql(self, query: str, variables: dict[str, Any], operation: str) -> dict[str, Any]:
            self.wait()
            response = self.client.post(module.GRAPHQL_URL, json={
                "query": query, "variables": variables, "operationName": operation})
            text = response.text
            if response.status_code in {403, 429}:
                raise module.AccessLimitedError(f"HTTP {response.status_code}: {module.GRAPHQL_URL}")
            lowered = text[:5000].casefold()
            if "captcha" in lowered or "too many requests" in lowered or "access denied" in lowered:
                raise module.AccessLimitedError("LeetCode access restriction detected")
            if response.status_code >= 400:
                raise module.BackupError(f"GraphQL HTTP {response.status_code}")
            payload = json.loads(text)
            if payload.get("errors"):
                raise module.BackupError(f"GraphQL {operation} errors: {payload['errors']}")
            return payload.get("data") or {}

    return HttpRunner()


class LeetCodeAdapter(SiteAdapter):
    site = "LeetCode"
    config_name = "leetcode"
    capabilities = {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNOFFICIAL,
        "verdict": Support.UNOFFICIAL,
        "source_code": Support.UNOFFICIAL,
        "auth_required": Support.SUPPORTED,
    }
    notes = "LeetCode 웹이 쓰는 GraphQL을 본인 로그인 세션으로 조회 (공식 공개 API 아님)"

    def __init__(self, runner: Any = None, module: Any = None) -> None:
        self.runner = runner
        self.module = module

    @classmethod
    def over_cdp(cls, playwright: Any, cdp_url: str, delay: float, output: Any, state_path: Any) -> "LeetCodeAdapter":
        module = load_script("leetcode_backup")
        state = module.StateStore(state_path)  # keep the script's own state file untouched
        try:
            runner = module.BackupRunner(runner_args(cdp_url, delay, output), playwright, state)
        except Exception as error:  # noqa: BLE001
            raise Unavailable(f"Edge 디버그 브라우저에 연결하지 못했습니다: {error}") from error
        return cls(runner, module)

    @classmethod
    def over_http(cls, delay: float, output: Any) -> "LeetCodeAdapter":
        session = os.environ.get(SESSION_ENV, "")
        csrf = os.environ.get(CSRF_ENV, "")
        if not session or not csrf:
            raise Unavailable(f"{SESSION_ENV}/{CSRF_ENV} 환경 변수가 없습니다")
        module = load_script("leetcode_backup")
        return cls(make_http_runner(module, output, delay, session, csrf), module)

    def fetch(self, context: FetchContext) -> Iterator[FetchedSubmission]:
        module, runner = self.module, self.runner
        try:
            runner.verify_login()
            questions = runner.collect_questions()
        except Exception as error:  # noqa: BLE001
            raise translate_errors(module, error) from error

        def needs_check(question: dict[str, Any]) -> bool:
            problem_id = str(question.get("frontendId") or "")
            if problem_id in context.pending_problem_ids or problem_id not in context.known_problem_ids:
                return True
            latest_known = module.timestamp_value(context.latest_known_at.get(problem_id, ""))
            remote = module.timestamp_value(question.get("lastSubmittedAt"))
            return remote > latest_known

        checked = 0
        for question in questions:
            if checked >= context.max_problems:
                break
            if not needs_check(question):
                continue
            checked += 1
            problem_id = str(question.get("frontendId") or "")
            slug = str(question.get("titleSlug") or "")
            title = str(question.get("title") or question.get("translatedTitle") or slug)
            difficulty = module.difficulty_folder(str(question.get("difficulty") or ""))
            try:
                latest = runner.latest_accepted(slug)
                for item in latest.values():
                    key = str(item["id"])
                    if key in context.known_submission_keys:
                        continue
                    details = runner.submission_details(int(item["id"]))
                    yield FetchedSubmission(
                        site=self.site, submission_key=key, problem_id=problem_id, title=title,
                        tier=difficulty,
                        language=str(item["language"]), extension=str(item["extension"]),
                        code=str(details["code"]), submitted_at=module.submitted_at(details.get("timestamp")),
                        extra={"slug": slug})
            except (LoginRequired, AccessLimited):
                raise
            except Exception as error:  # noqa: BLE001
                mapped = translate_errors(module, error)
                if isinstance(mapped, (LoginRequired, AccessLimited)):
                    raise mapped from error
                raise AdapterError(f"{problem_id} {title}: {mapped}") from error

    def close(self) -> None:
        client = getattr(self.runner, "client", None)
        if client is not None:
            client.close()
