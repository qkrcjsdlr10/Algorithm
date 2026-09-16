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

EXTENSION_LANGUAGES: dict[str, str] = {
    ".c": "C",
    ".cpp": "CPP",
    ".cs": "CSharp",
    ".go": "Go",
    ".java": "Java",
    ".js": "JavaScript",
    ".kt": "Kotlin",
    ".py": "Python",
    ".rb": "Ruby",
    ".rs": "Rust",
    ".scala": "Scala",
    ".sql": "SQL",
    ".swift": "Swift",
    ".ts": "TypeScript",
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
    mode.add_argument(
        "--inspect",
        action="store_true",
        help="inspect the signed-in solved/problem UI without writing backups",
    )
    mode.add_argument(
        "--test",
        action="store_true",
        help="save at most 3 locally missing solved problems",
    )
    mode.add_argument(
        "--all",
        action="store_true",
        help="save all locally missing solved problems without overwriting files",
    )
    mode.add_argument(
        "--limit",
        type=int,
        metavar="N",
        help="save at most N locally missing solved problems",
    )
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

    def inspect_flow(self) -> None:
        """Report the actual signed-in DOM links and observed request paths."""
        observed: list[tuple[int, str]] = []

        def remember_response(response: Any) -> None:
            if "programmers.co.kr" in response.url:
                observed.append((response.status, response.url))

        self.page.on("response", remember_response)
        self.verify_login()
        self.page.wait_for_timeout(1_500)
        links = self.page.locator("a").evaluate_all(
            """elements => elements.map(element => ({
                text: (element.innerText || '').trim(),
                href: element.href || '',
            })).filter(item => item.href.includes('/learn/courses/30/lessons/'))"""
        )
        unique_links: list[dict[str, str]] = []
        seen_urls: set[str] = set()
        for link in links:
            href = str(link.get("href") or "")
            if href and href not in seen_urls:
                seen_urls.add(href)
                unique_links.append(link)

        print(f"[INSPECT] solved URL: {self.page.url}")
        print(f"[INSPECT] solved links visible in DOM: {len(unique_links)}")
        for link in unique_links[:5]:
            print(f"[INSPECT] solved link: {link}")
        print("[INSPECT] observed solved-list routes:")
        for status, url in dict.fromkeys(observed):
            if "/api/" in url or "challenges" in url:
                print(f"  {status} {url}")

        if not unique_links:
            raise BackupError("No solved problem link was found in the current DOM")

        observed.clear()
        self.navigate(unique_links[0]["href"])
        self.page.wait_for_timeout(1_000)
        controls = self.page.locator("a, button").evaluate_all(
            """elements => elements.map(element => ({
                tag: element.tagName,
                text: (element.innerText || '').trim(),
                href: element.href || '',
            })).filter(item => /제출|채점|기록|내역/.test(item.text))"""
        )
        print(f"[INSPECT] problem URL from DOM: {self.page.url}")
        print(f"[INSPECT] problem title: {self.page.title()}")
        print(f"[INSPECT] submission-related controls: {controls[:20]}")
        print("[INSPECT] observed problem routes:")
        for status, url in dict.fromkeys(observed):
            if "/api/" in url or "submission" in url or "lesson" in url:
                print(f"  {status} {url}")

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

    def existing_solution_keys(
        self, metadata: dict[str, dict[str, Any]]
    ) -> set[str]:
        keys = {
            key
            for key, item in metadata.items()
            if item.get("file") and (self.output / str(item["file"])).is_file()
        }
        for path in self.output.rglob("*"):
            if not path.is_file():
                continue
            match = re.match(r"^(\d+)_", path.name)
            language = EXTENSION_LANGUAGES.get(path.suffix.casefold())
            if match and language:
                keys.add(f"{match.group(1)}|{language.casefold()}")
        return keys

    @staticmethod
    def metadata_item(
        problem: dict[str, Any],
        language: str,
        relative: str,
        submission: dict[str, Any],
        action: str,
    ) -> dict[str, Any]:
        problem_id = str(problem["id"])
        level_value = problem.get("level")
        level = (
            f"Lv{level_value}"
            if isinstance(level_value, int) and 0 <= level_value <= 5
            else "Unclassified"
        )
        return {
            "id": problem_id,
            "title": str(problem.get("title") or "Untitled"),
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
            "sha256": code_digest(submission["code"]),
            "lastAction": action,
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
            action = "NEW"
            path.write_bytes(code_bytes)
            self.log(f"[NEW] {problem_id} {title} / {language}\n      -> {relative}")

        metadata[key] = self.metadata_item(
            problem, language, relative, submission, action
        )

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

    def run_missing(self, problem_limit: int | None) -> None:
        """Save only absent problem/language pairs; never update an existing file."""
        self.verify_login()
        self.log("[INFO] signed-in Programmers session confirmed")
        problems = self.load_solved_problems(None)
        metadata = self.load_metadata()
        existing_keys = self.existing_solution_keys(metadata)
        existing_problem_ids = {key.split("|", 1)[0] for key in existing_keys}
        ordered = sorted(
            enumerate(problems),
            key=lambda item: (str(item[1].get("id")) in existing_problem_ids, item[0]),
        )
        selected_problems = 0
        new_files = 0
        metadata_changed = False

        self.log(
            f"[INFO] checking {len(problems)} solved problem(s) against "
            f"{len(existing_keys)} local problem/language key(s)"
        )
        for _, problem in ordered:
            if problem_limit is not None and selected_problems >= problem_limit:
                break
            problem_id = str(problem.get("id") or "")
            title = str(problem.get("title") or "Untitled")
            try:
                self.wait()
                self.navigate(PROBLEM_URL.format(problem_id=problem_id))
                self.wait()
                submissions = self.load_submissions(problem_id)
                latest = self.latest_accepted_by_language(submissions)
                missing: list[tuple[str, str, dict[str, Any]]] = []
                recoverable: list[tuple[str, dict[str, Any], Path]] = []
                for language, extension, submission in latest.values():
                    key = f"{problem_id}|{language.casefold()}"
                    if key not in existing_keys:
                        missing.append((language, extension, submission))
                        continue
                    if key in metadata:
                        continue
                    level_value = problem.get("level")
                    level = (
                        f"Lv{level_value}"
                        if isinstance(level_value, int) and 0 <= level_value <= 5
                        else "Unclassified"
                    )
                    filename = build_filename(problem_id, title, language, extension)
                    target = self.output / level / filename
                    if target.is_file() and target.read_bytes() == submission["code"].encode(
                        "utf-8"
                    ):
                        recoverable.append((language, submission, target))

                if not missing and not recoverable:
                    continue

                selected_problems += 1
                for language, submission, target in recoverable:
                    key = f"{problem_id}|{language.casefold()}"
                    relative = target.relative_to(self.output).as_posix()
                    metadata[key] = self.metadata_item(
                        problem, language, relative, submission, "SKIP"
                    )
                    metadata_changed = True
                    self.log(
                        f"[SKIP] existing identical file indexed: "
                        f"{problem_id} {title} / {language}\n"
                        f"       -> {relative}"
                    )
                for language, extension, submission in missing:
                    key = f"{problem_id}|{language.casefold()}"
                    level_value = problem.get("level")
                    level = (
                        f"Lv{level_value}"
                        if isinstance(level_value, int) and 0 <= level_value <= 5
                        else "Unclassified"
                    )
                    filename = build_filename(problem_id, title, language, extension)
                    target = self.output / level / filename
                    if target.exists():
                        self.log(
                            f"[SKIP] ambiguous existing target kept unchanged: "
                            f"{target.relative_to(self.output).as_posix()}"
                        )
                        existing_keys.add(key)
                        continue
                    self.save_solution(
                        problem, language, extension, submission, metadata
                    )
                    existing_keys.add(key)
                    new_files += 1
                    metadata_changed = True
            except AccessLimitedError:
                raise
            except Exception as error:
                message = f"{type(error).__name__}: {error}"
                self.failures.append({"id": problem_id, "title": title, "error": message})
                self.log(f"[ERROR] {problem_id} {title}: {message}")

        if metadata_changed:
            self.write_metadata(metadata)
        self.log(
            f"[SUMMARY] missing-only run: {selected_problems} problem(s), "
            f"{new_files} new file(s)"
        )
        if self.failures:
            self.log("[SUMMARY] failed problems:")
            for failure in self.failures:
                self.log(
                    f"  - {failure['id']} {failure['title']}: {failure['error']}"
                )

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
            if args.inspect:
                runner.inspect_flow()
            else:
                runner.run_missing(limit)
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
