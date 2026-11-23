"""End-to-end flows on a temporary repository + bare origin (no real site)."""

from __future__ import annotations

import json
from datetime import datetime, timedelta

import pytest

from algo import db as dbm
from algo import engine, ingest, migrate, scanner
from algo.adapters.base import AdapterError, LoginRequired
from algo.publisher import Publisher, RetryLater
from algo.repo import ForbiddenGitOperation

from conftest import FakeAdapter, factory, git, submission


def setup(env, **publish):
    config = env.config(**publish)
    database = env.db()
    repo = env.repo()
    migrate.baseline(config, database, repo)
    return config, database, repo


def stats_total(readme: str, site: str) -> int:
    section = readme.split(f"### {site}\n", 1)[1]
    line = next(l for l in section.splitlines() if l.startswith("| **Total**"))
    return int(line.split("**")[3])


# 1, 2 ---------------------------------------------------------------------
def test_new_java_and_cpp_files_are_detected(env):
    config, database, repo = setup(env)
    env.write("BOJ/Silver/2839_설탕_배달_Java.java", "class Main {}\n")
    env.write("BOJ/Silver/2839_설탕_배달_CPP.cpp", "int main(){}\n")
    found = scanner.scan(config, database, repo)
    assert found["changed"] == 2
    java = database.file("BOJ/Silver/2839_설탕_배달_Java.java")
    cpp = database.file("BOJ/Silver/2839_설탕_배달_CPP.cpp")
    assert java["status"] == cpp["status"] == dbm.PENDING
    assert (java["lang_key"], cpp["lang_key"]) == ("java", "cpp")


def test_unidentifiable_file_asks_instead_of_guessing(env):
    config, database, repo = setup(env)
    env.write("BOJ/Silver/설탕배달.java", "class Main {}\n")
    scanner.scan(config, database, repo)
    row = database.file("BOJ/Silver/설탕배달.java")
    assert row["status"] == dbm.PENDING and row["site"] is None
    assert "문제 번호" in row["reason"]


# 3 ------------------------------------------------------------------------
def test_unverified_file_is_never_uploaded(env):
    config, database, repo = setup(env)
    env.write("BOJ/Silver/2839_설탕_배달_Java.java", "class Main {}\n")
    before = env.remote_log()
    engine.cycle(config, database, repo, adapters=factory())
    assert env.remote_log() == before
    assert database.file("BOJ/Silver/2839_설탕_배달_Java.java")["status"] == dbm.PENDING
    assert "BOJ/Silver/2839_설탕_배달_Java.java" not in env.remote_files()


# auto-verification with CRLF local file vs LF site code --------------------
def test_ide_file_verified_by_site_code_and_uploaded(env):
    config, database, repo = setup(env)
    local = "Programmers/Lv1/12940_최대공약수와_최소공배수_Java.java"
    env.write(local, b"class Solution {\r\n  int x;\r\n}\r\n")
    site_code = "class Solution {\n  int x;\n}\n"
    adapter = FakeAdapter("Programmers", "programmers", [submission(key="777", code=site_code)])
    report = engine.cycle(config, database, repo, adapters=factory(adapter), force_poll=True)
    row = database.file(local)
    assert row["status"] == dbm.SYNCED, report
    assert row["verification_method"] == "auto-hash-normalized"
    assert local in env.remote_files()
    # local bytes were not replaced by the site version
    assert (env.root / local).read_bytes() == b"class Solution {\r\n  int x;\r\n}\r\n"
    meta = json.loads(env.remote_show("Programmers/metadata.json"))
    assert meta[0]["submissionId"] == 777


# 4, 10 --------------------------------------------------------------------
def test_same_submission_is_processed_once_and_runs_are_idempotent(env):
    config, database, repo = setup(env)
    item = submission(key="501", problem="42840", title="모의고사")
    adapter = FakeAdapter("Programmers", "programmers", [item])
    engine.cycle(config, database, repo, adapters=factory(adapter), force_poll=True)
    log_after_first = env.remote_log()
    assert ingest.ingest(config, database, item) == "seen"
    for _ in range(3):
        migrate.baseline(config, database, repo)
        engine.cycle(config, database, repo, adapters=factory(adapter), force_poll=True)
    assert env.remote_log() == log_after_first
    files = [p for p in env.remote_files() if "42840_" in p]
    assert files == ["Programmers/Lv1/42840_모의고사_Java.java"]
    assert database.one("SELECT COUNT(*) AS n FROM submissions WHERE submission_key = '501'")["n"] == 1


