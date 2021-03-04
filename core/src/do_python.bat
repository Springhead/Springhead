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
::	デフォルトで python が利用できない場合はメッセージを表示して
::	処理を中止する。
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
:: ============================================================================
set PROG=%~n0
set START_DIR=%CD%
set verbose=0
set dry_run=0

::-----------------------------------------------------------------------------
:: Springhead tree のトップへ移動する
::	このスクリプトは "<top>/core/src" に置く。
::
cd /d %~dp0
cd ..\..
set SprTop=%CD%
cd %START_DIR%

::-----------------------------------------------------------------------------
:: "%SprTop%/buildtool/win32/python.exe" があれば、それを使用する。
:: さもなければデフォルトの python を使用する。
::
set BUILDTOOL_PATH=%SprTop%\buildtool\win32
if exist "%BUILDTOOL_PATH%\python.exe" (
	set path=%BUILDTOOL_PATH%;!PATH!
	if %verbose% geq 2 (
		echo buildtool found at "%BUILDTOOL_PATH%"
	)
)
) else (
	where python >NUL 2>& 1
	if !ERRORLEVEL! neq 0 (
		echo Python not found.
		echo We need python to build Springhead Library.
		endlocal
		exit /b
	)
)
if %verbose% geq 1 (
	python --version
)

::----------------------------------------------
::  Python を実行する（引数はそのまま渡す）
::
set ARGS=%*
if %dry_run% neq 0 (
	echo python %ARGS%
) else (
	python %ARGS%
)

endlocal
exit /b

:: end: do_python.bat
