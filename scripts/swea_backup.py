#!/usr/bin/env python3
"""Back up the signed-in user's accepted SWEA submissions via the site UI."""

from __future__ import annotations

import argparse
import hashlib
import html
import json
import re
import sys
import time
from pathlib import Path
from typing import Any

from playwright.sync_api import Browser, Page, Playwright, sync_playwright


if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")
    sys.stderr.reconfigure(encoding="utf-8")


START_URL = (
    "https://swexpertacademy.com/main/userpage/code/userCode.do"
    "?userId=AZLcLMHaFlcDFAQN"
)
USER_ID = "AZLcLMHaFlcDFAQN"
USER_NAME = "박천익_1647661"
DETAIL_URL = "https://swexpertacademy.com/main/code/problem/problemSubmitDetail.do"
CHECK_URL = "https://swexpertacademy.com/main/contestHistory/checkIsFirstOpenCode.do"
HISTORY_URL = "https://swexpertacademy.com/main/code/problem/problemSubmitHistory.do"
ACCESS_MARKERS = (
    "captcha",
    "캡차",
    "로봇이 아닙니다",
    "too many requests",
    "access denied",
)
INVALID_FILENAME_CHARS = re.compile(r'[\\/:*?"<>|\x00-\x1f]')
WHITESPACE = re.compile(r"\s+")
UNDERSCORES = re.compile(r"_+")
CODE_VIEW_ID = re.compile(r"codeview\('([^']+)'\)")

LANGUAGES: dict[str, tuple[str, str]] = {
    "c": ("C", ".c"),
    "c++": ("CPP", ".cpp"),
    "cpp": ("CPP", ".cpp"),
    "java": ("Java", ".java"),
    "python": ("Python", ".py"),
    "python3": ("Python", ".py"),
    "javascript": ("JavaScript", ".js"),
    "typescript": ("TypeScript", ".ts"),
    "kotlin": ("Kotlin", ".kt"),
    "swift": ("Swift", ".swift"),
    "go": ("Go", ".go"),
    "c#": ("CSharp", ".cs"),
    "csharp": ("CSharp", ".cs"),
    "ruby": ("Ruby", ".rb"),
    "scala": ("Scala", ".scala"),
}
EXTENSION_LANGUAGES = {extension: language for language, extension in LANGUAGES.values()}


class BackupError(RuntimeError):
    pass


class AuthenticationError(BackupError):
    pass


class AccessLimitedError(BackupError):
    pass


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Back up accepted SWEA submissions")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--inspect", action="store_true", help="inspect without saving")
    mode.add_argument("--test", action="store_true", help="save at most 3 new problems")
    mode.add_argument("--all", action="store_true", help="check every submitted problem")
    mode.add_argument("--limit", type=int, metavar="N", help="save at most N new problems")
    parser.add_argument("--cdp-url", default="http://127.0.0.1:9222")
    parser.add_argument("--delay", type=float, default=1.0)
    parser.add_argument(
        "--output",
        type=Path,
        default=Path(__file__).resolve().parents[1] / "SWEA",
    )
    args = parser.parse_args()
    if args.limit is not None and args.limit < 1:
        parser.error("--limit must be at least 1")
    if args.delay < 0.5:
        parser.error("--delay must be at least 0.5 seconds")
    return args


def sanitize_component(value: str) -> str:
    value = INVALID_FILENAME_CHARS.sub("_", value)
    value = WHITESPACE.sub("_", value.strip())
    value = UNDERSCORES.sub("_", value)
    return value.strip(" ._") or "Untitled"


def clean_title(value: str) -> str:
    value = re.sub(r"^(?:\[[^\]]+\]\s*)+", "", value).strip()
    return value or "Untitled"


def normalize_language(value: str) -> tuple[str, str]:
    key = re.sub(r"[\s._-]+", "", value).casefold()
    if key in LANGUAGES:
        return LANGUAGES[key]
    return sanitize_component(value or "Unknown"), ".txt"


