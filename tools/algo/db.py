"""Local SQLite state (``.algo/state.sqlite3``, ignored by Git).

Three identities are kept separate:

* submission  -> ``submissions``  UNIQUE(site, submission_key)   one site submission
* solution    -> ``solutions``    UNIQUE(site, problem_id, lang_key)  one problem+language
* version     -> ``versions``     UNIQUE(solution_id, normalized_hash) one code version

``files`` tracks each local file and its sync state.
"""

from __future__ import annotations

import sqlite3
from contextlib import contextmanager
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Iterator

DISCOVERED = "DISCOVERED"
PENDING = "PENDING_VERIFICATION"
ACCEPTED = "ACCEPTED"
SYNCED = "SYNCED"
FAILED = "FAILED"
SKIPPED = "SKIPPED"
STATUSES = (DISCOVERED, PENDING, ACCEPTED, SYNCED, FAILED, SKIPPED)

SCHEMA = """
CREATE TABLE IF NOT EXISTS submissions (
    id INTEGER PRIMARY KEY,
    site TEXT NOT NULL,
    submission_key TEXT NOT NULL,
    problem_id TEXT NOT NULL,
    title TEXT,
    tier TEXT,
    language TEXT NOT NULL,
    lang_key TEXT NOT NULL,
    verdict TEXT NOT NULL,
    raw_hash TEXT,
    normalized_hash TEXT,
    submitted_at TEXT,
    source TEXT NOT NULL,
    discovered_at TEXT NOT NULL,
    UNIQUE (site, submission_key)
);
CREATE INDEX IF NOT EXISTS submissions_solution ON submissions (site, problem_id, lang_key);

CREATE TABLE IF NOT EXISTS solutions (
    id INTEGER PRIMARY KEY,
    site TEXT NOT NULL,
    problem_id TEXT NOT NULL,
    lang_key TEXT NOT NULL,
    primary_path TEXT,
    UNIQUE (site, problem_id, lang_key)
);

CREATE TABLE IF NOT EXISTS versions (
    id INTEGER PRIMARY KEY,
    solution_id INTEGER NOT NULL REFERENCES solutions (id),
    raw_hash TEXT NOT NULL,
    normalized_hash TEXT NOT NULL,
    path TEXT,
    first_seen_at TEXT NOT NULL,
    UNIQUE (solution_id, normalized_hash)
);

CREATE TABLE IF NOT EXISTS files (
    id INTEGER PRIMARY KEY,
    path TEXT NOT NULL UNIQUE,
    site TEXT,
    problem_id TEXT,
    lang_key TEXT,
    language TEXT,
    solution_id INTEGER REFERENCES solutions (id),
    raw_hash TEXT,
    normalized_hash TEXT,
    status TEXT NOT NULL,
    reason TEXT,
    verification_method TEXT,
    submission_id INTEGER REFERENCES submissions (id),
    external_source TEXT,
    commit_sha TEXT,
    github_commit_sha TEXT,
    attempts INTEGER NOT NULL DEFAULT 0,
    first_seen_at TEXT NOT NULL,
    accepted_at TEXT,
    last_checked_at TEXT,
    updated_at TEXT NOT NULL
);
CREATE INDEX IF NOT EXISTS files_status ON files (status);

CREATE TABLE IF NOT EXISTS candidates (
    id INTEGER PRIMARY KEY,
    submission_id INTEGER NOT NULL UNIQUE REFERENCES submissions (id),
    target_path TEXT NOT NULL,
    stored_path TEXT NOT NULL,
    status TEXT NOT NULL,
    created_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS aux_paths (
    path TEXT PRIMARY KEY,
    added_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS events (
    id INTEGER PRIMARY KEY,
    at TEXT NOT NULL,
    level TEXT NOT NULL,
    site TEXT,
    path TEXT,
    message TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS kv (
    key TEXT PRIMARY KEY,
    value TEXT
);
"""


def now() -> str:
    return datetime.now(timezone.utc).astimezone().isoformat(timespec="seconds")


@dataclass
class Submission:
    site: str
    submission_key: str
    problem_id: str
    language: str
    lang_key: str
    verdict: str
    source: str
    title: str = ""
    tier: str = ""
    raw_hash: str | None = None
    normalized_hash: str | None = None
    submitted_at: str = ""


