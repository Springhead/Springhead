@echo off
setlocal
set PY_VERSION=%1
if "%PY_VERSION%" equ "" set PY_VERSION=32
endlocal && set CURR_PY_VERSION=%PY_VERSION%