def normalized_bytes(value: bytes) -> bytes:
    return value.replace(b"\r\n", b"\n").replace(b"\r", b"\n")


class BackupRunner:
    def __init__(self, args: argparse.Namespace, playwright: Playwright) -> None:
        self.args = args
        self.output = args.output.resolve()
        self.browser: Browser = playwright.chromium.connect_over_cdp(args.cdp_url)
        if not self.browser.contexts:
            raise BackupError("CDP browser has no available context")
        self.context = self.browser.contexts[0]
        self.page: Page = next(
            (page for page in self.context.pages if "swexpertacademy.com" in page.url),
            self.context.new_page(),
        )

    def wait(self) -> None:
        time.sleep(self.args.delay)

    def check_response(self, status: int, body: str, url: str) -> None:
        if status in {403, 429}:
            raise AccessLimitedError(f"HTTP {status}: {url}")
        lowered = body[:5000].casefold()
        if any(marker in lowered for marker in ACCESS_MARKERS):
            raise AccessLimitedError(f"access restriction detected: {url}")
        if status >= 400:
            raise BackupError(f"HTTP {status}: {url}")

    def navigate(self, url: str) -> None:
        response = self.page.goto(url, wait_until="domcontentloaded", timeout=30_000)
        self.page.wait_for_timeout(600)
        body = self.page.locator("body").inner_text(timeout=10_000)
        self.check_response(response.status if response else 200, body, url)

    def verify_login(self) -> None:
        self.navigate(START_URL)
        body = self.page.locator("body").inner_text()
        logout = self.page.locator("a[onclick*='logout']").count()
        if USER_NAME not in body or logout == 0:
            raise AuthenticationError("Log in to SWEA in the dedicated Edge profile first")

    def load_metadata(self) -> dict[str, dict[str, Any]]:
        path = self.output / "metadata.json"
        if not path.exists():
            return {}
        raw = json.loads(path.read_text(encoding="utf-8"))
        if not isinstance(raw, list):
            raise BackupError("SWEA metadata.json must contain an array")
        return {
            f"{item.get('id')}|{str(item.get('language')).casefold()}": item
            for item in raw
            if isinstance(item, dict) and item.get("id") and item.get("language")
        }

    def existing_files(self, metadata: dict[str, dict[str, Any]]) -> dict[str, Path]:
        result: dict[str, Path] = {}
        for key, item in metadata.items():
            path = self.output / str(item.get("file") or "")
            if path.is_file():
                result[key] = path
        for path in self.output.rglob("*"):
            if not path.is_file() or path.suffix.casefold() not in EXTENSION_LANGUAGES:
                continue
            match = re.match(r"^(\d+)_", path.name)
            if not match:
                continue
            language = EXTENSION_LANGUAGES[path.suffix.casefold()]
            result.setdefault(f"{match.group(1)}|{language.casefold()}", path)
        return result

    def extract_cards(self, page_index: int) -> list[dict[str, str]]:
        cards = self.page.locator(".widget-box-sub").evaluate_all(
            """elements => elements.map(card => {
                const link = card.querySelector("a[onclick*='fn_move_prob']");
                return link ? {
                    title: (link.innerText || '').trim(),
                    onclick: link.getAttribute('onclick') || '',
                    text: (card.innerText || '').trim(),
                } : null;
            }).filter(Boolean)"""
        )
        result: list[dict[str, str]] = []
        for card in cards:
            id_match = re.search(r"(?:^|\n)(\d+)\.", card["text"])
            contest_match = re.search(r"fn_move_prob\('([^']+)'", card["onclick"])
            date_match = re.search(r"제출일\s+(\d{4}-\d{2}-\d{2}\s+\d{2}:\d{2})", card["text"])
            level_match = re.search(r"(?:^|\s)(D[1-8])(?:\s|$)", card["text"])
            if not id_match or not contest_match:
                continue
            level = level_match.group(1) if level_match else "Unclassified"
            if level not in {f"D{i}" for i in range(1, 7)}:
                level = "Unclassified"
            result.append(
                {
                    "id": id_match.group(1),
                    "title": clean_title(card["title"]),
                    "level": level,
                    "contestProbId": contest_match.group(1),
                    "listedAt": date_match.group(1) if date_match else "",
                    "pageIndex": str(page_index),
                }
            )
        return result

    def collect_cards(self, missing_ids_only: bool, limit: int | None) -> list[dict[str, str]]:
        metadata = self.load_metadata()
        existing_ids = {key.split("|", 1)[0] for key in self.existing_files(metadata)}
        self.verify_login()
        selected: list[dict[str, str]] = []
        page_index = 1
        while True:
            for card in self.extract_cards(page_index):
                if missing_ids_only and card["id"] in existing_ids:
                    continue
                selected.append(card)
                if limit is not None and len(selected) >= limit:
                    return selected
            next_index = page_index + 1
            link = self.page.locator(".pagination a.page-link", has_text=str(next_index))
            if link.count() == 0:
                return selected
            self.wait()
            with self.page.expect_navigation(wait_until="domcontentloaded", timeout=30_000):
                link.first.click()
            self.page.wait_for_timeout(500)
            page_index = next_index

    def open_card_history(self, card: dict[str, str]) -> None:
        self.navigate(START_URL)
        page_index = int(card["pageIndex"])
        if page_index > 1:
            link = self.page.locator(".pagination a.page-link", has_text=str(page_index))
            with self.page.expect_navigation(wait_until="domcontentloaded", timeout=30_000):
                link.first.click()
            self.page.wait_for_timeout(500)
        problem_link = self.page.locator(
            f"a[onclick*='{card['contestProbId']}']"
        ).first
        if problem_link.count() == 0:
            raise BackupError(f"problem link disappeared: {card['id']}")
        self.wait()
        with self.page.expect_navigation(wait_until="domcontentloaded", timeout=30_000):
            problem_link.click()
        self.page.wait_for_timeout(500)
        history = self.page.locator("a", has_text="제출이력").first
        if history.count() == 0:
            raise BackupError(f"submission history tab missing: {card['id']}")
        self.wait()
        with self.page.expect_navigation(wait_until="domcontentloaded", timeout=30_000):
            history.click()
        self.page.wait_for_timeout(500)
        checkbox = self.page.locator("#mySubmit")
        if checkbox.count() == 0:
            raise BackupError(f"My submissions filter missing: {card['id']}")
        self.wait()
        with self.page.expect_navigation(wait_until="domcontentloaded", timeout=30_000):
            checkbox.check()
        self.page.wait_for_timeout(500)

    def latest_pass_submissions(
        self, contest_prob_id: str
    ) -> dict[str, dict[str, str]]:
        latest: dict[str, dict[str, str]] = {}
        page_index = 1
        total_pages = 1
        while page_index <= total_pages:
            self.wait()
            result = self.page.evaluate(
                """async ({url, contestProbId, userId, pageIndex}) => {
                    const body = new URLSearchParams({
                        contestProbId, codeLangSet:'', sortType:'1', nickName:'',
                        isChecked:'checked', checkUserId:userId,
                        pageSize:'30', pageIndex:String(pageIndex),
                    }).toString();
                    const response = await fetch(url, {
                        method:'POST', credentials:'include',
                        headers:{'Content-Type':'application/x-www-form-urlencoded'},
                        body,
                    });
                    const text = await response.text();
                    const doc = new DOMParser().parseFromString(text, 'text/html');
                    const rows = [...doc.querySelectorAll('.problem_smt')].map(row => {
                        const info = [...row.querySelectorAll('.info li')];
                        const link = row.querySelector("a[href^='javascript:codeview']");
                        return {
                            text:(row.innerText || '').trim(),
                            owner:(row.querySelector('.club_name .name')?.textContent || '').trim(),
                            language:(info[0]?.querySelector('span')?.textContent || '').trim(),
                            result:(info[4]?.querySelector('span')?.textContent || '').trim(),
                            href:link?.getAttribute('href') || '',
                        };
                    });
                    const pages = [...doc.querySelectorAll('.pagination .page-link')]
                        .map(e => Number((e.textContent || '').trim()))
                        .filter(Number.isFinite);
                    return {status:response.status, text, rows, pages};
                }""",
                {
                    "url": HISTORY_URL,
                    "contestProbId": contest_prob_id,
                    "userId": USER_ID,
                    "pageIndex": page_index,
                },
            )
            self.check_response(int(result["status"]), str(result["text"]), HISTORY_URL)
            if USER_NAME not in result["text"]:
                raise AuthenticationError("SWEA My submissions response is not authenticated")
            if result["pages"]:
                total_pages = max(int(value) for value in result["pages"])
            for row in result["rows"]:
                if row["owner"] != USER_NAME or row["result"] != "Pass":
                    continue
                date_match = re.search(
                    r"제출일\s*:\s*(\d{4}-\d{2}-\d{2}\s+\d{2}:\d{2})",
                    row["text"],
                )
                id_match = CODE_VIEW_ID.search(row["href"])
                if not date_match or not id_match:
                    continue
                language, extension = normalize_language(row["language"])
                item = {
                    "language": language,
                    "extension": extension,
                    "submittedAt": date_match.group(1),
                    "result": "Accepted",
                    "siteResult": "Pass",
                    "contestHistoryId": id_match.group(1),
                }
                key = language.casefold()
                if key not in latest or item["submittedAt"] > latest[key]["submittedAt"]:
                    latest[key] = item
            page_index += 1
        return latest

    def fetch_code(self, contest_prob_id: str, history_id: str) -> str:
        self.wait()
        result = self.page.evaluate(
            """async ({checkUrl, detailUrl, contestProbId, contestHistoryId}) => {
                const encode = value => new URLSearchParams(value).toString();
                const check = await fetch(checkUrl, {
                    method: 'POST', credentials: 'include',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'},
                    body: encode({contestHistoryId}),
                });
                const checkText = await check.text();
                if (!check.ok) return {stage:'check', status:check.status, text:checkText};
                let parsed;
                try { parsed = JSON.parse(checkText); } catch (_) {
                    return {stage:'check', status:check.status, text:checkText};
                }
                if (parsed.message !== 'Success' || Number(parsed.data || 0) > 0) {
                    return {stage:'permission', status:check.status, text:checkText};
                }
                const detail = await fetch(detailUrl, {
                    method: 'POST', credentials: 'include',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                    body: encode({contestProbId, contestHistoryId}),
                });
                return {stage:'detail', status:detail.status, text:await detail.text()};
            }""",
            {
                "checkUrl": CHECK_URL,
                "detailUrl": DETAIL_URL,
                "contestProbId": contest_prob_id,
                "contestHistoryId": history_id,
            },
        )
        self.check_response(int(result["status"]), str(result["text"]), DETAIL_URL)
        if result["stage"] != "detail":
            raise BackupError(
                f"code view was not free/authorized for own submission: {history_id}"
            )
        match = re.search(
            r"<textarea[^>]*class=[\"'][^\"']*brush:[^\"']*[\"'][^>]*>([\s\S]*?)</textarea>",
            result["text"],
            re.IGNORECASE,
        )
        if not match:
            raise BackupError(f"source textarea missing: {history_id}")
        code = html.unescape(match.group(1))
        if code.startswith("\r\n"):
            code = code[2:]
        elif code.startswith("\n"):
            code = code[1:]
        return code

    def write_metadata(self, metadata: dict[str, dict[str, Any]]) -> None:
        entries = sorted(
            metadata.values(),
            key=lambda item: (
                item.get("level") == "Unclassified",
                item.get("level") or "",
                int(item.get("id") or 0),
                item.get("language") or "",
            ),
        )
        payload = json.dumps(entries, ensure_ascii=False, indent=2) + "\n"
        (self.output / "metadata.json").write_text(payload, encoding="utf-8", newline="\n")

    def process(
        self, cards: list[dict[str, str]], problem_limit: int | None = None
    ) -> None:
        self.output.mkdir(parents=True, exist_ok=True)
        metadata = self.load_metadata()
        existing = self.existing_files(metadata)
        changed = False
        completed_problems = 0
        for card in cards:
            if problem_limit is not None and completed_problems >= problem_limit:
                break
            try:
                latest = self.latest_pass_submissions(card["contestProbId"])
                if not latest:
                    print(f"[SKIP] {card['id']} {card['title']}: no accepted own submission")
                    continue
                for submission in latest.values():
                    language = submission["language"]
                    key = f"{card['id']}|{language.casefold()}"
                    code = self.fetch_code(card["contestProbId"], submission["contestHistoryId"])
                    code_bytes = code.encode("utf-8")
                    title = sanitize_component(card["title"])
                    filename = f"{card['id']}_{title}_{language}{submission['extension']}"
                    relative = f"{card['level']}/{filename}"
                    target = self.output / relative
                    action = "NEW"
                    existing_path = existing.get(key)
                    if existing_path is not None:
                        if normalized_bytes(existing_path.read_bytes()) == normalized_bytes(code_bytes):
                            action = "SKIP"
                        else:
                            action = "UPDATE"
                        relative = existing_path.relative_to(self.output).as_posix()
                    elif target.exists():
                        action = "SKIP" if normalized_bytes(target.read_bytes()) == normalized_bytes(code_bytes) else "UPDATE"
                    if action == "NEW":
                        target.parent.mkdir(parents=True, exist_ok=True)
                        target.write_bytes(code_bytes)
                        existing[key] = target
                        changed = True
                        metadata[key] = {
                            "id": card["id"],
                            "title": card["title"],
                            "level": card["level"],
                            "language": language,
                            "file": relative,
                            "submittedAt": submission["submittedAt"],
                            "result": submission["result"],
                            "siteResult": submission["siteResult"],
                            "contestProbId": card["contestProbId"],
                            "contestHistoryId": submission["contestHistoryId"],
                            "sha256": hashlib.sha256(code_bytes).hexdigest(),
                        }
                    print(
                        f"[{action}] {card['id']} | {card['title']} | {card['level']} | "
                        f"{language} | {submission['submittedAt']} | {relative}"
                    )
                completed_problems += 1
                print(f"[INFO] test progress {completed_problems}/{problem_limit or len(cards)}")
            except AccessLimitedError:
                raise
            except Exception as error:
                print(f"[SKIP] {card['id']} {card['title']}: {type(error).__name__}: {error}")
        if changed:
            self.write_metadata(metadata)

    def inspect(self) -> None:
        self.verify_login()
        cards = self.extract_cards(1)
        pages = self.page.locator(".pagination .page-item").all_inner_texts()
        print(f"[INSPECT] logged in as {USER_NAME}")
        print(f"[INSPECT] first-page problems: {len(cards)}")
        print(f"[INSPECT] pagination: {pages}")
        for card in cards[:3]:
            print(f"[INSPECT] {card}")


def main() -> int:
    args = parse_args()
    limit = 10 if args.test else args.limit
    try:
        with sync_playwright() as playwright:
            runner = BackupRunner(args, playwright)
            if args.inspect:
                runner.inspect()
            else:
                cards = runner.collect_cards(not args.all, limit)
                print(f"[INFO] selected {len(cards)} problem(s)")
                runner.process(cards, 3 if args.test else args.limit)
        return 0
    except AccessLimitedError as error:
        print(f"[STOP] access restriction detected; no bypass attempted: {error}", file=sys.stderr)
        return 3
    except AuthenticationError as error:
        print(f"[STOP] authentication required: {error}", file=sys.stderr)
        return 4
    except Exception as error:
        print(f"[FATAL] {type(error).__name__}: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