def test_baseline_does_not_treat_existing_solutions_as_new(env):
    config, database, repo = setup(env)
    before = env.remote_log()
    engine.cycle(config, database, repo, adapters=factory())
    assert env.remote_log() == before
    assert database.counts()[dbm.SYNCED] == 4
    assert database.files_with_status(dbm.PENDING) == []


# 5 ------------------------------------------------------------------------
def test_java_and_cpp_of_same_problem_are_both_kept(env):
    config, database, repo = setup(env)
    items = [submission(key="11", problem="42576", title="완주하지 못한 선수", language="Java", extension=".java"),
             submission(key="12", problem="42576", title="완주하지 못한 선수", language="CPP", extension=".cpp",
                        code="int main(){}\n")]
    engine.cycle(config, database, repo, adapters=factory(FakeAdapter("Programmers", "programmers", items)),
                 force_poll=True)
    names = sorted(p for p in env.remote_files() if "42576_" in p)
    assert names == ["Programmers/Lv1/42576_완주하지_못한_선수_CPP.cpp",
                     "Programmers/Lv1/42576_완주하지_못한_선수_Java.java"]
    # README counts the problem once
    before = stats_total(git(env.remote, "show", "main~1:README.md"), "Programmers")
    assert stats_total(env.remote_show("README.md"), "Programmers") == before + 1


# 6 ------------------------------------------------------------------------
@pytest.mark.parametrize("policy,expected", [("ask", "candidate"), ("keep", "kept"), ("ignore", "ignored"),
                                             ("add_as_new", "added")])
def test_existing_file_is_never_overwritten(env, policy, expected):
    config = env.config()
    config.data["duplicates"]["policy"] = policy
    database, repo = env.db(), env.repo()
    migrate.baseline(config, database, repo)
    existing = env.root / "Programmers/Lv1/12928_약수의_합_Java.java"
    original = existing.read_bytes()
    outcome = ingest.ingest(config, database,
                            submission(key="900", problem="12928", title="약수의 합", code="class Solution { int v2; }\n"))
    assert outcome == expected
    assert existing.read_bytes() == original
    if policy == "add_as_new":
        assert (env.root / "Programmers/Lv1/12928_약수의_합_Java_v2.java").is_file()
    if policy == "ask":
        candidate = database.one("SELECT * FROM candidates")
        assert candidate["target_path"] == "Programmers/Lv1/12928_약수의_합_Java.java"


# 7 ------------------------------------------------------------------------
def test_api_error_backs_off_and_recovers(env):
    config, database, repo = setup(env)
    broken = FakeAdapter("Programmers", "programmers", error=AdapterError("timeout"))
    now = datetime.now().astimezone()
    report = engine.cycle(config, database, repo, adapters=factory(broken), now=now, force_poll=True)
    assert "timeout" in report.site_errors["Programmers"]
    assert database.get("backoff_until:Programmers")
    # not due while backing off
    assert engine.due_sites(config, database, now + timedelta(seconds=30), only="programmers") == []
    healthy = FakeAdapter("Programmers", "programmers", [submission(key="42", problem="42748", title="K번째수")])
    later = now + timedelta(hours=2)
    report = engine.cycle(config, database, repo, adapters=factory(healthy), now=later)
    assert report.fetched["Programmers"] == {"new": 1}
    assert database.get("fail_streak:Programmers") == "0"


def test_login_required_is_reported_not_bypassed(env):
    config, database, repo = setup(env)
    adapter = FakeAdapter("Programmers", "programmers", error=LoginRequired("로그아웃 상태"))
    report = engine.cycle(config, database, repo, adapters=factory(adapter), force_poll=True)
    assert "로그아웃" in report.site_errors["Programmers"]
    assert database.get("site_status:Programmers").startswith("로그인 필요")


