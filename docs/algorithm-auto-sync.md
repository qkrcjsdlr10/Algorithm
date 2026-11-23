# 알고리즘 풀이 자동 수집·동기화 (ALGO auto-sync)

브라우저 확장 프로그램 없이, 웹사이트와 IDE에서 푼 풀이를 **정답이 확인된 것만** 이 저장소에 올리는 도구입니다.
기존 백업 스크립트(`scripts/*_backup.py`), README 집계(`scripts/update_stats.py`), pre-commit 훅, GitHub Actions를 그대로 재사용합니다.

## 1. 사이트별 자동화 수준

| 사이트 | 실행 위치 | 제출 목록·정답 | 소스코드 | 자동화 수준 |
|---|---|---|---|---|
| LeetCode | GitHub Actions (`sync-leetcode.yml`) | UNOFFICIAL | UNOFFICIAL | **PC 없이 자동** (세션 쿠키 필요) |
| 프로그래머스 | 로컬 에이전트 + Edge 전용 프로필 | UNOFFICIAL | UNOFFICIAL | **PC가 켜져 있을 때 자동** |
| SWEA | 로컬 에이전트 + Edge 전용 프로필 | UNOFFICIAL | UNOFFICIAL | **PC가 켜져 있을 때 자동** (느린 주기) |
| BOJ | 로컬 | UNKNOWN | UNSUPPORTED | **수동 승인** (`algo approve`) |
| CodeTree | GitHub Actions (`sync-codetree.yml`) | 사이트 자체 GitHub 연동 | 〃 | **이미 자동** (이 도구는 관여하지 않음) |

- **SUPPORTED** 공식 확인 / **UNOFFICIAL** 로그인한 사이트가 스스로 쓰는 요청을 본인 세션으로 호출 (이용 조건 검토 필요) / **UNSUPPORTED** 불가 / **UNKNOWN** 검증 안 됨.
- 5개 사이트 모두 **공식 공개 API로 제출 코드를 받는 방법은 없습니다.**
- 403·429·CAPTCHA가 보이면 **우회하지 않고 중단**하고 6시간 쉽니다. 로그아웃되면 자동 재로그인하지 않고 알려 줍니다.

### 한계
- 로컬 에이전트는 **PC가 꺼져 있거나 프로그램이 종료되어 있으면 동작하지 않습니다.** 사이트에 제출 기록이 남아 있으므로, 다음에 켰을 때 밀린 제출을 가져옵니다.
- 프로그래머스·SWEA는 "아직 저장소에 없는 문제"와 "로컬 파일이 확인을 기다리는 문제"만 조회합니다. 이미 있는 문제를 **웹에서 다른 언어로 다시 푼 경우**는 그 언어의 로컬 파일이 생겨야 확인합니다.
- BOJ는 정답 확인 방법(공개 채점 현황 조회)의 이용 조건을 확인하기 전까지 자동 조회를 하지 않습니다.
- GitHub는 60일 동안 저장소 활동이 없으면 예약 워크플로를 멈춥니다. Actions 탭에서 다시 켜면 됩니다.

## 2. 구조

```text
 IDE 저장 ──▶ scanner (git status, watchdog은 즉시 깨우기만) ──┐
                                                                ▼
 사이트 ──▶ adapters (기존 백업 스크립트의 조회 함수 재사용) ──▶ ingest ──▶ SQLite (.algo/state.sqlite3)
                                                                │  정답 코드와 일치 → ACCEPTED
                                                                │  불일치/불확실   → PENDING_VERIFICATION
                                                                ▼
                         publisher: 별도 index로 대상 파일만 커밋 → update_stats.py → origin 푸시
 GitHub Actions: sync-leetcode.yml / sync-codetree.yml (README까지 같은 커밋) / update-stats.yml (보완)
```

| 위치 | 내용 |
|---|---|
| `tools/algo/` | 에이전트 코드 (`cli`, `engine`, `scanner`, `ingest`, `publisher`, `adapters/`) |
| `tools/tests/` | pytest (임시 저장소 + 가짜 origin, 실제 사이트 호출 없음) |
| `algo.toml` | 설정 (비밀 정보 없음). PC별 설정은 `.algo/algo.local.toml` |
| `algo.cmd` | 실행기 (`.venv`가 있으면 사용) |
| `.algo/` | DB·로그·잠금·후보 코드·백업 (Git 제외) |

