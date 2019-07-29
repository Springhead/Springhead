@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS	SetMakePath
::
::  ARGUMENTS
::	なし
::
::  DESCRIPTION
::	nmake を使用するための path を求め, 環境変数 MAKEPATH に設定する.
::
::  VERSION
::	Ver 1.0  2017/01/16 F.Kanehori	初版
::	Ver 1.1  2019/08/25 F.Kanehori	検索パス追加 (15.0) 【暫定】
::	Ver 2.0  2019/08/29 F.Kanehori	検索方式変更
:: ============================================================================
set PROG=%~n0

set SCRIPT=..\..\RunSwig\find_path.py

echo %PROG%: wait a moment ..
for /f "usebackq" %%o in (`python %SCRIPT% -s nmake.exe`) do set OUT="%%o"
if "%out%" equ "" (
	set MAKEPATH=
) else (
	set MAKEPATH=!OUT:/= !
)

:done
endlocal && (set MAKEPATH=%MAKEPATH%)
exit /b

::end SetMakePath.bat
