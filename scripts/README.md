# Programmers Backup

별도 Edge 프로필의 로그인 세션에 CDP로 연결해 본인의 통과 제출 코드를 백업합니다.

## Setup

```powershell
python -m venv C:\Algorithm\.venv
C:\Algorithm\.venv\Scripts\python.exe -m pip install -r C:\Algorithm\scripts\requirements.txt
C:\Algorithm\.venv\Scripts\python.exe -m playwright install chromium
```

번들 Chromium 설치는 일반 Playwright 실행에 필요합니다. 현재 스크립트처럼 설치된 Edge에 CDP로 연결할 때는 생략할 수 있습니다.

## Login

```powershell
& 'C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe' `
  --remote-debugging-port=9222 `
  --remote-debugging-address=127.0.0.1 `
  --user-data-dir='C:\edge-codex-profile' `
  'https://school.programmers.co.kr/'
```

열린 Edge 창에서 직접 로그인합니다. 아이디와 비밀번호는 스크립트에 저장되지 않습니다.

## Run

```powershell
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --inspect
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --test
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --limit 10
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --all
```

`--inspect`는 파일을 저장하지 않고 현재 DOM의 solved 문제 링크와 브라우저가 실제로 요청한 경로를 확인합니다.

`--test`, `--limit`, `--all`은 metadata와 실제 풀이 파일을 기준으로 이미 존재하는 `문제 번호 + 언어` 조합을 제외합니다. 기존 파일은 갱신하거나 덮어쓰지 않으며, 아직 없는 통과 제출만 저장합니다. `--test`는 누락 문제를 최대 3개까지만 처리합니다.

`--all`은 테스트 결과를 확인한 뒤 실행하세요. 요청은 순차 처리되며 기본적으로 각 단계 사이에 1초 대기합니다.

## SWEA Backup

별도 Edge 프로필의 로그인 세션에 CDP로 연결해 본인의 SWEA Pass 제출 코드를 백업합니다.

```powershell
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\swea_backup.py --inspect
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\swea_backup.py --test
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\swea_backup.py --limit 10
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\swea_backup.py --all
```

`--test`는 아직 로컬에 없는 문제를 최대 3개 처리합니다. 같은 문제를 여러 언어로 통과했다면 언어별 최신 Pass 제출을 각각 저장합니다. 기존 코드가 다르면 덮어쓰지 않고 `UPDATE` 후보로만 보고합니다.

목록과 코드는 실제 SWEA DOM에서 확인한 사용자 코드 페이지, `My제출` 필터, 코드 보기 흐름을 사용해 순차 조회합니다. 403, 429, CAPTCHA 또는 코드 열람 권한 제한이 확인되면 우회하지 않고 중단합니다.

## LeetCode Backup

별도 Edge 프로필의 로그인 세션에 연결해 본인의 Accepted 제출 코드를 백업합니다.

```powershell
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\leetcode_backup.py --inspect
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\leetcode_backup.py --test
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\leetcode_backup.py --limit 3
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\leetcode_backup.py --all
```

목록, 제출 이력, 코드 원문은 로그인된 LeetCode 화면에서 실제 확인한 GraphQL 요청을 동일 브라우저 세션으로 순차 조회합니다. `--test`는 로컬에 아직 없는 문제를 최대 3개 처리하고, 같은 문제의 여러 Accepted 언어는 각각 저장합니다. 기존 코드가 다르고 최신 여부를 확정할 metadata가 없으면 덮어쓰지 않고 `UPDATE-CANDIDATE`로 보고합니다.
