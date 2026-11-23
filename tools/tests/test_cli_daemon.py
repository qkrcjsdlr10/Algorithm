from __future__ import annotations

import json
import os

import pytest

from algo import cli, daemon
from algo import db as dbm


def run(env, *args):
    return cli.main(["--root", str(env.root), *args])


def test_manual_approve_uploads_and_records_method(env, capsys):
    (env.root / "algo.toml").write_text('[general]\nexpected_remote = ""\n[publish]\nmode = "manual"\n', encoding="utf-8")
    env.git("add", "algo.toml")
    env.git("commit", "-q", "--no-verify", "-m", "config")
    env.git("push", "-q", "origin", "main")
    assert run(env, "init") == 0
    env.write("BOJ/Gold/1753_최단경로_CPP.cpp", "int main(){}\n")
    assert run(env, "sync", "--no-publish") == 0
    database = env.db()
    row = database.file("BOJ/Gold/1753_최단경로_CPP.cpp")
    assert row["status"] == dbm.PENDING
    before = env.remote_log()
    assert run(env, "approve", str(row["id"]), "--yes") == 0
    row = env.db().file("BOJ/Gold/1753_최단경로_CPP.cpp")
    assert row["status"] == dbm.SYNCED and row["verification_method"] == "manual"
    assert len(env.remote_log()) == len(before) + 1


def test_approve_refuses_unidentified_and_incomplete(env):
    run(env, "init")
    env.write("BOJ/Gold/최단경로.cpp", "int main(){}\n")
    env.write("BOJ/Gold/15683_감시_미완성_CPP.cpp", "int main(){}\n")
    run(env, "sync", "--no-publish")
    with pytest.raises(SystemExit, match="문제 번호"):
        run(env, "approve", "BOJ/Gold/최단경로.cpp", "--yes")
    assert env.db().file("BOJ/Gold/15683_감시_미완성_CPP.cpp")["status"] == dbm.SKIPPED


def test_external_file_requires_explicit_identity(env, tmp_path):
    outside = tmp_path / "workspace"
    outside.mkdir()
    (outside / "Main.java").write_text("class Main {}\n", encoding="utf-8")
    (env.root / ".algo").mkdir(exist_ok=True)
    (env.root / ".algo" / "algo.local.toml").write_text(
        f'[general]\nexpected_remote = ""\nextra_watch_dirs = ["{outside.as_posix()}"]\n[publish]\nmode = "manual"\n',
        encoding="utf-8")
    run(env, "init")
    run(env, "sync", "--no-publish")
    key = f"external:{(outside / 'Main.java').resolve().as_posix()}"
    row = env.db().file(key)
    assert row["status"] == dbm.PENDING
    with pytest.raises(SystemExit, match="--site"):
        run(env, "approve", str(row["id"]), "--yes")
    assert run(env, "approve", str(row["id"]), "--site", "boj", "--tier", "Silver", "--problem", "2557",
               "--title", "Hello World", "--yes") == 0
    assert "BOJ/Silver/2557_Hello_World_Java.java" in env.remote_files()


def test_instance_lock_recovers_stale_pid(tmp_path):
    lock = daemon.InstanceLock(tmp_path / "daemon.lock")
    (tmp_path / "daemon.lock").write_text(json.dumps({"pid": 999_999_9}), encoding="utf-8")
    lock.acquire()  # stale lock from a dead process is cleaned up
    assert json.loads((tmp_path / "daemon.lock").read_text())["pid"] == os.getpid()
    lock.release()
    assert not (tmp_path / "daemon.lock").exists()
    assert daemon.pid_alive(os.getpid())
    assert not daemon.pid_alive(999_999_9)


def test_status_and_pending_run(env, capsys):
    run(env, "init")
    assert run(env, "status") == 0
    assert run(env, "pending") == 0
    out = capsys.readouterr().out
    assert "LeetCode" in out and "BOJ" in out
