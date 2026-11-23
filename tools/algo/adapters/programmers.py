"""Programmers adapter. Reuses scripts/programmers_backup.py through the
dedicated signed-in Edge profile (CDP). Programmers has no public API; the
requests are the ones the signed-in web page itself makes."""

from __future__ import annotations

from typing import Any, Iterator

from algo.adapters.base import (AccessLimited, AdapterError, FetchContext, LoginRequired, SiteAdapter,
                                Support, Unavailable, runner_args, translate_errors)
from algo.adapters.scripts import load_script
from algo.ingest import FetchedSubmission


class ProgrammersAdapter(SiteAdapter):
    site = "Programmers"
    config_name = "programmers"
    capabilities = {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNOFFICIAL,
        "verdict": Support.UNOFFICIAL,
        "source_code": Support.UNOFFICIAL,
        "auth_required": Support.SUPPORTED,
    }
    notes = ("로그인한 웹 페이지가 쓰는 내부 요청을 본인 세션으로 조회. 이미 저장된 문제를 "
             "다른 언어로 새로 푼 경우는 그 문제의 로컬 파일이 생겼을 때만 확인합니다")

    def __init__(self, runner: Any = None, module: Any = None) -> None:
        self.runner = runner
        self.module = module

    @classmethod
    def over_cdp(cls, playwright: Any, cdp_url: str, delay: float, output: Any) -> "ProgrammersAdapter":
        module = load_script("programmers_backup")
        try:
            runner = module.BackupRunner(runner_args(cdp_url, delay, output), playwright)
        except Exception as error:  # noqa: BLE001
            raise Unavailable(f"Edge 디버그 브라우저에 연결하지 못했습니다: {error}") from error
        return cls(runner, module)

    def fetch(self, context: FetchContext) -> Iterator[FetchedSubmission]:
        module, runner = self.module, self.runner
        try:
            runner.verify_login()
            problems = runner.load_solved_problems(None)
        except Exception as error:  # noqa: BLE001
            raise translate_errors(module, error) from error

        selected = [p for p in problems
                    if str(p.get("id")) in context.pending_problem_ids
                    or str(p.get("id")) not in context.known_problem_ids]
        for problem in selected[: context.max_problems]:
            problem_id = str(problem.get("id") or "")
            title = str(problem.get("title") or "Untitled")
            level = problem.get("level")
            tier = f"Lv{level}" if isinstance(level, int) and 0 <= level <= 5 else "Unclassified"
            try:
                runner.wait()
                runner.navigate(module.PROBLEM_URL.format(problem_id=problem_id))
                runner.wait()
                latest = runner.latest_accepted_by_language(runner.load_submissions(problem_id))
            except Exception as error:  # noqa: BLE001
                mapped = translate_errors(module, error)
                if isinstance(mapped, (LoginRequired, AccessLimited)):
                    raise mapped from error
                raise AdapterError(f"{problem_id} {title}: {mapped}") from error
            for language, extension, submission in latest.values():
                key = str(submission.get("id"))
                if key in context.known_submission_keys:
                    continue
                yield FetchedSubmission(
                    site=self.site, submission_key=key, problem_id=problem_id, title=title, tier=tier,
                    language=language, extension=extension, code=str(submission["code"]),
                    submitted_at=str(submission.get("createdAt") or ""),
                    extra={"sourceLanguage": submission.get("language"), "score": submission.get("score"),
                           "perfectScore": submission.get("perfectScore")})
