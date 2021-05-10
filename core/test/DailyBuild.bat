@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	DailyBuild [options] [test-repository [result-repository]]
::
::	OPTIONS:
::	    --do-not-clone:	クローンを実行せず既存のレポジトリを使用する.
::				--hook オプションより前に指定すること.
::	    --hook:		クローン後, "DailyBuildHook\hook.bat" が適用
::				される.
::	    -c conf:		Configurations (Debug | Release).
::	    -p plat:		Platform (x86 | x64).
::	    -s file:		Setup file name (default: setup.conf).
::	    -t tool:		Visual Studio toolset ID. (Windows only)
::
::	ARGUMENTS:
::	    test-repository:	テストを実行するディレクトリ.
::				既存のディレクトリの場合その内容は破棄される.
::				デフォルトは "Springhead".
::	    result-repository:	テスト結果が置かれるディレクトリ.
::				デフォルトは "DailyBuildResult\Result"
::
::  DESCRIPTION
::	DailyBuild を実行を制御する (Windows task に登録する).
::	  (1) test-repository に最新のコミットを clone する.
::	  (2) 指定があれば hook を適用する (ファイルのすげ替え).
::	  (3) テストを実行する.
::
::	Python が実行できること. このファイルは demo マシンの
::	  "I:\DailyBuild"
::	にコピーし, そこから起動する.
::
::	起動パラメータの例
::	  DailyBuild.bat -t 15.0 -c Release -p x64 \ (次の行に続く)
::				Springhead DailyBuildResult/Result
::	タスクスケジューラの開始 (オプション)(T) は "I:\DailyBuild" とする.
::
::	このファイル "DailyBuild.bat" 及び "hook.bat" は, DailyBuild を実行
::	しても自動的にはアップデートされない. 変更があったときは手動でコピー
::	すること.
::	
::  VERSION
::     Ver 1.0   2021/05/10 F.Kanehori	バッチファイルの再構築.
:: ============================================================================
set PROG=%~n0
set CWD=%CD%
set DEBUG=0

set CLONE=yes
set HOOK=no

:: ---------------------------------------------------------------------
::  Step 0
::	引数の解析
::
set OPTS=
:loop1
	if "%1" == "-h"		(call :usage & exit /b)
	if "%1" == "--help"	(call :usage & exit /b)
	if "%1" == "--do-not-clone"    (set CLONE=no & shift & goto :loop1)
	if "%1" == "--hook"	       (set HOOK=yes & shift & goto :loop1)
	if "%1" == "-c"	 (set OPTS=!OPTS! %1 %2 & shift & shift & goto :loop1)
	if "%1" == "-p"	 (set OPTS=!OPTS! %1 %2 & shift & shift & goto :loop1)
	if "%1" == "-t"	 (set OPTS=!OPTS! %1 %2 & shift & shift & goto :loop1)

if not "%2" == "" (set RESULT_REPOSITORY=%2)
if not "%1" == "" (set TEST_REPOSITORY=%1)
if "%TEST_REPOSITORY%" == "" (
	set TEST_REPOSITORY=Springhead
)
if "%RESULT_REPOSITORY%" == "" (
	set RESULT_REPOSITORY=DailyBuildResult/Result
)
set REMOTE_REPOSITORY=https://github.com/sprphys/Springhead
set HOOKFILE=DailyBuildHook\hook.bat

set CLONE=%CLONE: =%
set HOOK=%HOOK: =%
echo test repository:   [%TEST_REPOSITORY%]
echo result repository: [%RESULT_REPOSITORY%]
echo opts:  [%OPTS%]
echo clone: [%CLONE%]
echo hook:  [%HOOK%]
if "%HOOK%" neq "no" (
	if not exist %HOOKFILE% (
		echo --hook specified, but "%HOOKFILE%" does not exist.
		echo abort
		exit /b
	)
)
if %DEBUG% neq 0 (
	set /p x="OK [y/n]? "
	if not "!x!" == "y" (echo done & exit /b)
)

:: ---------------------------------------------------------------------
::  Step 1
::	必要なツールの確認 (python が実行できること)
::
set PYTHON=python.exe

set TOOLS=%PYTHON%
set OK=ok
for %%t in (%TOOLS%) do (
	where %%t >NUL 2>&1
	if !ERRORLEVEL! neq 0 (
		echo %PROG%: we need '%%t'
		set OK=no
	)
)
if "%OK%" neq "ok" (
	echo abort
	exit /b
)

:: ---------------------------------------------------------------------
::  Step 2
::	Springhead を最新の状態にする.
::
if "%CLONE%" neq "yes" goto :apply_hook
if exist %TEST_REPOSITORY% (
	rem 古いテストレポジトリがあったら削除して更の状態にする.
	rem
	echo %PROG%: removing directory "%TEST_REPOSITORY%"
	rmdir /s /q %TEST_REPOSITORY%
)
echo cloning %REMOTE_REPOSITORY%
git clone --recurse-submodules %REMOTE_REPOSITORY% %TEST_REPOSITORY%

:: ---------------------------------------------------------------------
::  Step 3
::	Hook ファイルが存在したらそれを実行する.
::
:apply_hook
if "%HOOK%" neq "no" (
	echo %PROG%: calling hook "%HOOKFILE%".
	call %HOOKFILE% %TEST_REPOSITORY%
)

:: ---------------------------------------------------------------------
::  Step 4
::	テストを実行する
::
cd %TEST_REPOSITORY%\core\test
echo %PROG%: test directory: "%CD%"
echo %PYTHON% DailyBuild.py -A -f %OPTS% %TEST_REPOSITORY% %RESULT_REPOSITORY%
%PYTHON% DailyBuild.py -A -f %OPTS% %TEST_REPOSITORY% %RESULT_REPOSITORY%

endlocal
exit /b

:: ---------------------------------------------------------------------
:usage
	echo DailyBuild [options] [test-repository [result-repository]]
	echo+
	echo   options:
	echo     --do-not-clone: Do not clone source tree (must be a first option).
	echo     --hook:         Apply hook script ("DailyBuildHook\hook.bat").
	echo     -c conf:        Configurations (Debug ^| Release).
	echo     -p plat:        Platform (x86 ^| x64).
	echo     -t tool:        Visual Studio toolset ID (15.0, ...).
	echo+
	echo   arguments:
	echo     test_repository:   test repository (default: Springhead)
	echo     result_repository: result repository (default: DailyBuildResult/Result)
	exit /b

::end: DailyBuild.bat
