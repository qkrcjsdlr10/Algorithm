#!/usr/bin/env python3
"""Back up the signed-in user's accepted Programmers submissions.

This script connects to an already-running Chromium/Edge instance over CDP.
It only uses pages and requests observed in the normal signed-in browser flow.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
import time
from datetime import datetime, timezone
from pathlib import Path
from typing import Any
from urllib.parse import quote

from playwright.sync_api import Browser, Page, Playwright, sync_playwright


SOLVED_PAGE_URL = (
    "https://school.programmers.co.kr/learn/challenges"
    "?order=acceptance_desc&page=1&statuses=solved"
)
SOLVED_API_URL = (
    "https://school.programmers.co.kr/api/v2/school/challenges/"
    "?perPage=20&statuses%5B%5D=solved&order=acceptance_desc&search=&page={page}"
)
PROBLEM_URL = "https://school.programmers.co.kr/learn/courses/30/lessons/{problem_id}"
SUBMISSIONS_API_URL = (
    "https://programmers.co.kr/api/v1/school/open-challenge/lessons/"
    "{problem_id}/submissions?page={page}&perPage=10"
)

INVALID_FILENAME_CHARS = re.compile(r'[\\/:*?"<>|\x00-\x1f]')
WHITESPACE = re.compile(r"\s+")
UNDERSCORES = re.compile(r"_+")
ACCESS_LIMIT_MARKERS = (
    "captcha",
    "캡차",
    "로봇이 아닙니다",
    "too many requests",
    "access denied",
)

LANGUAGES: dict[str, tuple[str, str]] = {
    "c": ("C", ".c"),
    "cpp": ("CPP", ".cpp"),
    "c++": ("CPP", ".cpp"),
    "csharp": ("CSharp", ".cs"),
    "c#": ("CSharp", ".cs"),
    "go": ("Go", ".go"),
    "java": ("Java", ".java"),
    "java17": ("Java", ".java"),
    "javascript": ("JavaScript", ".js"),
    "typescript": ("TypeScript", ".ts"),
    "kotlin": ("Kotlin", ".kt"),
    "python": ("Python", ".py"),
    "python3": ("Python", ".py"),
    "ruby": ("Ruby", ".rb"),
    "scala": ("Scala", ".scala"),
    "swift": ("Swift", ".swift"),
    "mysql": ("MySQL", ".sql"),
    "oracle": ("Oracle", ".sql"),
    "rust": ("Rust", ".rs"),
    "php": ("PHP", ".php"),
    "dart": ("Dart", ".dart"),
    "r": ("R", ".r"),
    "lua": ("Lua", ".lua"),
}


class BackupError(RuntimeError):
    """Base error for the backup process."""


class AuthenticationError(BackupError):
    """Raised when the browser is not signed in."""


class AccessLimitedError(BackupError):
    """Raised on 403, 429, CAPTCHA, or another access restriction."""


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Back up accepted submissions from the signed-in Programmers account."
    )
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--test", action="store_true", help="process at most 3 problems")
    mode.add_argument("--all", action="store_true", help="process all solved problems")
    mode.add_argument("--limit", type=int, metavar="N", help="process at most N problems")
    parser.add_argument(
        "--cdp-url",
        default="http://127.0.0.1:9222",
        help="remote debugging endpoint (default: %(default)s)",
    )
    parser.add_argument(
        "--delay",
        type=float,
        default=1.0,
        help="seconds between page/API requests (default: %(default)s)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path(__file__).resolve().parents[1] / "Programmers",
        help="backup directory",
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
    value = value.strip(" ._")
    return value or "Untitled"


def normalize_language(internal: str, display: str) -> tuple[str, str]:
    candidates = (internal, display)
    for candidate in candidates:
        key = re.sub(r"[\s._-]+", "", (candidate or "").lower())
        if key in LANGUAGES:
            return LANGUAGES[key]
        if key.startswith("java") and key[4:].isdigit():
            return "Java", ".java"
        if key.startswith("python") and key[6:].isdigit():
            return "Python", ".py"
    unknown = sanitize_component(display or internal or "Unknown")
    return unknown, ".txt"


def build_filename(
    problem_id: str, title: str, language: str, extension: str, max_length: int = 180
) -> str:
    safe_title = sanitize_component(title)
    suffix = f"_{sanitize_component(language)}{extension}"
    prefix = f"{problem_id}_"
    available = max(1, max_length - len(prefix) - len(suffix))
    safe_title = safe_title[:available].rstrip(" ._") or "Untitled"
    return f"{prefix}{safe_title}{suffix}"


def parse_timestamp(value: str | None) -> datetime:
    if not value:
        return datetime.min.replace(tzinfo=timezone.utc)
    try:
        return datetime.fromisoformat(value.replace("Z", "+00:00"))
    except ValueError:
        return datetime.min.replace(tzinfo=timezone.utc)


def is_accepted(submission: dict[str, Any]) -> bool:
    score = submission.get("score")
    perfect_score = submission.get("perfectScore")
    if isinstance(score, (int, float)) and isinstance(perfect_score, (int, float)):
        return perfect_score > 0 and score == perfect_score
    status = str(submission.get("status") or submission.get("result") or "").lower()
    return status in {"accepted", "correct", "passed", "success"}


def code_digest(code: str) -> str:
    return hashlib.sha256(code.encode("utf-8")).hexdigest()


class BackupRunner:
    def __init__(self, args: argparse.Namespace, playwright: Playwright) -> None:
        self.args = args
        self.output = args.output.resolve()
        self.output.mkdir(parents=True, exist_ok=True)
        self.log_file = self.output / "backup.log"
        self.failures: list[dict[str, str]] = []
        self.browser: Browser = playwright.chromium.connect_over_cdp(args.cdp_url)
        if not self.browser.contexts:
            raise BackupError("CDP browser has no available context")
        self.context = self.browser.contexts[0]
        self.page = self._select_page()

    def _select_page(self) -> Page:
        for page in self.context.pages:
            if "programmers.co.kr" in page.url:
                return page
        return self.context.new_page()

    def log(self, message: str) -> None:
        print(message, flush=True)
        timestamp = datetime.now().astimezone().isoformat(timespec="seconds")
        with self.log_file.open("a", encoding="utf-8", newline="\n") as stream:
            stream.write(f"{timestamp} {message}\n")

    def wait(self) -> None:
        time.sleep(self.args.delay)

    def check_page_for_limits(self) -> None:
        title = self.page.title().lower()
        text = self.page.locator("body").inner_text(timeout=10_000).lower()
        if any(marker in title or marker in text for marker in ACCESS_LIMIT_MARKERS):
            raise AccessLimitedError("CAPTCHA or access restriction detected in the page")

    def navigate(self, url: str) -> None:
        response = self.page.goto(url, wait_until="domcontentloaded", timeout=30_000)
        if response is not None:
            if response.status in {403, 429}:
                raise AccessLimitedError(f"HTTP {response.status}: {url}")
            if response.status == 401:
                raise AuthenticationError("Programmers login is required")
            if response.status >= 400:
                raise BackupError(f"HTTP {response.status}: {url}")
        self.page.wait_for_timeout(500)
        self.check_page_for_limits()

    def browser_fetch_json(self, url: str) -> dict[str, Any]:
        result = self.page.evaluate(
            """async (url) => {
                const response = await fetch(url, { credentials: "include" });
                return {
                    status: response.status,
                    contentType: response.headers.get("content-type") || "",
                    text: await response.text(),
                };
            }""",
            url,
        )
        status = int(result["status"])
        body_text = result["text"]
        if status in {403, 429}:
            raise AccessLimitedError(f"HTTP {status}: {url}")
        if status == 401:
            raise AuthenticationError("Programmers login session is no longer valid")
        if status >= 400:
            raise BackupError(f"HTTP {status}: {url}")
        lowered = body_text[:2000].lower()
        if any(marker in lowered for marker in ACCESS_LIMIT_MARKERS):
            raise AccessLimitedError(f"Access restriction response: {url}")
        try:
            data = json.loads(body_text)
        except json.JSONDecodeError as error:
            raise BackupError(f"Expected JSON response from {url}") from error
        if not isinstance(data, dict):
            raise BackupError(f"Unexpected response structure from {url}")
        return data

    def verify_login(self) -> None:
        self.navigate(SOLVED_PAGE_URL)
        # The signed-in menu is present in the DOM but hidden until the profile
        # popup opens, so body.inner_text() does not include its logout label.
        if self.page.locator("button", has_text="로그아웃").count() == 0:
            raise AuthenticationError("Log in to Programmers in the CDP browser first")

    def load_solved_problems(self, limit: int | None) -> list[dict[str, Any]]:
        problems: list[dict[str, Any]] = []
        seen: set[str] = set()
        page_number = 1
        total_pages = 1
        while page_number <= total_pages:
            data = self.browser_fetch_json(SOLVED_API_URL.format(page=page_number))
            total_pages = int(data.get("totalPages") or 1)
            for problem in data.get("result") or []:
                problem_id = str(problem.get("id") or "")
                if not problem_id or problem_id in seen or problem.get("status") != "solved":
                    continue
                seen.add(problem_id)
                problems.append(problem)
                if limit is not None and len(problems) >= limit:
                    return problems
            page_number += 1
            if page_number <= total_pages:
                self.wait()
        return problems

    def load_submissions(self, problem_id: str) -> list[dict[str, Any]]:
        submissions: list[dict[str, Any]] = []
        page_number = 1
        total_pages = 1
        while page_number <= total_pages:
            url = SUBMISSIONS_API_URL.format(problem_id=problem_id, page=page_number)
            data = self.browser_fetch_json(url)
            total_pages = int(data.get("totalPages") or 1)
            submissions.extend(data.get("submissions") or [])
            page_number += 1
            if page_number <= total_pages:
                self.wait()
        return submissions

    @staticmethod
    def latest_accepted_by_language(
        submissions: list[dict[str, Any]],
    ) -> dict[str, tuple[str, str, dict[str, Any]]]:
        latest: dict[str, tuple[str, str, dict[str, Any]]] = {}
        for submission in submissions:
            if not is_accepted(submission) or not isinstance(submission.get("code"), str):
                continue
            internal = str(submission.get("language") or "")
            display = str(submission.get("displayLanguage") or internal)
            label, extension = normalize_language(internal, display)
            key = label.casefold()
            current = latest.get(key)
            if current is None or parse_timestamp(submission.get("createdAt")) > parse_timestamp(
                current[2].get("createdAt")
            ):
                latest[key] = (label, extension, submission)
        return latest

    def load_metadata(self) -> dict[str, dict[str, Any]]:
        path = self.output / "metadata.json"
        if not path.exists():
            return {}
        try:
            raw = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as error:
            raise BackupError(f"Cannot read {path}: {error}") from error
        if not isinstance(raw, list):
            raise BackupError("metadata.json must contain a JSON array")
        return {
            f"{item.get('id')}|{str(item.get('language')).casefold()}": item
            for item in raw
            if isinstance(item, dict) and item.get("id") and item.get("language")
        }

    def save_solution(
        self,
        problem: dict[str, Any],
        language: str,
        extension: str,
        submission: dict[str, Any],
        metadata: dict[str, dict[str, Any]],
    ) -> None:
        problem_id = str(problem["id"])
        title = str(problem.get("title") or "Untitled")
        level_value = problem.get("level")
        if isinstance(level_value, int) and 0 <= level_value <= 5:
            level = f"Lv{level_value}"
        else:
            level = "Unclassified"
        directory = self.output / level
        directory.mkdir(parents=True, exist_ok=True)
        filename = build_filename(problem_id, title, language, extension)
        path = directory / filename
        relative = path.relative_to(self.output).as_posix()
        code = submission["code"]
        code_bytes = code.encode("utf-8")
        key = f"{problem_id}|{language.casefold()}"
        previous = metadata.get(key)

        if previous and parse_timestamp(previous.get("submittedAt")) > parse_timestamp(
            submission.get("createdAt")
        ):
            self.log(f"[SKIP] newer local metadata exists: {problem_id} {title} / {language}")
            return

        if path.exists():
            existing = path.read_bytes()
            if existing == code_bytes:
                action = "SKIP"
                self.log(f"[SKIP] identical code: {problem_id} {title} / {language}")
            else:
                action = "UPDATE"
                self.log(
                    f"[UPDATE] {problem_id} {title} / {language}\n"
                    f"         old={hashlib.sha256(existing).hexdigest()[:12]} "
                    f"new={code_digest(code)[:12]}"
                )
                path.write_bytes(code_bytes)
        else:
            action = "OK"
            path.write_bytes(code_bytes)
            self.log(f"[OK] {problem_id} {title} / {language}\n     -> {relative}")

        metadata[key] = {
            "id": problem_id,
            "title": title,
            "level": level,
            "language": language,
            "sourceLanguage": submission.get("language"),
            "file": relative,
            "submittedAt": submission.get("createdAt"),
            "accepted": True,
            "score": submission.get("score"),
            "perfectScore": submission.get("perfectScore"),
            "submissionId": submission.get("id"),
            "url": PROBLEM_URL.format(problem_id=problem_id),
            "sha256": code_digest(code),
            "lastAction": action,
        }

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
        (self.output / "metadata.json").write_bytes(payload.encode("utf-8"))
        self.write_readme(entries)

    def write_readme(self, entries: list[dict[str, Any]]) -> None:
        lines = [
            "# Programmers Solutions",
            "",
            "프로그래머스에서 통과한 개인 제출 코드의 백업 목록입니다.",
            "",
            "| 문제 | 난이도 | 언어 | 파일 |",
            "|---|---:|---|---|",
        ]
        for item in entries:
            title = str(item.get("title") or "").replace("|", "\\|")
            level = str(item.get("level") or "Unclassified")
            language = str(item.get("language") or "Unknown")
            relative = str(item.get("file") or "")
            link = quote(relative, safe="/._-()")
            lines.append(f"| {title} | {level} | {language} | [{relative}]({link}) |")
        lines.append("")
        (self.output / "README.md").write_bytes("\n".join(lines).encode("utf-8"))

    def run(self, problem_limit: int | None) -> None:
        self.verify_login()
        self.log("[INFO] signed-in Programmers session confirmed")
        problems = self.load_solved_problems(problem_limit)
        self.log(f"[INFO] selected {len(problems)} solved problem(s)")
        metadata = self.load_metadata()

        for index, problem in enumerate(problems, start=1):
            problem_id = str(problem.get("id"))
            title = str(problem.get("title") or "Untitled")
            try:
                self.wait()
                self.navigate(PROBLEM_URL.format(problem_id=problem_id))
                self.wait()
                submissions = self.load_submissions(problem_id)
                latest = self.latest_accepted_by_language(submissions)
                if not latest:
                    self.log(f"[SKIP] no accepted submission found: {problem_id} {title}")
                for language, extension, submission in latest.values():
                    self.save_solution(
                        problem, language, extension, submission, metadata
                    )
                self.log(f"[INFO] progress {index}/{len(problems)}")
            except AccessLimitedError:
                raise
            except Exception as error:  # keep one problem failure from stopping the run
                message = f"{type(error).__name__}: {error}"
                self.failures.append({"id": problem_id, "title": title, "error": message})
                self.log(f"[ERROR] {problem_id} {title}: {message}")

        self.write_metadata(metadata)
        if self.failures:
            self.log("[SUMMARY] failed problems:")
            for failure in self.failures:
                self.log(
                    f"  - {failure['id']} {failure['title']}: {failure['error']}"
                )
        else:
            self.log("[SUMMARY] completed without per-problem failures")


def main() -> int:
    args = parse_args()
    limit = 3 if args.test else args.limit
    try:
        with sync_playwright() as playwright:
            runner = BackupRunner(args, playwright)
            runner.run(limit)
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