class Database:
    def __init__(self, path: Path | str) -> None:
        if str(path) != ":memory:":
            Path(path).parent.mkdir(parents=True, exist_ok=True)
        self.conn = sqlite3.connect(str(path), timeout=30)
        self.conn.row_factory = sqlite3.Row
        self.conn.execute("PRAGMA foreign_keys = ON")
        self.conn.execute("PRAGMA journal_mode = WAL") if str(path) != ":memory:" else None
        self.conn.executescript(SCHEMA)
        self.conn.commit()

    def close(self) -> None:
        self.conn.close()

    @contextmanager
    def tx(self) -> Iterator[sqlite3.Connection]:
        try:
            yield self.conn
            self.conn.commit()
        except BaseException:
            self.conn.rollback()
            raise

    def q(self, sql: str, *args: Any) -> list[sqlite3.Row]:
        return list(self.conn.execute(sql, args))

    def one(self, sql: str, *args: Any) -> sqlite3.Row | None:
        return self.conn.execute(sql, args).fetchone()

    # -- key/value ---------------------------------------------------------
    def get(self, key: str, default: str | None = None) -> str | None:
        row = self.one("SELECT value FROM kv WHERE key = ?", key)
        return row["value"] if row else default

    def set(self, key: str, value: str | None) -> None:
        with self.tx() as c:
            c.execute("INSERT INTO kv (key, value) VALUES (?, ?) "
                      "ON CONFLICT (key) DO UPDATE SET value = excluded.value", (key, value))

    # -- events ------------------------------------------------------------
    def event(self, level: str, message: str, site: str | None = None, path: str | None = None) -> None:
        with self.tx() as c:
            c.execute("INSERT INTO events (at, level, site, path, message) VALUES (?, ?, ?, ?, ?)",
                      (now(), level, site, path, message))

    # -- submissions -------------------------------------------------------
    def add_submission(self, s: Submission) -> tuple[int, bool]:
        """Insert once per (site, submission_key). Returns (id, created)."""
        row = self.one("SELECT id, raw_hash, normalized_hash FROM submissions "
                       "WHERE site = ? AND submission_key = ?", s.site, s.submission_key)
        if row:
            if (s.normalized_hash and not row["normalized_hash"]) or (s.raw_hash and not row["raw_hash"]):
                with self.tx() as c:
                    c.execute("UPDATE submissions SET raw_hash = COALESCE(raw_hash, ?), "
                              "normalized_hash = COALESCE(normalized_hash, ?) WHERE id = ?",
                              (s.raw_hash, s.normalized_hash, row["id"]))
            return int(row["id"]), False
        with self.tx() as c:
            cur = c.execute(
                "INSERT INTO submissions (site, submission_key, problem_id, title, tier, language, "
                "lang_key, verdict, raw_hash, normalized_hash, submitted_at, source, discovered_at) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                (s.site, s.submission_key, s.problem_id, s.title, s.tier, s.language, s.lang_key,
                 s.verdict, s.raw_hash, s.normalized_hash, s.submitted_at, s.source, now()))
            return int(cur.lastrowid), True

    def known_submission_keys(self, site: str) -> set[str]:
        return {r["submission_key"] for r in self.q("SELECT submission_key FROM submissions WHERE site = ?", site)}

    def accepted_submissions(self, site: str, problem_id: str, lang: str) -> list[sqlite3.Row]:
        return self.q("SELECT * FROM submissions WHERE site = ? AND problem_id = ? AND lang_key = ? "
                      "AND verdict = 'ACCEPTED' ORDER BY submitted_at DESC, id DESC", site, problem_id, lang)

    # -- solutions / versions ---------------------------------------------
    def solution_id(self, site: str, problem_id: str, lang: str, primary_path: str | None = None) -> int:
        with self.tx() as c:
            c.execute("INSERT INTO solutions (site, problem_id, lang_key, primary_path) VALUES (?, ?, ?, ?) "
                      "ON CONFLICT (site, problem_id, lang_key) DO UPDATE SET "
                      "primary_path = COALESCE(solutions.primary_path, excluded.primary_path)",
                      (site, problem_id, lang, primary_path))
        row = self.one("SELECT id FROM solutions WHERE site = ? AND problem_id = ? AND lang_key = ?",
                       site, problem_id, lang)
        assert row is not None
        return int(row["id"])

    def add_version(self, solution_id: int, raw: str, normalized: str, path: str) -> None:
        with self.tx() as c:
            c.execute("INSERT OR IGNORE INTO versions (solution_id, raw_hash, normalized_hash, path, "
                      "first_seen_at) VALUES (?, ?, ?, ?, ?)", (solution_id, raw, normalized, path, now()))

    # -- files -------------------------------------------------------------
    def file(self, path: str) -> sqlite3.Row | None:
        return self.one("SELECT * FROM files WHERE path = ?", path)

    def file_by_id(self, file_id: int) -> sqlite3.Row | None:
        return self.one("SELECT * FROM files WHERE id = ?", file_id)

    def upsert_file(self, path: str, **fields: Any) -> int:
        fields["updated_at"] = now()
        existing = self.file(path)
        with self.tx() as c:
            if existing is None:
                fields.setdefault("status", DISCOVERED)
                fields["first_seen_at"] = fields["updated_at"]
                columns = ["path", *fields]
                c.execute(f"INSERT INTO files ({', '.join(columns)}) VALUES ({', '.join('?' * len(columns))})",
                          (path, *fields.values()))
            else:
                assignments = ", ".join(f"{key} = ?" for key in fields)
                c.execute(f"UPDATE files SET {assignments} WHERE path = ?", (*fields.values(), path))
        row = self.file(path)
        assert row is not None
        return int(row["id"])

    def files_with_status(self, *statuses: str) -> list[sqlite3.Row]:
        marks = ", ".join("?" * len(statuses))
        return self.q(f"SELECT * FROM files WHERE status IN ({marks}) ORDER BY id", *statuses)

    def counts(self) -> dict[str, int]:
        result = {status: 0 for status in STATUSES}
        for row in self.q("SELECT status, COUNT(*) AS n FROM files GROUP BY status"):
            result[row["status"]] = int(row["n"])
        return result

    # -- auxiliary files (metadata.json etc.) that must ship with a commit --
    def add_aux(self, path: str) -> None:
        with self.tx() as c:
            c.execute("INSERT OR IGNORE INTO aux_paths (path, added_at) VALUES (?, ?)", (path, now()))

    def aux(self) -> list[str]:
        return [r["path"] for r in self.q("SELECT path FROM aux_paths ORDER BY path")]

    def clear_aux(self, paths: list[str]) -> None:
        with self.tx() as c:
            c.executemany("DELETE FROM aux_paths WHERE path = ?", [(p,) for p in paths])
