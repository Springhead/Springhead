@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	do_python.bat script.py [args..]	（Windows 専用）
::
::  DESCRIPTION
::	Python script を実行するための Windows 用アダプタ。
::	"Springhead/buildtool" が利用できるときは、それを使って実行する。
::	さもなければ、デフォルトで利用できる python を使って実行する。
::	Python が利用できない場合はメッセージを表示して処理を中止する。
::	
::	このファイルは Windows 専用である。
::	このファイルは "<top-dir>/core/src" に置く。
::
::  VERSION
::    Ver 1.0    2019/10/10 F.Kanehori	RunSwig から移動. -SprTop は廃止.
::    Ver 1.1    2020/04/16 F.Kanehori	_SPRTOP_ のデフォルトを変更.
::    Ver 2.0    2020/05/09 F.Kanehori	_SPRTOP_ は廃止.
::    Ver 3.0    2020/12/07 F.Kanehori	Setup 導入移行期間開始.
::    Ver 4.0    2021/03/04 F.Kanehori	Setupは非標準、python 2.7もOK.
::    Ver 4.1    2021/03/18 F.Kanehori	判定及びメッセージ修正
::    Ver 4.1.1  2021/03/24 F.Kanehori	Bug fix.
:: ============================================================================
set dry_run=0
set verbose=0

:: -------------------------------------------------------------
::  このスクリプトは "<SprTop>/core/src" に置く
::
set CWD=%cd%
cd /d %~dp0\..\..\..
set SprTop=%CD%
cd %CWD%

::-----------------------------------------------------------------------------
:: "%SprTop%/buildtool/win32/python.exe" があれば、それを使用する。
:: さもなければデフォルトの python を使用する。
::
set BUILDTOOL_PATH=%SprTop%\buildtool
if exist "%BUILDTOOL_PATH%\win32\python.exe" (
	Path=%BUILDTOOL_PATH%\win32;%BUILDTOOL_PATH%;!PATH!
)

set python_found=0
if %verbose% equ 0 (
	where python >NUL 2>&1
) else (
	echo where python
	where python
)
if %ERRORLEVEL% equ 0 (
	for /f "usebackq tokens=*" %%a in (`python --version`) do (
		set py_version=%%a
	)
	if "!py_version!" neq "" (
		set python_found=1
	)
)
if %python_found% neq 1 (	
	echo do_python.bat: Python not found.
	echo do_python.bat: We need python to build Springhead Library.
	endlocal
	exit /b
)

::----------------------------------------------
::  Python を実行する（引数はそのまま渡す）
::
set ARGS=%*
echo (SprTop: %SprTop%, using %py_version%)
echo (python %ARGS%)
if %dry_run% equ 0 (
	python %ARGS%
)

endlocal
exit /b

:: end: do_python.bat
