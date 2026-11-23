"""``algo`` command line interface."""

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path

from algo import db as dbm
from algo import hashing, layout
from algo.config import Config, find_root, load
from algo.repo import Repo

if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")
    sys.stderr.reconfigure(encoding="utf-8")


def _open(args: argparse.Namespace) -> tuple[Config, dbm.Database, Repo]:
    root = Path(args.root).resolve() if args.root else find_root()
    config = load(root)
    return config, dbm.Database(config.db_path), Repo(root, config.remote)


def _ensure_baseline(config: Config, database: dbm.Database, repo: Repo) -> None:
    if database.get("baseline_at") is None:
        from algo import migrate

        report = migrate.baseline(config, database, repo)
        print(f"[init] 기존 풀이 {report.tracked_files}개를 기준점으로 등록했습니다 (업로드하지 않음)")


def _find_rows(database: dbm.Database, targets: list[str]) -> list:
    rows = []
    for target in targets:
        row = database.file_by_id(int(target)) if target.isdigit() else database.file(target.replace("\\", "/"))
        if row is None and not target.isdigit():
            row = database.file(f"external:{Path(target).resolve().as_posix()}")
        if row is None:
            raise SystemExit(f"대상을 찾지 못했습니다: {target} (algo pending으로 ID를 확인하세요)")
        rows.append(row)
    return rows


# -- commands ---------------------------------------------------------------
def cmd_init(args: argparse.Namespace) -> int:
    from algo import ingest, migrate, scanner

    config, database, repo = _open(args)
    report = migrate.baseline(config, database, repo)
    found = scanner.scan(config, database, repo)
    ingest.ingest_metadata(config, database)
    verified = ingest.verify_pending(config, database)
    print(f"추적 중인 풀이 파일: {report.tracked_files}개 (이번에 새로 등록 {report.newly_registered}개)")
    print(f"metadata 제출 기록: 새로 등록 {report.metadata_submissions}건, 파일 연결 {report.linked}건")
    if report.hash_mismatch:
        print(f"metadata와 내용이 다른 기존 파일 {len(report.hash_mismatch)}개 (변경하지 않음):")
        for path in report.hash_mismatch:
            print(f"  - {path}")
    if report.missing_files:
        print(f"metadata에는 있지만 파일이 없는 항목 {len(report.missing_files)}개:")
        for path in report.missing_files:
            print(f"  - {path}")
    print(f"현재 작업 트리의 새/수정 파일: {found['changed']}개 → algo pending 확인 (자동 확인 {verified}개)")
    return 0


def cmd_status(args: argparse.Namespace) -> int:
    from algo import browser, daemon, startup
    from algo.adapters import SITE_SUPPORT

    config, database, repo = _open(args)
    counts = database.counts()
    print("== 상태")
    print("  " + "  ".join(f"{k}={v}" for k, v in counts.items()))
    print(f"  업로드 정책: {config.publish_mode}"
          + (f" ({config.batch_minutes:g}분 묶음)" if config.publish_mode == "batch" else "")
          + f" / 중복 정책: {config.duplicate_policy}")
    pid = daemon.running_pid(config)
    print(f"  daemon: {'실행 중 (PID %s)' % pid if pid else '중지됨'} / 자동 시작: {startup.status()}")
    print(f"  Edge 전용 브라우저: {browser.check(config)}")
    error = database.get("last_publish_error")
    if error:
        print(f"  마지막 업로드 보류 사유: {error}")
    remote = database.get("last_remote_sync")
    if remote:
        print(f"  원격 동기화: {remote}")
    print(f"  Git: branch={repo.current_branch() or '?'} remote={repo.remote_url() or '?'} "
          f"hooks={'on' if repo.hooks_active() else 'off'}")
    print("== 사이트")
    names = {"LeetCode": "leetcode", "Programmers": "programmers", "SWEA": "swea", "BOJ": "boj"}
    for site, support in SITE_SUPPORT.items():
        mode = config.site(names[site]).mode if site in names else "actions"
        state = database.get(f"site_status:{site}") or "-"
        print(f"  {site:<12} mode={mode:<7} 제출목록={support['submission_list'].value:<11} "
              f"소스={support['source_code'].value:<11} 최근={state}")
        print(f"  {'':<12} {support['automation']}")
    return 0