# Git safety ---------------------------------------------------------------
def test_commit_contains_only_agent_files_and_user_index_is_kept(env):
    config, database, repo = setup(env, mode="commit_only")
    env.write("notes.txt", "user work\n")
    env.git("add", "notes.txt")
    env.write("BOJ/Silver/1000_A+B_CPP.cpp", "int main(){ /* user edit */ }\n")  # unstaged user edit
    item = submission(key="1", problem="42748", title="K번째수")
    engine.cycle(config, database, repo, adapters=factory(FakeAdapter("Programmers", "programmers", [item])),
                 force_poll=True)
    changed = env.git("show", "--name-only", "--format=", "HEAD").splitlines()
    assert sorted(changed) == ["Programmers/Lv1/42748_K번째수_Java.java", "Programmers/README.md",
                               "Programmers/metadata.json", "README.md"]
    status = git(env.root, "status", "--porcelain", "--untracked-files=all").splitlines()
    status = [line.strip() for line in status]
    assert "A  notes.txt" in status                       # user's staged file still staged
    assert "M BOJ/Silver/1000_A+B_CPP.cpp" in status      # user's edit untouched (unstaged)
    assert git(env.root, "diff", "--cached", "--name-only") == "notes.txt"
    assert not any("42748" in line for line in status)    # real index matches the agent commit
    assert not any("README.md" in line and "Programmers" not in line for line in status)
    assert "Algo-Sync: auto" in env.git("log", "-1", "--format=%B")


def test_readme_counts_follow_existing_rules(env):
    config, database, repo = setup(env)
    before = env.root.joinpath("README.md").read_text(encoding="utf-8")
    env.write("BOJ/Silver/2839_설탕_배달_CPP.cpp", "int main(){}\n")
    scanner.scan(config, database, repo)
    database.upsert_file("BOJ/Silver/2839_설탕_배달_CPP.cpp", status=dbm.ACCEPTED, verification_method="manual",
                         accepted_at=dbm.now())
    Publisher(config, database, repo).run(force=True)
    after = env.remote_show("README.md")
    assert stats_total(after, "BOJ") == stats_total(before, "BOJ") + 1
    assert stats_total(after, "SWEA") == stats_total(before, "SWEA")


def test_readme_with_user_edits_postpones_commit(env):
    config, database, repo = setup(env)
    env.write("README.md", env.root.joinpath("README.md").read_text(encoding="utf-8") + "\nmy note\n")
    database.upsert_file("BOJ/Silver/9_x_CPP.cpp", status=dbm.ACCEPTED, raw_hash="x", accepted_at=dbm.now())
    env.write("BOJ/Silver/9_x_CPP.cpp", "int main(){}\n")
    with pytest.raises(RetryLater):
        Publisher(config, database, repo).commit()
    assert env.root.joinpath("README.md").read_text(encoding="utf-8").endswith("my note\n")


def test_remote_moved_is_rebased_safely(env):
    config, database, repo = setup(env)
    other = env.other_clone()
    (other / "unrelated.txt").write_text("from elsewhere\n", encoding="utf-8")
    git(other, "add", "unrelated.txt")
    git(other, "commit", "-q", "-m", "remote change")
    git(other, "push", "-q", "origin", "main")
    item = submission(key="5", problem="42747", title="H-Index")
    engine.cycle(config, database, repo, adapters=factory(FakeAdapter("Programmers", "programmers", [item])),
                 force_poll=True)
    log = env.remote_log()
    assert log[0].startswith("sync: Programmers 42747") and "remote change" in log
    assert database.file("Programmers/Lv1/42747_H-Index_Java.java")["status"] == dbm.SYNCED


def test_conflicting_remote_aborts_rebase_and_keeps_work(env):
    config, database, repo = setup(env, mode="commit_only")
    item = submission(key="6", problem="42746", title="가장 큰 수", code="class Solution { /* local */ }\n")
    engine.cycle(config, database, repo, adapters=factory(FakeAdapter("Programmers", "programmers", [item])),
                 force_poll=True)
    local_commit = env.git("rev-parse", "HEAD")
    other = env.other_clone()
    path = other / "Programmers/Lv1/42746_가장_큰_수_Java.java"
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("class Solution { /* other */ }\n", encoding="utf-8")
    git(other, "add", "-A")
    git(other, "commit", "-q", "-m", "conflicting")
    git(other, "push", "-q", "origin", "main")
    with pytest.raises(RetryLater, match="충돌"):
        Publisher(config, database, repo).push()
    assert repo.operation_in_progress() is None
    assert env.git("rev-parse", "HEAD") == local_commit
    assert "local" in (env.root / "Programmers/Lv1/42746_가장_큰_수_Java.java").read_text(encoding="utf-8")


