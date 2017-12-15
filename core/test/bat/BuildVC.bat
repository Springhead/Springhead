@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	BuildVC args (see below)
::
::  DESCRIPTION
::	引数 solution で指定したソリューションファイルについて Build (and Run)
::	テストを行なう.
::
::  ARGUMENTS
::	toolset_id	ツールセットの識別 { 10.0 | 11.0 | .. }
::	testset		テストセット名（表示用）
::	category	テストカテゴリー名（表示用）
::	solution_dir	ソリューション名（ディレクトリ名）
::	build_conf	ビルドオプション（構成）
::	build_plat	ビルドオプション（プラットフォーム）
::	do_build	ビルド指定 { yes | no }
::	do_run		実行指定 { yes | no }
::	log_1		ビルド結果のログファイル名（追記形式）
::	log_2		ビルドエラーのログファイル名（追記形式）
::	log_3		実行結果のログファイル名（追記形式）
::	log_4		実行エラーのログファイル名（追記形式）
::	python_ver	Python のバージョン
::
::  CAUTION
::	実行に必要な PATH が正しく設定されていること.
::	次の環境変数は呼出し元と共有するので注意すること.
::	    BLD_SUCC, BLD_FAIL, BLD_SUCC_LIST, BLD_FAIL_LIST
::	    RUN_SUCC, RUN_FAIL, RUN_SUCC_LIST, RUN_FAIL_LIST
::	Visual Studio 10.0 以降のみに対応
::
::  VERSION
::	Ver 1.0  2013/09/18 F.Kanehori	バッチファイルの再構成
::	Ver 1.1  2013/12/05 F.Kanehori	Visual Studio 2008 対応
::	Ver 1.2	 2014/06/11 F.Kanehori	ソリューションファイルの別名導入
::	Ver 1.3	 2014/10/23 F.Kanehori	出力ディレクトリ指定ファイルの導入
::	Ver 1.4	 2014/11/20 F.Kanehori	ビルド構成に python_ver を反映
::	Ver 1.5  2016/06/02 F.Kanehori	Visual Studio 2015 対応
::	Ver 1.6  2017/07/27 F.Kanehori	EmbPython configuration の変更
::	Ver 1.7  2017/07/27 F.Kanehori	ディレクトリ名変更に対応 (->dependency)
:: ============================================================================
call :leaf_name PROG %0
set CWD=%cd%
set RET=0

::----------------------------------------------
:: 引数の処理
::
call bat\BuildVC_ArgCheck.bat %*
if not %$status% == 0 goto :done

set TOOLSETID=%1&& shift
set TESTSET=%1&& shift
set CATEGORY=%1&& shift
(set SOLUTIONDIR=%1&& set SOLUTION=%~n1) && shift
set CONFIGURATION=%1&& shift
set PLATFORM=%1&& shift
set DO_BLD=%1&& shift
set DO_RUN=%1&& shift
set LOG_BLD=%1&& shift
set LOG_BLDERROR=%1&& shift
set LOG_RUN=%1&& shift
set LOG_RUNERROR=%1&& shift
rem call :vars_dump TOOLSETID TESTSET CATEGORY SOLUTIONDIR CONFIGURATION
rem call :vars_dump PLATFORM DO_BLD DO_RUN LOG_BLD LOG_BLDERROR
rem call :vars_dump LOG_RUN LOG_RUNERROR

set VS_DIR=%TOOLSETID%
if "%TOOLSETID%" equ "9" set VS_DIR=9.0
if "%TOOLSETID%" equ "9.0" set TOOLSETID=9
if "%TOOLSETID%" equ "10" set VS_DIR=10.0
if "%TOOLSETID%" equ "10.0" set TOOLSETID=10
if "%TOOLSETID%" equ "12" set VS_DIR=12.0
if "%TOOLSETID%" equ "12" set TOOLSETID=12.0
if "%TOOLSETID%" equ "14" set TOOLSETID=14.0

::----------------------------------------------
:: PATH の確認
::
set VS_PATH=Microsoft Visual Studio %VS_DIR%
rem if not exist "C:\Program Files\Microsoft Visual Studio %VS_DIR%\" (
if not exist "C:\Program Files\%VS_PATH%\" (
	if not exist "C:\Program Files (x86)\%VS_PATH%\" (
		echo %PROG%: bad toolset ID [%TOOLSETID%]
		set RET=-1
		goto :done
	)
)

