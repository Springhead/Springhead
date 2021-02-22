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
::	Ver 2.1  2019/09/18 F.Kanehori	Bug fixed (default python がないとき)
::	Ver 2.2  2020/09/18 F.Kanehori	python_adapter を do_python に入れ替え
:: ============================================================================
set PROG=%~n0

set CWD=%CD%
set SRCDIR=..\..
set SPRTOP=%SRCDIR%\..\..
set RUNSWIG_DIR=%SRCDIR%\RunSwig
set PYTHON=do_python.bat
set SCRIPT=%RUNSWIG_DIR%\find_path.py

echo %PROG%: wait a moment ..
cd %SRCDIR%
for /f "usebackq" %%o in (`do_python.bat RunSwig\find_path.py -s nmake.exe`) do set OUT="%%o"
cd %CWD%
if "%OUT%" equ "" (
	set MAKEPATH=
) else (
	set MAKEPATH=!OUT:/= !
)

:done
echo %PROG%: %MAKEPATH%
endlocal && (set MAKEPATH=%MAKEPATH%)
exit /b

::end SetMakePath.bat