### 상태와 판정 기준
`DISCOVERED → PENDING_VERIFICATION → ACCEPTED → SYNCED` (그 밖에 `FAILED`, `SKIPPED`)

- **제출 중복**: `site + submission_key` (LeetCode·프로그래머스 `submissionId`, SWEA `contestHistoryId`)
- **풀이 중복**: `site + problem_id + 언어 계열(확장자)` → Java와 C++는 따로 보존
- **코드 버전**: 원본 SHA-256 + 보수적 정규화 해시
- **자동 승인 조건**: 사이트의 정답(Accepted/Pass/만점) 코드와 **원본이 같거나**, 다음 차이만 있을 때
  - 줄바꿈 CRLF/LF, 맨 앞 UTF-8 BOM, 파일 끝 빈 줄 개수
  - 공백·탭·줄 끝 공백은 비교에 그대로 포함합니다 (문자열 리터럴·raw string 보호).
- 그 밖의 경우는 모두 `PENDING_VERIFICATION`. **파일 저장만으로 정답 처리하지 않습니다.**
- 파일명으로 사이트·문제 번호를 확정할 수 없으면 추측하지 않고 확인을 요청합니다.

### 중복·수정본 정책 (`[duplicates] policy`)
| 값 | 동작 |
|---|---|
| `ask` (기본) | 기존 파일 유지, 사이트 코드는 `.algo/candidates/`에 저장 → `algo approve --use-site C<ID>`로 교체 |
| `keep` | 기존 파일 유지 |
| `add_as_new` | `..._v2.java`처럼 새 이름으로 추가 (README에서는 같은 문제로 1번만 셈) |
| `ignore` | 해당 제출 무시 |

## 3. 설치 및 최초 실행 (Windows 11)

```powershell
cd C:\SSAFY\Algorithm
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r tools\requirements.txt
git config core.hooksPath .githooks
.\algo init
.\algo status
```

`algo init`은 저장소에 있는 풀이(547개)와 metadata 제출 기록(392건)을 **이미 업로드된 것으로 등록**합니다. 처음 실행해도 과거 풀이를 다시 올리지 않으며, 여러 번 실행해도 결과가 같습니다.

### 프로그래머스·SWEA 로그인 (Edge 전용 프로필)
```powershell
.\algo browser
```
- `%LOCALAPPDATA%\algo-sync\edge-profile`에 **전용 프로필**을 만들고 디버그 포트를 `127.0.0.1:9222`로 엽니다.
- 일반 Edge 프로필과 저장소 안 경로는 거부합니다. 외부 주소에 포트가 열리면 즉시 닫습니다.
- 열린 창에서 **직접 로그인**하세요. 비밀번호·쿠키를 파일로 저장하지 않습니다.
- 포트가 열려 있는 동안에는 같은 PC의 다른 프로그램도 이 브라우저에 접근할 수 있으므로, 쓰지 않을 때는 닫으세요: `.\algo browser stop`
- 상태 확인: `.\algo browser check`

### LeetCode (GitHub Actions, PC 불필요)
1. leetcode.com에 로그인한 브라우저에서 개발자 도구 → Application → Cookies → `https://leetcode.com`
2. `LEETCODE_SESSION`, `csrftoken` 값을 복사
3. GitHub 저장소 → Settings → Secrets and variables → Actions → New repository secret
   - `LEETCODE_SESSION` = LEETCODE_SESSION 값
   - `LEETCODE_CSRFTOKEN` = csrftoken 값
4. Actions → "Sync LeetCode solutions" → Run workflow로 한 번 실행해 확인

- 쿠키는 계정 접근 권한과 같습니다. Secrets에만 넣고 다른 곳에 붙여 넣지 마세요.
- 만료되면 워크플로가 실패하며 알림이 옵니다. 새 쿠키로 바꿔 넣으면 됩니다.
- Secrets가 없으면 워크플로는 안내만 남기고 아무것도 바꾸지 않습니다.
- 로컬에서 LeetCode를 조회하려면 `algo.toml`에서 `[sites.leetcode] mode = "local"`로 바꿉니다 (클라우드와 동시에 쓰지 마세요).

