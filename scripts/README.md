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
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --test
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --limit 10
C:\Algorithm\.venv\Scripts\python.exe C:\Algorithm\scripts\programmers_backup.py --all
```

`--all`은 테스트 결과를 확인한 뒤 실행하세요. 요청은 순차 처리되며 기본적으로 각 단계 사이에 1초 대기합니다.