::----------------------------------------------
:: 処理開始
::
call :fixed STR %SOLUTION%
set /p=.  %TESTSET%: %CATEGORY%: %STR%	.< NUL
cd %SOLUTIONDIR%

rem ** 下記ファイルの存在有無で実行を制御できる **
rem **** ただし, FORCE_DONT_xx の方が優先する ****
set FOECE_DO_BLD=dailybuild.do.build
set FORCE_DO_RUN=dailybuild.do.run
set FORCE_DONT_BLD=dailybuild.dont.build
set FORCE_DONT_RUN=dailybuild.dont.run
if exist %FOECE_DO_BLD% set DO_BLD=yes
if exist %FORCE_DO_RUN% set DO_RUN=yes
if exist %FORCE_DONT_BLD% set DO_BLD=no
if exist %FORCE_DONT_RUN% set DO_RUN=no

rem **** 出力ディレクトリの指定 *******************************************
rem **** 指定がなければ "$TOOLSET/$PLATFORM/$CONFIGURATION" と仮定する ****
rem **** 指定の中に $PYTHON_VERSION が含まれていたならば、ビルド構成を ****
rem **** %CONFIGURATION%_Py%PYTHOON_VERSION% に変更する *******************
set OUTDIR_ALIAS_FILE=dailybuild.outdir

set OUTDIRSPEC=$TOOLSET/$PLATFORM/$CONFIGURATION
if exist %OUTDIR_ALIAS_FILE% (
	for /f %%l in (%OUTDIR_ALIAS_FILE%) do set OUTDIRSPEC=%%l
	rem if "%PYTHON_VERSION%" neq "32" (
	rem 	rem ただし python32 だけは例外
	rem 	set CONFIGURATION=%CONFIGURATION%_Py%PYTHON_VERSION%
	rem 	set /p <nul=!CONFIGURATION! 
	rem )
	rem echo OUTDIRSPEC=[!OUTDIRSPEC!]
)
call set OUTDIRSPEC=%%OUTDIRSPEC:$TOOLSET=%TOOLSETID%%%
call set OUTDIRSPEC=%%OUTDIRSPEC:$PLATFORM=%PLATFORM%%%
call set OUTDIRSPEC=%%OUTDIRSPEC:$CONFIGURATION=%CONFIGURATION%%%
call set OUTDIRSPEC=%%OUTDIRSPEC:$PYTHON_VERSION=py%PYTHON_VERSION%%%

rem **** ソリューションファイル名の指定 ******************
rem **** 指定がなければディレクトリ名と同じと仮定する ****
set SOLUTION_ALIAS_FILE=dailybuild.alias

set SOLUTIONNAME=%SOLUTION%
if exist %SOLUTION_ALIAS_FILE% (
	for /f %%l in (%SOLUTION_ALIAS_FILE%) do set SOLUTIONNAME=%%l
)

set BUILD_OPTION="%CONFIGURATION%|%PLATFORM%"
set OUTDIR=%OUTDIRSPEC:/=\%
set EXEFILE=%OUTDIR%\%SOLUTIONNAME%.exe

::----------------------------------------------
:: ビルド
::
if /i "%DO_BLD%" equ "yes" (
	set SOLUTIONFILE=%SOLUTIONNAME%%TOOLSETID%.sln
	if not exist !SOLUTIONFILE! (
		rem echo %PROG%: can't find solution file "!SOLUTIONFILE!"
		set RET=-1
		goto :done
	)
	set LOG=%CWD%\%LOG_BLD%
	set ERRORLOG=%CWD%\%LOG_BLDERROR%
	set /p=build.< NUL

	rem ビルドをして結果をログファイルに出力
	echo *** %CATEGORY%: %SOLUTION% *** >> !LOG!
	echo %% devenv !SOLUTIONFILE! /build %BUILD_OPTION% > !LOG!.tmp
	devenv !SOLUTIONFILE! /build %BUILD_OPTION% >> !LOG!.tmp
	type !LOG!.tmp >> !LOG!
	echo. >> !LOG!
	echo. >> !LOG!

	rem エラーがある行だけをエラーログファイルに出力
	echo *** %CATEGORY%: %SOLUTION% *** >> !ERRORLOG!
	type !LOG!.tmp | find " error " >> !ERRORLOG!
	echo. >> !ERRORLOG!
	echo. >> !ERRORLOG!
	del !LOG!.tmp

	if exist %EXEFILE% (
		set BLD_SUCC=ビルド成功
		set BLD_SUCC_LIST=%BLD_SUCC_LIST%%CATEGORY%:%SOLUTION%,
	)
	if not exist %EXEFILE% (
		set BLD_FAIL=ビルド失敗
		set BLD_FAIL_LIST=%BLD_FAIL_LIST%%CATEGORY%:%SOLUTION%,
	)
) else (
	set /p=.< NUL
)