def test_user_commits_are_not_pushed_automatically(env):
    config, database, repo = setup(env)
    env.write("mine.txt", "x\n")
    env.git("add", "mine.txt")
    env.git("commit", "-q", "-m", "my own commit")
    from algo import hashing

    data = b"int main(){}\n"
    env.write("BOJ/Silver/9_x_CPP.cpp", data)
    database.upsert_file("BOJ/Silver/9_x_CPP.cpp", status=dbm.ACCEPTED, raw_hash=hashing.raw_hash(data),
                         normalized_hash=hashing.normalized_hash(data), accepted_at=dbm.now())
    with pytest.raises(RetryLater, match="직접 만든 커밋"):
        Publisher(config, database, repo).run(force=True)
    assert "my own commit" not in env.remote_log()


def test_identical_untracked_file_does_not_block_cloud_commits(env):
    config, database, repo = setup(env)
    other = env.other_clone()
    target = other / "LeetCode/Medium/48_Rotate_Image_CPP.cpp"
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes(b"class Solution{};\n")
    git(other, "add", "-A")
    git(other, "commit", "-q", "-m", "sync: update LeetCode solutions")
    git(other, "push", "-q", "origin", "main")
    env.write("LeetCode/Medium/48_Rotate_Image_CPP.cpp", b"class Solution{};\r\n")  # same code from IDE
    engine.cycle(config, database, repo, adapters=factory())
    assert env.git("rev-parse", "HEAD") == git(env.remote, "rev-parse", "main")
    assert list((config.state_dir / "backup").rglob("48_Rotate_Image_CPP.cpp"))


def test_forbidden_git_operations_are_blocked(env):
    repo = env.repo()
    for args in (("push", "--force", "origin", "main"), ("push", "origin", "+main"), ("reset", "--hard"),
                 ("push", "upstream", "main"), ("clean", "-fd"), ("checkout", "--", "."), ("push", "-f", "origin")):
        with pytest.raises(ForbiddenGitOperation):
            repo.git(*args)


# migration ------------------------------------------------------------------
def test_metadata_migration_registers_history_and_flags_mismatch(env):
    from algo import hashing

    code = b"class Solution {}\n"
    meta = [
        {"id": "12928", "title": "약수의 합", "level": "Lv1", "language": "Java",
         "file": "Lv1/12928_약수의_합_Java.java", "submissionId": 100, "sha256": hashing.raw_hash(code)},
        {"id": "1", "title": "x", "level": "Lv1", "language": "Java",
         "file": "Lv1/1_x_Java.java", "submissionId": 101, "sha256": "0" * 64},
    ]
    env.write("Programmers/metadata.json", json.dumps(meta, ensure_ascii=False))
    env.write("Programmers/Lv1/1_x_Java.java", "class Edited {}\n")
    env.git("add", "-A")
    env.git("commit", "-q", "--no-verify", "-m", "seed")
    config, database = env.config(), env.db()
    first = migrate.baseline(config, database, env.repo())
    again = migrate.baseline(config, database, env.repo())
    assert first.metadata_submissions == 2 and again.metadata_submissions == 0
    assert first.hash_mismatch == ["Programmers/Lv1/1_x_Java.java"]
    # an already-migrated submission is not processed again
    assert ingest.ingest(config, database, submission(key="100", problem="12928", title="약수의 합", code=code.decode())) == "seen"
    # the mismatching one surfaces as a decision instead of overwriting
    assert ingest.ingest(config, database, submission(key="101", problem="1", title="x")) == "candidate"
    assert (env.root / "Programmers/Lv1/1_x_Java.java").read_text(encoding="utf-8") == "class Edited {}\n"