def cmd_pending(args: argparse.Namespace) -> int:
    config, database, repo = _open(args)
    statuses = [dbm.PENDING, dbm.DISCOVERED, dbm.FAILED] + ([dbm.ACCEPTED, dbm.SKIPPED] if args.all else [dbm.ACCEPTED])
    rows = database.files_with_status(*statuses)
    if not rows:
        print("확인이 필요한 항목이 없습니다")
    for row in rows:
        target = row["external_source"] or row["path"]
        who = f"{row['site'] or '?'} {row['problem_id'] or '?'} {row['language'] or ''}".strip()
        state = row["status"] + (" (커밋됨, 푸시 대기)" if row["status"] == dbm.ACCEPTED and row["commit_sha"] else "")
        print(f"[{row['id']:>4}] {state:<34} {who:<24} {target}")
        if row["reason"]:
            print(f"       └ {row['reason']}")
    candidates = database.q("SELECT c.id, c.target_path, c.stored_path, s.submission_key, s.site FROM candidates c "
                            "JOIN submissions s ON s.id = c.submission_id WHERE c.status = 'OPEN'")
    if candidates:
        print("\n== 기존 파일과 다른 사이트 정답 코드 (approve --use-site C<ID> 로 교체)")
        for c in candidates:
            print(f"[C{c['id']}] {c['site']} 제출 {c['submission_key']} → {c['target_path']}\n       사이트 코드: {c['stored_path']}")
    return 0


def _confirm(prompt: str, assume_yes: bool) -> bool:
    if assume_yes:
        return True
    try:
        return input(f"{prompt} [y/N] ").strip().casefold() in {"y", "yes"}
    except EOFError:
        return False


def cmd_approve(args: argparse.Namespace) -> int:
    from algo.publisher import Publisher, RetryLater

    config, database, repo = _open(args)
    _ensure_baseline(config, database, repo)
    if args.use_site:
        return _approve_candidate(config, database, repo, args)
    rows = _find_rows(database, args.targets)
    for row in rows:
        relative = row["path"]
        if row["external_source"]:
            relative = _import_external(config, database, row, args)
        parsed, reason = layout.parse_path(relative)
        if parsed is None:
            raise SystemExit(f"{relative}: {reason}\n파일을 <사이트>/<난이도>/<번호>_<제목>_<언어>.<확장자>로 옮긴 뒤 다시 실행하세요.")
        if parsed.incomplete:
            raise SystemExit(f"{relative}: 미완성 파일은 승인할 수 없습니다")
        if row["status"] == dbm.SYNCED and row["path"] == relative:
            print(f"이미 업로드됨: {relative}")
            continue
        print(f"{parsed.site} {parsed.problem_id} ({parsed.language}) → {relative}")
        if not _confirm("사이트에서 이 코드가 정답 처리된 것을 직접 확인했습니까?", args.yes):
            print("취소했습니다")
            return 1
        data = (config.root / relative).read_bytes()
        solution = database.solution_id(parsed.site, parsed.problem_id, parsed.lang_key, relative)
        database.add_version(solution, hashing.raw_hash(data), hashing.normalized_hash(data), relative)
        database.upsert_file(relative, status=dbm.ACCEPTED, verification_method="manual", site=parsed.site,
                             problem_id=parsed.problem_id, lang_key=parsed.lang_key, language=parsed.language,
                             solution_id=solution, raw_hash=hashing.raw_hash(data),
                             normalized_hash=hashing.normalized_hash(data), accepted_at=dbm.now(), reason=None,
                             commit_sha=None)
        database.event("INFO", "사용자 수동 승인", site=parsed.site, path=relative)
    if args.no_push:
        print("승인했습니다. 정책에 따라 업로드됩니다")
        return 0
    try:
        result = Publisher(config, database, repo).run(force=True)
    except RetryLater as error:
        print(f"승인은 기록했지만 업로드는 보류했습니다: {error}\n원인을 해결한 뒤 algo retry 를 실행하세요")
        return 2
    print(f"커밋 {result.committed[:10] if result.committed else '-'} / 푸시 {'완료' if result.pushed else '안 함'}")
    return 0


def _import_external(config: Config, database: dbm.Database, row, args: argparse.Namespace) -> str:
    site = layout.site_name(args.site or "")
    if not (site and args.tier and args.problem and args.title):
        raise SystemExit("외부 파일은 --site --tier --problem --title 를 모두 지정해야 합니다 (추측하지 않습니다)")
    source = Path(row["external_source"])
    language = args.language or layout.EXTENSION_LANGUAGE.get(source.suffix.casefold(), "")
    relative = layout.build_path(site, args.tier, args.problem, args.title, language, source.suffix.casefold())
    target = config.root / relative
    if target.exists():
        raise SystemExit(f"이미 같은 경로의 파일이 있습니다: {relative} (덮어쓰지 않습니다)")
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    database.upsert_file(row["path"], status=dbm.SKIPPED, reason=f"저장소로 복사됨: {relative}")
    database.upsert_file(relative, status=dbm.DISCOVERED, raw_hash=row["raw_hash"],
                         normalized_hash=row["normalized_hash"])
    return relative


