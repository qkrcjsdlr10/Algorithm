"""Adapters reuse the real backup scripts; only the network is mocked."""

from __future__ import annotations

import json

import httpx
import pytest

from algo import db as dbm
from algo import migrate
from algo.adapters.base import AccessLimited, FetchContext, LoginRequired
from algo.adapters.leetcode import LeetCodeAdapter, make_http_runner
from algo.adapters.programmers import ProgrammersAdapter
from algo.adapters.scripts import load_script
from algo.adapters.swea import SweaAdapter

lb = load_script("leetcode_backup")


def leetcode_transport(code_by_id: dict[int, tuple[str, str]], questions: list[dict], user="nachal123",
                       status: int = 200):
    calls: list[str] = []

    def handler(request: httpx.Request) -> httpx.Response:
        body = json.loads(request.content)
        operation = body["operationName"]
        calls.append(operation)
        assert request.headers["x-csrftoken"] == "csrf"
        assert "LEETCODE_SESSION=sess" in request.headers["cookie"]
        if status != 200:
            return httpx.Response(status, text="Too Many Requests")
        if operation == "globalData":
            return httpx.Response(200, json={"data": {"userStatus": {"isSignedIn": True, "username": user}}})
        if operation == "userProgressQuestionList":
            return httpx.Response(200, json={"data": {"userProgressQuestionList": {
                "totalNum": len(questions), "questions": questions}}})
        if operation == "submissionList":
            slug = body["variables"]["questionSlug"]
            subs = [{"id": str(sid), "statusDisplay": "Accepted", "langName": lang, "timestamp": "1790000000"}
                    for sid, (s, lang) in code_by_id.items() if s.startswith(slug)]
            return httpx.Response(200, json={"data": {"questionSubmissionList": {"hasNext": False, "submissions": subs}}})
        if operation == "submissionDetails":
            sid = body["variables"]["submissionId"]
            slug, lang = code_by_id[sid]
            return httpx.Response(200, json={"data": {"submissionDetails": {
                "code": f"// {slug} {lang}\n", "statusCode": 10, "timestamp": 1790000000,
                "user": {"username": user}}}})
        raise AssertionError(operation)

    return httpx.MockTransport(handler), calls


def http_adapter(tmp_path, transport) -> LeetCodeAdapter:
    runner = make_http_runner(lb, tmp_path, 0.5, "sess", "csrf")
    runner.client = httpx.Client(transport=transport, headers=runner.client.headers, cookies=runner.client.cookies)
    runner.wait = lambda: None
    return LeetCodeAdapter(runner, lb)


def question(fid, slug, title, difficulty="Medium", last="1790000000"):
    return {"frontendId": str(fid), "titleSlug": slug, "title": title, "difficulty": difficulty,
            "questionStatus": "SOLVED", "lastResult": "AC", "lastSubmittedAt": last}


def ctx(**kw) -> FetchContext:
    base = dict(known_submission_keys=set(), known_problem_ids=set(), pending_problem_ids=set(), max_problems=20)
    base.update(kw)
    return FetchContext(**base)


def test_leetcode_http_reuses_backup_queries(tmp_path):
    transport, calls = leetcode_transport(
        {11: ("two-sum", "Java"), 12: ("two-sum", "C++"), 21: ("department-top-three-salaries", "MySQL")},
        [question(1, "two-sum", "Two Sum", "Easy"),
         question(185, "department-top-three-salaries", "Department Top Three Salaries", "Hard")])
    items = list(http_adapter(tmp_path, transport).fetch(ctx()))
    assert {(i.problem_id, i.language, i.extension) for i in items} == {
        ("1", "Java", ".java"), ("1", "CPP", ".cpp"), ("185", "MySQL", ".sql")}
    assert calls[:2] == ["globalData", "userProgressQuestionList"]


def test_leetcode_skips_known_and_unchanged(tmp_path):
    transport, calls = leetcode_transport({11: ("two-sum", "Java")}, [question(1, "two-sum", "Two Sum", "Easy")])
    items = list(http_adapter(tmp_path, transport).fetch(
        ctx(known_problem_ids={"1"}, latest_known_at={"1": "2026-09-22T00:00:00+09:00"}, known_submission_keys={"11"})))
    assert items == [] and "submissionList" not in calls


def test_leetcode_wrong_account_and_rate_limit_stop(tmp_path):
    transport, _ = leetcode_transport({}, [], user="someone-else")
    with pytest.raises(LoginRequired):
        list(http_adapter(tmp_path, transport).fetch(ctx()))
    transport, _ = leetcode_transport({}, [], status=429)
    with pytest.raises(AccessLimited):
        list(http_adapter(tmp_path, transport).fetch(ctx()))


