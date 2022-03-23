@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	DailyBuild [options] [test-repository [result-repository]]
::
::	OPTIONS:
::	    --do-not-clone:	クローンを実行せず既存のレポジトリを使用する.
::	    --hook:		クローン後, "DailyBuildHook\hook.bat" が適用
::				される.
::	    -c conf:		Configurations (Debug | Release).
::	    -p plat:		Platform (x86 | x64).
::	    -s file:		Setup file name (default: setup.conf).
::	    -t tool:		Visual Studio toolset ID. (Windows only)
::	    -X:			Build and Run only (do not commit/copy).
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
::  IMPORTANT
::	このファイルを置く位置
::	    .../DailyBuild/DailyBuildHook
::			  /DailyBuildTestTools	…… 必要なツールを置く位置
::			  /Springhead		…… テストレポジトリ
::			  DailyBuild.bat	…… このファイルはここに置く
::
::	このファイル "DailyBuild.bat" 及び "DailyBuildHook/hook.bat" は,
::	DailyBuild を実行しても自動的にはアップデートされない.
::	変更があったときは手動でコピーすること.
::
::	Springhead Library 作成は外部ツールの存在を前提とせずに実行可能とする.
::	そのため python, nkf などのツールは PATH には登録しない.
::	ただし DailyBuild を実行するためにはこれらのツールが必要となるので,
::	DailyBuildTestTools に配置したツールを絶対パス起動の条件で使用する.
::	
::  VERSION
::     Ver 1.0   2021/05/10 F.Kanehori	バッチファイルの再構築.
::     Ver 1.1   2021/07/05 F.Kanehori	DailyBuildTestTools の導入.
::     Ver 2.0   2022/01/26 F.Kanehori	DailyBuild.py の機能を吸収.
::     Ver 2.1   2022/03/10 F.Kanehori	Add option -X for debug.
:: ============================================================================
set PROG=%~n0
set CWD=%CD%
set DEBUG=0
set START_AT_TESTDIR=0

set CLONE=yes
set HOOK=no

:: ---------------------------------------------------------------------
::  Step 0
::	引数の解析
::
set OPTS=
set SETUPFILE=setup.conf
:loop1
	if "%1" == "-h"		(call :usage & exit /b)
	if "%1" == "--help"	(call :usage & exit /b)
	if "%1" == "--do-not-clone"    (set CLONE=no & shift & goto :loop1)
	if "%1" == "--hook"	       (set HOOK=yes & shift & goto :loop1)
	if "%1" == "-c"	 (set OPTS=!OPTS! %1 %2 & shift & shift & goto :loop1)
	if "%1" == "-p"	 (set OPTS=!OPTS! %1 %2 & shift & shift & goto :loop1)
	if "%1" == "-t"	 (set OPTS=!OPTS! %1 %2 & shift & shift & goto :loop1)
	if "%1" == "-s"	 (set SETUPFILE=%1& shift & goto :loop1)
	if "%1" == "-S"	 (set FLAG[S]=true& shift & goto :loop1)
	if "%1" == "-D"	 (set FLAG[D]=true& shift & goto :loop1)
	if "%1" == "-X"	 (set FLAG[X]=true& shift & goto :loop1)
	if "%1" == "-v"	 (set FLAG[v]=true& shift & goto :loop1)

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
echo opts:   [%OPTS%]
echo clone:  [%CLONE%]
echo hook:   [%HOOK%]
echo dryrun: [%FLAG[D]%]
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
::	必要なツールの確認
::
set PREFIX=
if %START_AT_TESTDIR% == 1 (
	set PREFIX=..\..\..\
)
call :abspath %PREFIX%DailyBuildTestTools
set TOOLSBASE=%$result%
set PYTHON=%TOOLSBASE%\Python\python.exe
set NKF=%TOOLSBASE%\bin\nkf.exe
set SED=%TOOLSBASE%\sed\bin\sed.exe
set CMAKE=%TOOLSBASE%\cmake\bin\cmake.exe

set TOOLS=%PYTHON% %NKF% %SED% %CMAKE%
set OK=ok
for %%t in (%TOOLS%) do (
	%%t --version >NUL 2>&1
	if !ERRORLEVEL! neq 0 (
		echo Error: "%%t" not found
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
::	Hook ファイルが存在したらそれをコピーする.
::
:apply_hook
if "%HOOK%" neq "no" (
	echo %PROG%: calling hook "%HOOKFILE%".
	call %HOOKFILE% %TEST_REPOSITORY%
)

:: ---------------------------------------------------------------------
::  Step 4
::	セットアップを実行する (-S オプション指定時のみ)
::
if "%FLAG[S]%" == "true" (
	echo execute setup process ^(-S^)
	call :abspath %PREFIX%%TEST_REPOSITORY%\core\src
	cd !$result!
	set ARGS=-R %TEST_REPOSITORY% -f -d 1 -s %SETUPFILE% %FLAG[-v]%
	call :exec setup !ARGS!
	if not !ERRORLEVEL! equ 0 (
		echo setup failed ^(!ERRORLEVEL!^)
		exit /b
	)
)

:: ---------------------------------------------------------------------
::  Step 5
::	テストを実行する
::
call :abspath %PREFIX%%TEST_REPOSITORY%\core\test
cd !%$result%!
echo test directory: "%CD%"
set ARGS=%OPTS% %FLAG[v]% %TEST_REPOSITORY% %RESULT_REPOSITORY%
if "%FLAG[X]%" equ "true" (
	set ARGS=%ARGS% -X
)
call :exec %PYTHON% TestMainGit.py %ARGS%

endlocal
exit /b

:: ---------------------------------------------------------------------
:exec
	echo %*
	if not "%FLAG[D]%" == "true" (%*)
	exit /b

:abspath
	set $result=%~f1
	exit /b

:usage
	echo DailyBuild [options] [test-repository [result-repository]]
	echo+
	echo options:
	echo     --do-not-clone: do not clone source tree.
	echo     --hook file:    apply hook script.
	echo     -c conf:        Configurations (Debug ^| Release).
	echo     -p plat:        Platform (x86 ^| x64).
	echo     -s file:        Setup file name (default: setup.conf).
	echo     -t tool:        Visual Studio toolset ID. (Windows only).
	echo     -X:             build and run only (do not commit nor copy).
	echo+
	echo arguments:
	echo     test_repository:   test repository (default: Springhead)
	echo     result_repository: result repository (default: DailyBuildResult/Result)
	echo+
	exit /b

::end: DailyBuild.bat