def _approve_candidate(config: Config, database: dbm.Database, repo: Repo, args: argparse.Namespace) -> int:
    from algo.publisher import Publisher, RetryLater

    candidate_id = int(str(args.use_site).lstrip("Cc"))
    row = database.one("SELECT * FROM candidates WHERE id = ? AND status = 'OPEN'", candidate_id)
    if row is None:
        raise SystemExit("열린 후보를 찾지 못했습니다 (algo pending)")
    target = config.root / row["target_path"]
    if repo.is_dirty_vs_head(row["target_path"]) and not args.yes:
        raise SystemExit(f"{row['target_path']}에 커밋되지 않은 수정이 있습니다. 먼저 정리하거나 --yes로 확인하세요")
    if not _confirm(f"{row['target_path']}를 사이트 정답 코드로 교체할까요? (이전 내용은 Git 기록에 남습니다)", args.yes):
        return 1
    data = Path(row["stored_path"]).read_bytes()
    target.write_bytes(data)
    parsed, _ = layout.parse_path(row["target_path"])
    database.upsert_file(row["target_path"], status=dbm.ACCEPTED, verification_method="manual-site-replace",
                         submission_id=row["submission_id"], raw_hash=hashing.raw_hash(data),
                         normalized_hash=hashing.normalized_hash(data), accepted_at=dbm.now(), reason=None,
                         commit_sha=None, site=parsed.site if parsed else None,
                         problem_id=parsed.problem_id if parsed else None,
                         lang_key=parsed.lang_key if parsed else None, language=parsed.language if parsed else None)
    with database.tx() as c:
        c.execute("UPDATE candidates SET status = 'APPLIED' WHERE id = ?", (candidate_id,))
    try:
        Publisher(config, database, repo).run(force=not args.no_push)
    except RetryLater as error:
        print(f"교체했지만 업로드는 보류했습니다: {error}")
        return 2
    return 0


def cmd_skip(args: argparse.Namespace) -> int:
    config, database, repo = _open(args)
    for row in _find_rows(database, args.targets):
        database.upsert_file(row["path"], status=dbm.SKIPPED, reason=args.reason or "사용자가 건너뜀")
        print(f"건너뜀: {row['path']}")
    return 0


def cmd_retry(args: argparse.Namespace) -> int:
    from algo import engine

    config, database, repo = _open(args)
    for row in database.files_with_status(dbm.FAILED):
        database.upsert_file(row["path"], status=dbm.PENDING, reason="재시도")
    for key in [r["key"] for r in database.q("SELECT key FROM kv WHERE key LIKE 'backoff_until:%'")]:
        database.set(key, None)
    report = engine.cycle(config, database, repo, force_poll=True, publish_mode="immediate"
                          if config.publish_mode in {"batch", "immediate"} else None)
    _print_report(report)
    return 0


def _print_report(report) -> None:
    print(f"감지 {report.scanned} / 자동 확인 {report.verified}건")
    for site, counts in report.fetched.items():
        print(f"  {site}: {counts or '새 제출 없음'}")
    for site, error in report.site_errors.items():
        print(f"  {site}: {error}")
    print(f"업로드: {report.publish or '대상 없음'}")


def cmd_sync(args: argparse.Namespace) -> int:
    from algo import engine

    config, database, repo = _open(args)
    _ensure_baseline(config, database, repo)
    report = engine.cycle(config, database, repo, only_site=args.site, force_poll=True,
                          publish=not args.no_publish)
    _print_report(report)
    return 0


def cmd_push(args: argparse.Namespace) -> int:
    from algo.publisher import Publisher, RetryLater

    config, database, repo = _open(args)
    try:
        result = Publisher(config, database, repo).run(mode="immediate", force=True)
    except RetryLater as error:
        print(f"보류: {error}")
        return 2
    print(f"커밋 {result.committed[:10] if result.committed else '-'} / 푸시 {'완료' if result.pushed else '할 것 없음'}")
    return 0


def cmd_daemon(args: argparse.Namespace) -> int:
    from algo import daemon

    root = Path(args.root).resolve() if args.root else find_root()
    return daemon.run(load(root), once=args.once)


def cmd_stop(args: argparse.Namespace) -> int:
    from algo import daemon

    root = Path(args.root).resolve() if args.root else find_root()
    print(daemon.request_stop(load(root)))
    return 0