::----------------------------------------------
:: 実行
::
if /i "%DO_RUN%" equ "yes" (
	if not exist %EXEFILE% goto :done

	set BINBASE=%CWD%\..\bin
	set LIBSBASE=%CWD%\..\..\dependency\bin
	if "%PLATFORM%" equ "x64" (
		set TMPPATH=!LIBSBASE!\win64;!LIBSBASE!\win32
		set TMPPATH=!TMPPATH!;!BINBASE!\win64;!BINBASE!\win32
	) else (
		set TMPPATH=!LIBSBASE!\win32;!BINBASE!\win32
	)
	set PATH=!TMPPATH!;!PATH!
	set LOG=%CWD%\%LOG_RUN%
	set ERRORLOG=%CWD%\%LOG_RUNERROR%
	set /p=run< NUL

	rem 実行をして結果をログファイルに出力
	echo *** %CATEGORY%: %SOLUTION% *** >> !LOG!
	rem set ERRORLEVEL=
	%EXEFILE% > !LOG!.tmp
	set ERROR=%ERRORLEVEL%
	type !LOG!.tmp >> !LOG!
	echo. >> !LOG!
	echo. >> !LOG!

	rem エラーがある行だけをエラーログファイルに出力
	echo *** %CATEGORY%: %SOLUTION% *** >> !ERRORLOG!
	if not !ERROR! == 0 (
		type !LOG!.tmp | find " error " >> !ERRORLOG!
	)
	echo. >> !ERRORLOG!
	echo. >> !ERRORLOG!
	del !LOG!.tmp

	if !ERROR! == 0 (
		set RUN_SUCC=実行成功
		set RUN_SUCC_LIST=%RUN_SUCC_LIST%%CATEGORY%:%SOLUTION%,
	)
	if not !ERROR! == 0 (
		set RUN_FAIL=実行失敗
		set RUN_FAIL_LIST=%RUN_FAIL_LIST%%CATEGORY%:%SOLUTION%,
	)
)

:done
echo .
(
	endlocal
	set BLD_SUCC=%BLD_SUCC%
	set BLD_FAIL=%BLD_FAIL%
	set BLD_SUCC_LIST=%BLD_SUCC_LIST%
	set BLD_FAIL_LIST=%BLD_FAIL_LIST%
	set RUN_SUCC=%RUN_SUCC%
	set RUN_FAIL=%RUN_FAIL%
	set RUN_SUCC_LIST=%RUN_SUCC_LIST%
	set RUN_FAIL_LIST=%RUN_FAIL_LIST%
	set $status=%RET%
)
exit /b


::----------------------------------------------
:: リーフ名の取出し
::	arg1:	値を返す環境変数名
::	arg2:	パス名
::
:leaf_name
	set %1=%~n2
exit /b

::----------------------------------------------
:: 文字列長を固定にする（表示用：汎用性なし）
::	arg1:	値を返す環境変数名
::	arg2:	文字列
::
:fixed
	setlocal
	set STR1=%2,,,,,,,,,,,,,,,,,,,,,,,,
	set STR2=%STR1:~0,24%
	set STR3=%STR2:,= %
	endlocal && set %1=%STR3%
exit /b

::----------------------------------------------
:: 変数のダンプ
::	arg1:	変数のリスト
::
:vars_dump
	setlocal
	for %%v in (%*) do (
		set VAR=%%v
		call set VAL=%%!VAR!%%
		set VAR=!VAR!                +
		set VAR=!VAR:~0,16!
		echo !VAR!: [!VAL!]
	)
	endlocal
exit /b

::end BuildVC.bat
