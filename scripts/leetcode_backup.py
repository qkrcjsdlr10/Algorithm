#!/usr/bin/env python3
"""Back up the signed-in user's accepted LeetCode submissions."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
import time
from collections import Counter
from datetime import datetime
from pathlib import Path
from typing import Any

from playwright.sync_api import Browser, Page, Playwright, sync_playwright


if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")
    sys.stderr.reconfigure(encoding="utf-8")


START_URL = "https://leetcode.com/progress/"
GRAPHQL_URL = "https://leetcode.com/graphql/"
EXPECTED_USERNAME = "nachal123"
INVALID_FILENAME_CHARS = re.compile(r'[\\/:*?"<>|\x00-\x1f]')
WHITESPACE = re.compile(r"\s+")
UNDERSCORES = re.compile(r"_+")

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
    "golang": ("Go", ".go"),
    "c#": ("CSharp", ".cs"),
    "csharp": ("CSharp", ".cs"),
    "ruby": ("Ruby", ".rb"),
    "scala": ("Scala", ".scala"),
    "rust": ("Rust", ".rs"),
    "php": ("PHP", ".php"),
    "racket": ("Racket", ".rkt"),
    "erlang": ("Erlang", ".erl"),
    "elixir": ("Elixir", ".ex"),
    "dart": ("Dart", ".dart"),
    "mysql": ("MySQL", ".sql"),
    "mssql": ("MSSQL", ".sql"),
    "oraclesql": ("OracleSQL", ".sql"),
    "pandas": ("Pandas", ".py"),
}

REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
STATE_PATH = REPOSITORY_ROOT / "scripts" / "state" / "leetcode_state.json"

PROGRESS_QUERY = """
query userProgressQuestionList($filters: UserProgressQuestionListInput) {
  userProgressQuestionList(filters: $filters) {
    totalNum
    questions {
      translatedTitle
      frontendId
      title
      titleSlug
      difficulty
      lastSubmittedAt
      numSubmitted
      questionStatus
      lastResult
      topicTags { name nameTranslated slug }
    }
  }
}
"""

GLOBAL_DATA_QUERY = """
query globalData {
  userStatus {
    userId isSignedIn isMockUser isPremium premiumCountryCode isVerified
    username realName avatar isAdmin isSuperuser permissions isTranslator
    activeSessionId checkedInToday completedFeatureGuides premiumExpiredAt
    notificationStatus { lastModified numUnread }
  }
}
"""

SUBMISSION_LIST_QUERY = """
query submissionList($offset: Int!, $limit: Int!, $lastKey: String, $questionSlug: String!, $lang: Int, $status: Int) {
  questionSubmissionList(offset: $offset, limit: $limit, lastKey: $lastKey, questionSlug: $questionSlug, lang: $lang, status: $status) {
    lastKey
    hasNext
    submissions {
      id title titleSlug status statusDisplay lang langName runtime timestamp url
      isPending memory hasNotes notes flagType frontendId
      topicTags { id }
    }
  }
}
"""

SUBMISSION_DETAILS_QUERY = """
query submissionDetails($submissionId: Int!) {
  submissionDetails(submissionId: $submissionId) {
    runtime runtimeDisplay runtimePercentile runtimeDistribution
    memory memoryDisplay memoryPercentile memoryDistribution
    code timestamp statusCode aiJudgeMessage isCompiledLang aiRecheckSubmitted
    user { username profile { realName userAvatar } }
    lang { name verboseName }
    question { questionId titleSlug hasFrontendPreview }
    notes flagType
    topicTags { tagId slug name }
    runtimeError compileError lastTestcase codeOutput expectedOutput totalCorrect
    totalTestcases fullCodeOutput testDescriptions testBodies testInfo stdOutput
  }
}
"""


class BackupError(RuntimeError):
    pass


class AuthenticationError(BackupError):
    pass


class AccessLimitedError(BackupError):
    pass


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Back up accepted LeetCode submissions")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--inspect", action="store_true")
    mode.add_argument("--test", action="store_true", help="save at most 3 new problems")
    mode.add_argument("--all", action="store_true")
    mode.add_argument("--limit", type=int, metavar="N")
    parser.add_argument("--cdp-url", default="http://127.0.0.1:9222")
    parser.add_argument("--delay", type=float, default=1.0)
    parser.add_argument(
        "--output",
        type=Path,
        default=REPOSITORY_ROOT / "LeetCode",
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


def normalize_language(value: str) -> tuple[str, str]:
    key = re.sub(r"[\s._-]+", "", value).casefold()
    if key in LANGUAGES:
        return LANGUAGES[key]
    return sanitize_component(value or "Unknown"), ".txt"


def difficulty_folder(value: str) -> str:
    normalized = value.strip().title()
    return normalized if normalized in {"Easy", "Medium", "Hard"} else "Unclassified"


def submitted_at(value: Any) -> str:
    try:
        stamp = int(value)
    except (TypeError, ValueError):
        return str(value or "")
    return datetime.fromtimestamp(stamp).astimezone().isoformat(timespec="seconds")


def normalized_bytes(value: bytes) -> bytes:
    return value.replace(b"\r\n", b"\n").replace(b"\r", b"\n")


def timestamp_value(value: Any) -> int:
    try:
        return int(value)
    except (TypeError, ValueError):
        pass
    try:
        return int(datetime.fromisoformat(str(value).replace("Z", "+00:00")).timestamp())
    except (TypeError, ValueError):
        return 0


class StateStore:
    def __init__(self, path: Path = STATE_PATH) -> None:
        self.path = path
        self.data = self.load()

    def load(self) -> dict[str, Any]:
        if self.path.is_file():
            raw = json.loads(self.path.read_text(encoding="utf-8"))
            if isinstance(raw, dict):
                return raw
        return {
            "status": "paused",
            "lastUpdated": "",
            "lastProblemId": None,
            "lastSubmissionId": None,
            "processed": [],
            "failedItems": [],
            "nextAction": "Run python scripts/leetcode_backup.py",
            "lastSeenAcceptedAt": {},
        }

    def save(self, **changes: Any) -> None:
        self.data.update(changes)
        self.data["lastUpdated"] = datetime.now().astimezone().isoformat(timespec="seconds")
        self.data["processed"] = sorted(
            set(self.data.get("processed") or []),
            key=lambda key: (
                int(str(key).split(":", 1)[0]) if str(key).split(":", 1)[0].isdigit() else 10**12,
                str(key),
            ),
        )
        self.path.parent.mkdir(parents=True, exist_ok=True)
        temporary = self.path.with_suffix(".json.tmp")
        temporary.write_text(
            json.dumps(self.data, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
            newline="\n",
        )
        temporary.replace(self.path)

    def seed_metadata(self, metadata: dict[str, dict[str, Any]]) -> None:
        processed = set(self.data.get("processed") or [])
        last_seen = dict(self.data.get("lastSeenAcceptedAt") or {})
        for item in metadata.values():
            problem_id = str(item.get("id") or "")
            language = str(item.get("language") or "")
            if problem_id and language:
                processed.add(f"{problem_id}:{language}")
                stamp = timestamp_value(item.get("submittedAt"))
                last_seen[problem_id] = max(int(last_seen.get(problem_id) or 0), stamp)
        self.data["processed"] = list(processed)
        self.data["lastSeenAcceptedAt"] = last_seen

    def checkpoint(
        self,
        problem_id: str,
        language: str,
        submission_id: str,
        question_accepted_at: Any,
    ) -> None:
        processed = set(self.data.get("processed") or [])
        processed.add(f"{problem_id}:{language}")
        last_seen = dict(self.data.get("lastSeenAcceptedAt") or {})
        last_seen[problem_id] = max(
            int(last_seen.get(problem_id) or 0), timestamp_value(question_accepted_at)
        )
        self.save(
            status="running",
            lastProblemId=problem_id,
            lastSubmissionId=submission_id,
            processed=list(processed),
            lastSeenAcceptedAt=last_seen,
            nextAction="Continue the current LeetCode backup run",
        )


class BackupRunner:
    def __init__(self, args: argparse.Namespace, playwright: Playwright, state: StateStore) -> None:
        self.args = args
        self.output = args.output.resolve()
        self.state = state
        self.stats = Counter()
        self.browser: Browser = playwright.chromium.connect_over_cdp(args.cdp_url)
        if not self.browser.contexts:
            raise BackupError("CDP browser has no available context")
        self.context = self.browser.contexts[0]
        self.page: Page = next(
            (page for page in self.context.pages if "leetcode.com" in page.url),
            self.context.new_page(),
        )

    def wait(self) -> None:
        time.sleep(self.args.delay)

    def navigate(self, url: str) -> None:
        response = self.page.goto(url, wait_until="domcontentloaded", timeout=60_000)
        self.page.wait_for_timeout(2_000)
        status = response.status if response else 200
        body = self.page.locator("body").inner_text(timeout=20_000)
        lowered = body.casefold()
        if status in {403, 429}:
            raise AccessLimitedError(f"HTTP {status}: {url}")
        if "captcha" in lowered or "too many requests" in lowered or "access denied" in lowered:
            raise AccessLimitedError(f"access restriction detected: {url}")
        if status >= 400:
            raise BackupError(f"HTTP {status}: {url}")

    def graphql(self, query: str, variables: dict[str, Any], operation: str) -> dict[str, Any]:
        self.wait()
        result = self.page.evaluate(
            """async ({url, query, variables, operationName}) => {
                const response = await fetch(url, {
                    method:'POST', credentials:'include',
                    headers:{'Content-Type':'application/json'},
                    body:JSON.stringify({query, variables, operationName}),
                });
                return {status:response.status, text:await response.text()};
            }""",
            {
                "url": GRAPHQL_URL,
                "query": query,
                "variables": variables,
                "operationName": operation,
            },
        )
        status = int(result["status"])
        text = str(result["text"])
        if status in {403, 429}:
            raise AccessLimitedError(f"HTTP {status}: {GRAPHQL_URL}")
        lowered = text[:5000].casefold()
        if "captcha" in lowered or "too many requests" in lowered or "access denied" in lowered:
            raise AccessLimitedError("LeetCode access restriction detected")
        if status >= 400:
            raise BackupError(f"GraphQL HTTP {status}")
        payload = json.loads(text)
        if payload.get("errors"):
            raise BackupError(f"GraphQL {operation} errors: {payload['errors']}")
        return payload.get("data") or {}

    def verify_login(self) -> str:
        self.navigate(START_URL)
        data = self.graphql(GLOBAL_DATA_QUERY, {}, "globalData")
        user = data.get("userStatus") or {}
        if not user.get("isSignedIn"):
            raise AuthenticationError("log in to LeetCode in the dedicated Edge profile")
        username = str(user.get("username") or "")
        if username != EXPECTED_USERNAME:
            raise AuthenticationError(f"unexpected LeetCode account: {username or 'unknown'}")
        return username

    def collect_questions(self) -> list[dict[str, Any]]:
        data = self.graphql(
            PROGRESS_QUERY,
            {"filters": {"skip": 0, "limit": 50}},
            "userProgressQuestionList",
        )
        result = data.get("userProgressQuestionList") or {}
        total = int(result.get("totalNum") or 0)
        questions = list(result.get("questions") or [])
        skip = len(questions)
        while skip < total:
            page_data = self.graphql(
                PROGRESS_QUERY,
                {"filters": {"skip": skip, "limit": 50}},
                "userProgressQuestionList",
            )
            batch = list((page_data.get("userProgressQuestionList") or {}).get("questions") or [])
            if not batch:
                break
            questions.extend(batch)
            skip += len(batch)
        return [
            item
            for item in questions
            if str(item.get("questionStatus") or "").casefold() == "solved"
            and str(item.get("lastResult") or "").casefold() in {"ac", "accepted"}
        ]

    def load_metadata(self) -> dict[str, dict[str, Any]]:
        path = self.output / "metadata.json"
        if not path.exists():
            return {}
        raw = json.loads(path.read_text(encoding="utf-8"))
        if not isinstance(raw, list):
            raise BackupError("LeetCode metadata.json must contain an array")
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
        extensions = {extension: language for language, extension in LANGUAGES.values()}
        for path in self.output.rglob("*"):
            if not path.is_file() or path.suffix.casefold() not in extensions:
                continue
            match = re.match(r"^(\d+)_", path.name)
            if not match:
                continue
            language = extensions[path.suffix.casefold()]
            result.setdefault(f"{match.group(1)}|{language.casefold()}", path)
        return result

    def latest_accepted(self, slug: str) -> dict[str, dict[str, Any]]:
        latest: dict[str, dict[str, Any]] = {}
        offset = 0
        last_key: str | None = None
        while True:
            data = self.graphql(
                SUBMISSION_LIST_QUERY,
                {
                    "questionSlug": slug,
                    "offset": offset,
                    "limit": 20,
                    "lastKey": last_key,
                },
                "submissionList",
            )
            result = data.get("questionSubmissionList") or {}
            submissions = list(result.get("submissions") or [])
            for item in submissions:
                if str(item.get("statusDisplay") or "").casefold() != "accepted":
                    continue
                language, extension = normalize_language(str(item.get("langName") or item.get("lang") or ""))
                candidate = dict(item)
                candidate["language"] = language
                candidate["extension"] = extension
                key = language.casefold()
                if key not in latest or int(candidate.get("timestamp") or 0) > int(latest[key].get("timestamp") or 0):
                    latest[key] = candidate
            if not result.get("hasNext") or not submissions:
                break
            offset += len(submissions)
            last_key = result.get("lastKey")
        return latest

    def submission_details(self, submission_id: int) -> dict[str, Any]:
        data = self.graphql(
            SUBMISSION_DETAILS_QUERY,
            {"submissionId": submission_id},
            "submissionDetails",
        )
        details = data.get("submissionDetails") or {}
        username = str((details.get("user") or {}).get("username") or "")
        if username != EXPECTED_USERNAME:
            raise AuthenticationError(f"submission {submission_id} is not owned by {EXPECTED_USERNAME}")
        if int(details.get("statusCode") or 0) != 10:
            raise BackupError(f"submission {submission_id} is not Accepted")
        if not isinstance(details.get("code"), str):
            raise BackupError(f"submission {submission_id} has no source code")
        return details

    def write_metadata(self, metadata: dict[str, dict[str, Any]]) -> None:
        entries = sorted(
            metadata.values(),
            key=lambda item: (
                {"Easy": 0, "Medium": 1, "Hard": 2}.get(str(item.get("difficulty")), 3),
                int(item.get("id")) if str(item.get("id", "")).isdigit() else 10**12,
                str(item.get("id")),
                str(item.get("language")),
            ),
        )
        payload = json.dumps(entries, ensure_ascii=False, indent=2) + "\n"
        path = self.output / "metadata.json"
        temporary = path.with_suffix(".json.tmp")
        temporary.write_text(payload, encoding="utf-8", newline="\n")
        temporary.replace(path)

    def process(self, questions: list[dict[str, Any]], limit: int | None) -> dict[str, dict[str, Any]]:
        self.output.mkdir(parents=True, exist_ok=True)
        metadata = self.load_metadata()
        self.state.seed_metadata(metadata)
        self.state.save(
            status="running",
            failedItems=[],
            nextAction="Continue the current LeetCode backup run",
        )
        existing = self.existing_files(metadata)
        existing_ids = {key.split("|", 1)[0] for key in existing}
        completed = 0
        for question in questions:
            problem_id = str(question.get("frontendId") or question.get("titleSlug") or "")
            if limit is not None and problem_id in existing_ids:
                continue
            if limit is not None and completed >= limit:
                break
            slug = str(question.get("titleSlug") or "")
            title = str(question.get("title") or question.get("translatedTitle") or slug)
            difficulty = difficulty_folder(str(question.get("difficulty") or ""))
            remote_accepted_at = timestamp_value(question.get("lastSubmittedAt"))
            checkpoint_at = int(
                (self.state.data.get("lastSeenAcceptedAt") or {}).get(problem_id) or 0
            )
            known_keys = [key for key in metadata if key.startswith(f"{problem_id}|")]
            if known_keys and remote_accepted_at and remote_accepted_at <= checkpoint_at:
                self.stats["SKIP"] += len(known_keys)
                print(f"[SKIP] {problem_id} {title} | checkpoint unchanged")
                continue
            try:
                submissions = self.latest_accepted(slug)
                if not submissions:
                    print(f"[SKIP] {problem_id} | {title} | no Accepted submission")
                    self.stats["SKIP"] += 1
                    continue
                for submission in submissions.values():
                    language = str(submission["language"])
                    key = f"{problem_id}|{language.casefold()}"
                    details = self.submission_details(int(submission["id"]))
                    code_bytes = str(details["code"]).encode("utf-8")
                    filename = (
                        f"{sanitize_component(problem_id)}_"
                        f"{sanitize_component(title)}_"
                        f"{language}{submission['extension']}"
                    )
                    target = self.output / difficulty / filename
                    relative = target.relative_to(self.output).as_posix()
                    action = "NEW"
                    existing_path = existing.get(key)
                    if existing_path is not None:
                        relative = existing_path.relative_to(self.output).as_posix()
                        if normalized_bytes(existing_path.read_bytes()) == normalized_bytes(code_bytes):
                            action = "SKIP"
                        else:
                            old = metadata.get(key) or {}
                            old_time = str(old.get("submittedAt") or "")
                            new_time = submitted_at(details.get("timestamp"))
                            if old_time and new_time > old_time:
                                existing_path.write_bytes(code_bytes)
                                action = "UPDATE"
                            else:
                                action = "UPDATE-CANDIDATE"
                    elif target.exists():
                        action = "SKIP" if normalized_bytes(target.read_bytes()) == normalized_bytes(code_bytes) else "UPDATE-CANDIDATE"
                    if action == "NEW":
                        target.parent.mkdir(parents=True, exist_ok=True)
                        target.write_bytes(code_bytes)
                        existing[key] = target
                    if action in {"NEW", "SKIP", "UPDATE"}:
                        path = existing.get(key, existing_path or target)
                        metadata[key] = {
                            "id": problem_id,
                            "title": title,
                            "slug": slug,
                            "difficulty": difficulty,
                            "language": language,
                            "file": relative,
                            "submittedAt": submitted_at(details.get("timestamp")),
                            "url": f"https://leetcode.com/problems/{slug}/",
                            "submissionId": str(submission["id"]),
                            "result": "Accepted",
                            "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
                        }
                        self.write_metadata(metadata)
                        self.state.checkpoint(
                            problem_id,
                            language,
                            str(submission["id"]),
                            question.get("lastSubmittedAt") or details.get("timestamp"),
                        )
                    self.stats[action] += 1
                    print(
                        f"[{action}] {problem_id} | {title} | {difficulty} | {language} | "
                        f"{submitted_at(details.get('timestamp'))} | {relative}"
                    )
                completed += 1
            except AccessLimitedError:
                raise
            except Exception as error:
                failed = list(self.state.data.get("failedItems") or [])
                failed.append(
                    {"id": problem_id, "title": title, "error": f"{type(error).__name__}: {error}"}
                )
                self.state.save(failedItems=failed)
                self.stats["ERROR"] += 1
                print(f"[ERROR] {problem_id} {title}: {type(error).__name__}: {error}")
        print(f"[INFO] completed problems: {completed}")
        return metadata

    def validate(self, accepted_count: int, metadata: dict[str, dict[str, Any]]) -> bool:
        source_files = [
            path
            for path in self.output.rglob("*")
            if path.is_file() and path.name != "metadata.json"
        ]
        zero_bytes = [path for path in source_files if path.stat().st_size == 0]
        missing: list[str] = []
        mismatched: list[str] = []
        for item in metadata.values():
            relative = str(item.get("file") or "")
            path = self.output / relative
            if not path.is_file():
                missing.append(relative)
                continue
            expected = str(item.get("sha256") or "")
            actual = hashlib.sha256(path.read_bytes()).hexdigest()
            if expected and expected != actual:
                mismatched.append(relative)
        unique_ids = {str(item.get("id")) for item in metadata.values()}
        difficulty_counts = Counter(
            str(item.get("difficulty") or "Unclassified") for item in metadata.values()
        )
        language_counts = Counter(str(item.get("language") or "Unknown") for item in metadata.values())
        failures = list(self.state.data.get("failedItems") or [])
        print(f"[VERIFY] Accepted problems: {accepted_count}")
        print(f"[VERIFY] stored unique problems: {len(unique_ids)}")
        print(f"[VERIFY] stored problem/language files: {len(metadata)}")
        print(
            f"[VERIFY] NEW={self.stats['NEW']} SKIP={self.stats['SKIP']} "
            f"UPDATE={self.stats['UPDATE']} failures={len(failures)}"
        )
        print(f"[VERIFY] zero-byte files: {len(zero_bytes)}")
        print(f"[VERIFY] missing metadata files: {len(missing)}")
        print(f"[VERIFY] SHA-256 mismatches: {len(mismatched)}")
        print(
            "[VERIFY] difficulty problems: "
            + " ".join(
                f"{level}={len({str(item.get('id')) for item in metadata.values() if item.get('difficulty') == level})}"
                for level in ("Easy", "Medium", "Hard", "Unclassified")
            )
        )
        print(
            "[VERIFY] languages: "
            + (" ".join(f"{key}={value}" for key, value in sorted(language_counts.items())) or "none")
        )
        for item in failures:
            print(f"[ERROR] failed item: {item.get('id')} {item.get('title')} | {item.get('error')}")
        for relative in zero_bytes:
            print(f"[ERROR] zero-byte file: {relative.relative_to(self.output).as_posix()}")
        for relative in missing:
            print(f"[ERROR] missing file: {relative}")
        for relative in mismatched:
            print(f"[ERROR] hash mismatch: {relative}")
        return not (failures or zero_bytes or missing or mismatched)

    def inspect(self) -> None:
        username = self.verify_login()
        questions = self.collect_questions()
        print(f"[INSPECT] logged in as {username}")
        print(f"[INSPECT] Accepted problems: {len(questions)}")
        for item in questions:
            print(
                f"[INSPECT] {item.get('frontendId')} | {item.get('title')} | "
                f"{difficulty_folder(str(item.get('difficulty') or ''))} | {item.get('titleSlug')} | "
                f"{item.get('lastSubmittedAt')} | {item.get('numSubmitted')} submission(s)"
            )


def main() -> int:
    args = parse_args()
    state = StateStore()
    try:
        with sync_playwright() as playwright:
            runner = BackupRunner(args, playwright, state)
            if args.inspect:
                runner.inspect()
            else:
                username = runner.verify_login()
                questions = runner.collect_questions()
                print(f"[INFO] logged in as {username}; Accepted problems: {len(questions)}")
                metadata = runner.process(questions, 3 if args.test else args.limit)
                valid = runner.validate(len(questions), metadata)
                state.save(
                    status="completed" if valid else "failed",
                    nextAction=(
                        "Run python scripts/leetcode_backup.py after solving new LeetCode problems"
                        if valid
                        else "Review failedItems and rerun python scripts/leetcode_backup.py"
                    ),
                    lastRunStats={
                        "acceptedProblems": len(questions),
                        "storedProblems": len({str(item.get('id')) for item in metadata.values()}),
                        "storedLanguageFiles": len(metadata),
                        "new": runner.stats["NEW"],
                        "skip": runner.stats["SKIP"],
                        "update": runner.stats["UPDATE"],
                        "failed": len(state.data.get("failedItems") or []),
                    },
                )
                if not valid:
                    return 2
        return 0
    except KeyboardInterrupt:
        state.save(
            status="paused",
            nextAction="Rerun python scripts/leetcode_backup.py to continue from the last checkpoint",
        )
        print("[STOP] interrupted; checkpoint saved", file=sys.stderr)
        return 130
    except AccessLimitedError as error:
        state.save(status="paused", nextAction="Wait, then rerun python scripts/leetcode_backup.py")
        print(f"[STOP] access restriction detected; no bypass attempted: {error}", file=sys.stderr)
        return 3
    except AuthenticationError as error:
        state.save(status="paused", nextAction="Log in to LeetCode, then rerun python scripts/leetcode_backup.py")
        print(f"[STOP] authentication required: {error}", file=sys.stderr)
        return 4
    except Exception as error:
        state.save(
            status="failed",
            nextAction="Resolve the reported error, then rerun python scripts/leetcode_backup.py",
        )
        print(f"[FATAL] {type(error).__name__}: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