def cmd_browser(args: argparse.Namespace) -> int:
    from algo import browser

    root = Path(args.root).resolve() if args.root else find_root()
    config = load(root)
    print({"start": browser.start, "stop": browser.stop, "check": browser.check}[args.action](config))
    return 0


def cmd_startup(args: argparse.Namespace) -> int:
    from algo import startup

    root = Path(args.root).resolve() if args.root else find_root()
    config = load(root)
    if args.action == "install":
        print(startup.install(config))
    elif args.action == "uninstall":
        print(startup.uninstall())
    else:
        print(" ".join(startup.command(config)))
    return 0


def cmd_log(args: argparse.Namespace) -> int:
    config, database, repo = _open(args)
    rows = database.q("SELECT * FROM events ORDER BY id DESC LIMIT ?", args.n)
    for row in reversed(rows):
        where = " ".join(x for x in (row["site"], row["path"]) if x)
        print(f"{row['at']} {row['level']:<5} {where} {row['message']}")
    print(f"\n상세 로그: {config.log_dir / 'algo.log'}")
    return 0


def cmd_cloud_sync(args: argparse.Namespace) -> int:
    from algo import cloud

    root = Path(args.root).resolve() if args.root else find_root()
    return cloud.sync_leetcode(load(root))


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog="algo", description="알고리즘 풀이 자동 수집·동기화")
    parser.add_argument("--root", help="저장소 경로 (기본: 현재 위치에서 탐색)")
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("init", help="기존 풀이를 기준점으로 등록 (반복 실행 안전)").set_defaults(func=cmd_init)
    sub.add_parser("status", help="전체 자동화 상태").set_defaults(func=cmd_status)
    p = sub.add_parser("pending", help="정답 확인·코드 수집이 필요한 항목")
    p.add_argument("--all", action="store_true", help="건너뛴 항목도 표시")
    p.set_defaults(func=cmd_pending)
    p = sub.add_parser("approve", help="직접 확인한 풀이를 승인하고 업로드")
    p.add_argument("targets", nargs="*", help="pending의 ID 또는 파일 경로")
    p.add_argument("--use-site", metavar="C<ID>", help="기존 파일을 사이트 정답 코드 후보로 교체")
    p.add_argument("--site")
    p.add_argument("--tier")
    p.add_argument("--problem")
    p.add_argument("--title")
    p.add_argument("--language")
    p.add_argument("--yes", "-y", action="store_true", help="확인 질문 생략")
    p.add_argument("--no-push", action="store_true", help="승인만 하고 업로드는 정책에 맡김")
    p.set_defaults(func=cmd_approve)
    p = sub.add_parser("skip", help="항목을 업로드하지 않음")
    p.add_argument("targets", nargs="+")
    p.add_argument("--reason")
    p.set_defaults(func=cmd_skip)
    sub.add_parser("retry", help="실패·보류 작업 재시도").set_defaults(func=cmd_retry)
    p = sub.add_parser("sync", help="지금 한 번 동기화")
    p.add_argument("--site", help="leetcode / programmers / swea")
    p.add_argument("--no-publish", action="store_true")
    p.set_defaults(func=cmd_sync)
    sub.add_parser("push", help="승인된 풀이를 지금 커밋·푸시").set_defaults(func=cmd_push)
    p = sub.add_parser("daemon", help="백그라운드 감시·동기화")
    p.add_argument("--once", action="store_true")
    p.set_defaults(func=cmd_daemon)
    sub.add_parser("stop", help="daemon 정상 종료").set_defaults(func=cmd_stop)
    p = sub.add_parser("browser", help="Edge 전용 프로필 (디버그 포트 127.0.0.1)")
    p.add_argument("action", choices=["start", "stop", "check"], nargs="?", default="start")
    p.set_defaults(func=cmd_browser)
    p = sub.add_parser("startup", help="로그인 시 자동 시작 (작업 스케줄러)")
    p.add_argument("action", choices=["install", "uninstall", "show"], nargs="?", default="show")
    p.set_defaults(func=cmd_startup)
    p = sub.add_parser("log", help="최근 이벤트")
    p.add_argument("-n", type=int, default=30)
    p.set_defaults(func=cmd_log)
    p = sub.add_parser("cloud-sync", help="(GitHub Actions) LeetCode 클라우드 동기화")
    p.add_argument("site", choices=["leetcode"])
    p.set_defaults(func=cmd_cloud_sync)
    return parser


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    if args.command == "approve" and not args.targets and not args.use_site:
        raise SystemExit("approve 할 ID 또는 파일을 지정하세요 (algo pending)")
    return int(args.func(args) or 0)