def test_cloud_sync_writes_new_files_without_overwriting(env, monkeypatch):
    from algo import cloud
    from algo.adapters import leetcode as lc

    transport, _ = leetcode_transport(
        {31: ("longest-substring", "Java"), 32: ("department-top-three-salaries", "MySQL")},
        [question(3, "longest-substring", "Longest Substring"),
         question(185, "department-top-three-salaries", "Department Top Three Salaries", "Hard")])
    original = lc.LeetCodeAdapter.over_http

    def fake_over_http(cls, delay, output):
        return http_adapter(env.root, transport)

    monkeypatch.setattr(lc.LeetCodeAdapter, "over_http", classmethod(fake_over_http))
    before = (env.root / "LeetCode/Medium/3_Longest_Substring_Java.java").read_bytes()
    assert cloud.sync_leetcode(env.config()) == 0
    assert (env.root / "LeetCode/Medium/3_Longest_Substring_Java.java").read_bytes() == before
    sql = env.root / "LeetCode/SQL/Hard/185_Department_Top_Three_Salaries_MySQL.sql"
    assert sql.read_text(encoding="utf-8") == "// department-top-three-salaries MySQL\n"
    meta = json.loads((env.root / "LeetCode/metadata.json").read_text(encoding="utf-8"))
    assert [m["file"] for m in meta] == ["SQL/Hard/185_Department_Top_Three_Salaries_MySQL.sql"]
    monkeypatch.setattr(lc.LeetCodeAdapter, "over_http", original)


def test_cloud_sync_without_secrets_is_a_notice(env, monkeypatch, capsys):
    from algo import cloud

    monkeypatch.delenv("LEETCODE_SESSION", raising=False)
    monkeypatch.delenv("LEETCODE_CSRFTOKEN", raising=False)
    assert cloud.sync_leetcode(env.config()) == 0
    assert "건너뜁니다" in capsys.readouterr().out


class FakeProgrammersRunner:
    """Duck-types programmers_backup.BackupRunner methods the adapter calls."""

    def __init__(self):
        self.visited = []

    def verify_login(self):
        pass

    def load_solved_problems(self, limit):
        return [{"id": 1, "title": "old", "level": 1}, {"id": 59034, "title": "모든 레코드 조회하기", "level": 1},
                {"id": 42840, "title": "모의고사", "level": 1}]

    def wait(self):
        pass

    def navigate(self, url):
        self.visited.append(url)

    def load_submissions(self, problem_id):
        if problem_id == "59034":
            return [{"id": 9, "language": "mysql", "code": "SELECT * FROM ANIMAL_INS;", "score": 100,
                     "perfectScore": 100, "createdAt": "2026-11-23T10:00:00+09:00"}]
        return [{"id": 8, "language": "java", "code": "class Solution {}", "score": 100, "perfectScore": 100,
                 "createdAt": "2026-11-23T09:00:00+09:00"},
                {"id": 7, "language": "java", "code": "wrong", "score": 50, "perfectScore": 100,
                 "createdAt": "2026-11-23T08:00:00+09:00"}]

    latest_accepted_by_language = staticmethod(load_script("programmers_backup").BackupRunner.latest_accepted_by_language)


def test_programmers_adapter_only_checks_unknown_or_pending_problems():
    pb = load_script("programmers_backup")
    runner = FakeProgrammersRunner()
    items = list(ProgrammersAdapter(runner, pb).fetch(ctx(known_problem_ids={"1", "42840"},
                                                          pending_problem_ids={"42840"})))
    assert len(runner.visited) == 2  # problem 1 skipped
    by_id = {i.problem_id: i for i in items}
    assert by_id["59034"].extension == ".sql" and by_id["59034"].language == "MySQL"
    assert by_id["42840"].code == "class Solution {}"  # the 50-point submission is ignored


class FakeSweaRunner:
    def collect_cards(self, missing_only, limit):
        return [{"id": "1206", "title": "View", "level": "D3", "contestProbId": "AV1"},
                {"id": "1206", "title": "View", "level": "D3", "contestProbId": "AV1"},
                {"id": "5643", "title": "키 순서", "level": "D4", "contestProbId": "AV2"}]

    def latest_pass_submissions(self, contest_prob_id):
        return {"java": {"language": "Java", "extension": ".java", "submittedAt": "2026-11-23 10:00",
                         "result": "Accepted", "siteResult": "Pass", "contestHistoryId": f"H-{contest_prob_id}"}}

    def fetch_code(self, contest_prob_id, history_id):
        return f"// {history_id}\n"


def test_swea_adapter_uses_history_id_as_submission_key():
    sb = load_script("swea_backup")
    items = list(SweaAdapter(FakeSweaRunner(), sb).fetch(ctx(known_problem_ids={"1206"})))
    assert [(i.problem_id, i.submission_key, i.tier) for i in items] == [("5643", "H-AV2", "D4")]


def test_programmers_ingest_routes_sql_and_regenerates_listing(env):
    from algo import ingest

    config, database = env.config(), env.db()
    migrate.baseline(config, database, env.repo())
    pb = load_script("programmers_backup")
    item = next(i for i in ProgrammersAdapter(FakeProgrammersRunner(), pb).fetch(ctx(known_problem_ids={"1", "42840"}))
                if i.problem_id == "59034")
    assert ingest.ingest(config, database, item) == "new"
    assert (env.root / "Programmers/SQL/Lv1/59034_모든_레코드_조회하기_MySQL.sql").is_file()
    listing = (env.root / "Programmers/README.md").read_text(encoding="utf-8")
    assert "SQL/Lv1/59034_모든_레코드_조회하기_MySQL.sql" in listing
    assert database.file("Programmers/SQL/Lv1/59034_모든_레코드_조회하기_MySQL.sql")["status"] == dbm.ACCEPTED