### 로그인 시 자동 시작 (선택)
```powershell
.\algo startup show
```
```powershell
.\algo startup install
```
제거: `.\algo startup uninstall`. 작업 스케줄러에 `ALGO Auto Sync`(로그인 시 `pythonw ... daemon`)를 등록합니다.

## 4. 사용법

| 명령 | 설명 |
|---|---|
| `algo daemon` | 파일 감시 + 사이트 주기 조회 + 정책에 따른 업로드 (Ctrl+C 또는 `algo stop`으로 종료) |
| `algo sync [--site programmers]` | 지금 한 번 동기화 |
| `algo status` | 전체 상태, 사이트별 최근 결과, 브라우저·daemon·훅 상태 |
| `algo pending` | 정답 확인이나 결정이 필요한 항목 (+ 사이트 코드 후보) |
| `algo approve <ID\|경로> [--yes]` | 직접 정답을 확인한 풀이를 승인하고 바로 커밋·푸시 (`verification_method=manual`로 구분 기록) |
| `algo approve --use-site C<ID>` | 기존 파일을 사이트 정답 코드로 교체 (이전 내용은 Git 기록에 남음) |
| `algo skip <ID>` | 올리지 않음 |
| `algo retry` | 실패·보류 항목 재시도 (대기 시간 초기화) |
| `algo push` | 승인된 풀이를 지금 커밋·푸시 |
| `algo log` | 최근 이벤트 |

### 웹사이트에서 직접 푼 경우
아무것도 하지 않아도 됩니다. LeetCode는 1시간 안에 Actions가, 프로그래머스(20분)·SWEA(60분)는 daemon이 정답 코드를 가져와 `<사이트>/<난이도>/<번호>_<제목>_<언어>.<확장자>`로 저장하고 업로드합니다. SQL 문제는 `Programmers/SQL/Lv*`, `LeetCode/SQL/*`에 저장됩니다.

### IntelliJ / VS Code에서 푼 경우
1. 기존 규칙대로 저장소 안에 파일을 만듭니다: `BOJ/Silver/2839_설탕_배달_CPP.cpp`
2. 사이트에 제출합니다.
3. 결과:
   - **프로그래머스·SWEA·LeetCode**: 사이트의 정답 코드와 같으면 자동으로 `ACCEPTED` → 업로드
   - **BOJ**: `algo pending`에 나타나면 `algo approve <ID>`
   - 코드가 다르면(제출 후 로컬에서 수정 등) 대기 상태로 남고 이유가 표시됩니다.

저장소 밖 폴더에서 푼다면 `algo.toml`의 `extra_watch_dirs`에 추가하고, `algo approve <ID> --site boj --tier Silver --problem 2839 --title "설탕 배달"`로 정해 주면 규칙에 맞는 이름으로 복사해 올립니다.

### 업로드 정책 (`[publish] mode`)
| 값 | 동작 |
|---|---|
| `batch` (기본) | 첫 정답 확인 후 `batch_minutes`(3분) 동안 모아서 한 번에 커밋·푸시 |
| `immediate` | 확인 즉시 커밋·푸시 |
| `commit_only` | 자동 커밋만, 푸시는 직접 |
| `manual` | 자동 커밋·푸시 없음. `algo push` 또는 `algo approve`로만 |

## 5. README 자동 갱신
- 집계 규칙은 바꾸지 않았습니다. 모든 경로가 `scripts/update_stats.py` 하나를 씁니다.
- **로컬 에이전트**: 별도 index에서 `update_stats.py --staged`를 실행하고 README를 같은 커밋에 넣습니다. 기존 pre-commit 훅도 그대로 실행됩니다.
- **sync-leetcode.yml / sync-codetree.yml**: `GITHUB_TOKEN`으로 만든 커밋은 다른 워크플로를 다시 실행하지 않습니다. 그래서 두 워크플로 모두 README를 직접 다시 계산해 같은 커밋에 넣습니다.
- **update-stats.yml**: 사람이 푸시한 커밋에 대해 누락분을 보완합니다. 봇 커밋으로는 다시 실행되지 않으므로 무한 반복이 없습니다.
- 워크플로마다 `concurrency`가 있고, 푸시 직전에 `pull --rebase`를 해서 동시에 실행돼도 충돌하지 않습니다.

