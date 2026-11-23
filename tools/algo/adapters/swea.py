"""SWEA adapter. Reuses scripts/swea_backup.py through the dedicated signed-in
Edge profile. Code is read only when SWEA reports viewing one's own code as
free; otherwise the script stops for that submission (never bypassed)."""

from __future__ import annotations

from typing import Any, Iterator

from algo.adapters.base import (AccessLimited, AdapterError, FetchContext, LoginRequired, SiteAdapter,
                                Support, Unavailable, runner_args, translate_errors)
from algo.adapters.scripts import load_script
from algo.ingest import FetchedSubmission


class SweaAdapter(SiteAdapter):
    site = "SWEA"
    config_name = "swea"
    capabilities = {
        "official_api": Support.UNSUPPORTED,
        "submission_list": Support.UNOFFICIAL,
        "verdict": Support.UNOFFICIAL,
        "source_code": Support.UNOFFICIAL,
        "auth_required": Support.SUPPORTED,
    }
    notes = "내 코드 목록 화면을 순서대로 읽음 (페이지 수만큼 이동). 제출 ID 대신 contestHistoryId 사용"

    def __init__(self, runner: Any = None, module: Any = None) -> None:
        self.runner = runner
        self.module = module

    @classmethod
    def over_cdp(cls, playwright: Any, cdp_url: str, delay: float, output: Any) -> "SweaAdapter":
        module = load_script("swea_backup")
        try:
            runner = module.BackupRunner(runner_args(cdp_url, delay, output), playwright)
        except Exception as error:  # noqa: BLE001
            raise Unavailable(f"Edge 디버그 브라우저에 연결하지 못했습니다: {error}") from error
        return cls(runner, module)

    def fetch(self, context: FetchContext) -> Iterator[FetchedSubmission]:
        module, runner = self.module, self.runner
        try:
            cards = runner.collect_cards(False, None)  # includes verify_login()
        except Exception as error:  # noqa: BLE001
            raise translate_errors(module, error) from error

        seen: set[str] = set()
        selected = []
        for card in cards:
            if card["id"] in seen:
                continue
            seen.add(card["id"])
            if card["id"] in context.pending_problem_ids or card["id"] not in context.known_problem_ids:
                selected.append(card)
        for card in selected[: context.max_problems]:
            try:
                latest = runner.latest_pass_submissions(card["contestProbId"])
                for submission in latest.values():
                    key = str(submission["contestHistoryId"])
                    if key in context.known_submission_keys:
                        continue
                    code = runner.fetch_code(card["contestProbId"], key)
                    yield FetchedSubmission(
                        site=self.site, submission_key=key, problem_id=card["id"], title=card["title"],
                        tier=card["level"], language=submission["language"], extension=submission["extension"],
                        code=code, submitted_at=submission["submittedAt"],
                        extra={"contestProbId": card["contestProbId"]})
            except (LoginRequired, AccessLimited):
                raise
            except Exception as error:  # noqa: BLE001
                mapped = translate_errors(module, error)
                if isinstance(mapped, (LoginRequired, AccessLimited)):
                    raise mapped from error
                raise AdapterError(f"{card['id']} {card['title']}: {mapped}") from error
