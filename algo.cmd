@echo off
rem ALGO auto-sync launcher: uses the repository .venv when present.
setlocal
set "ROOT=%~dp0"
if exist "%ROOT%.venv\Scripts\python.exe" (
  "%ROOT%.venv\Scripts\python.exe" "%ROOT%tools\algo_cli.py" %*
) else (
  python "%ROOT%tools\algo_cli.py" %*
)
exit /b %ERRORLEVEL%