## 6. Git 안전장치
- 자동 커밋은 **별도 index(`.git/algo-index`)** 를 씁니다. 순서는 HEAD로 초기화 → 대상 파일만 추가 → README 집계 → 포함 파일 목록 검사 → 커밋 → 실제 index의 해당 경로만 HEAD에 맞춤입니다. 사용자가 스테이징해 둔 파일이나 수정 중인 파일은 건드리지 않습니다.
- README에 커밋되지 않은 수정이 있으면 덮어쓰지 않도록 업로드를 미룹니다.
- 푸시는 `origin`(주소 `expected_remote` 확인)의 `main`에만 하고, `Algo-Sync: auto` 표시가 있는 자동 커밋만 올립니다. **직접 만든 커밋이 아직 푸시되지 않았다면 자동 푸시를 멈춥니다.**
- 원격이 앞서 있으면 작업 트리에 수정이 없을 때만 받아옵니다(fast-forward, 또는 자동 커밋만 rebase). 충돌하면 `rebase --abort`로 되돌리고 나중에 다시 시도합니다.
- 코드에서 막아 둔 명령: `push --force`, `+refspec`, `reset --hard`, `clean`, `checkout`·`restore`, origin이 아닌 곳으로의 푸시.
- 받아올 파일과 같은 이름의 미추적 파일이 있으면, 내용이 같을 때만 `.algo/backup/`으로 옮기고 받습니다. 다르면 멈추고 알려 줍니다.

### 충돌 해결
1. `algo status`에서 "업로드 보류 사유"를 확인합니다.
2. 흔한 원인과 조치:
   - **작업 트리에 수정 중인 파일**: 커밋하거나 정리한 뒤 `algo retry`
   - **직접 만든 커밋**: 직접 `git pull --rebase` 후 `git push`. 그 뒤 에이전트가 이어서 진행합니다.
   - **원격에 같은 이름의 다른 파일**: 두 내용을 비교해 하나를 남기고 커밋한 뒤 `algo retry`
3. 에이전트는 충돌한 상태로 저장소를 남겨 두지 않습니다. `git status`에 rebase 진행 중 표시가 있다면 직접 진행 중이던 작업입니다.

## 7. 로그
- `algo log`: 최근 이벤트 (감지, 확인, 커밋, 보류, 사이트 오류)
- `.algo/logs/algo.log`: daemon 상세 로그 (1MB × 5개 순환)
- GitHub Actions 로그: 저장소 Actions 탭

## 8. 문제 해결
| 증상 | 조치 |
|---|---|
| `Edge 전용 브라우저가 실행 중이 아닙니다` | `algo browser` 후 로그인 |
| `로그인 필요` | 전용 Edge 창에서 다시 로그인 → `algo retry` |
| `접근 제한 감지, 6시간 대기` | 기다리세요. 우회 기능은 없습니다. 주기(`interval_minutes`)를 늘리는 것을 권장합니다 |
| `playwright 패키지가 없습니다` | `.\.venv\Scripts\python.exe -m pip install -r tools\requirements.txt` |
| `algo daemon이 이미 실행 중입니다` | `algo stop`. 강제 종료된 프로세스가 남긴 잠금은 자동으로 정리됩니다 |
| IDE 파일이 계속 대기 | `algo pending`의 이유 확인. 코드가 제출본과 다르면 `algo approve`로 직접 승인 |
| LeetCode 워크플로 실패 | Secrets의 쿠키 만료 → 새 값으로 교체 |
| README 숫자가 다름 | `python scripts/update_stats.py`로 다시 계산 (집계 기준은 그대로) |

## 9. 테스트
```powershell
cd tools
..\.venv\Scripts\python.exe -m pytest -q
```
임시 저장소와 가짜 origin에서 실행합니다. 확인 내용: Java/C++ 감지, 정답 확인 전 업로드 차단, 같은 제출 중복 방지, 언어별 보존, 기존 파일 보호, API 오류 복구, 원격 충돌 처리, README 집계, 반복 실행 시 멱등성, metadata 마이그레이션. 실제 사이트는 호출하지 않습니다.

### 실제 제출로 검증이 필요한 항목
- 프로그래머스·SWEA 어댑터를 실제 로그인 세션으로 실행 (`algo sync --site programmers --no-publish`)
- LeetCode 쿠키 방식이 GitHub Actions(클라우드 IP)에서 차단되지 않는지 (Run workflow)
- IDE에서 만든 파일과 사이트 제출 코드가 자동으로 일치 판정되는지
