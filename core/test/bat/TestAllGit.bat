@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	TestAllGit toolset_id configuration platform test_repository
::
::  DESCRIPTION
::	dailybuild を実行する.
::	
::  ARGUMENTS
::	toolset_id	ツールセットの識別 { 10.0 | 11.0 | .. }
::	configuration	ビルド構成 { Debug | Release | .. }
::	platform	ビルドプラットフォーム { Win32 | x64 }
::	test_repository	作業リポジトリのルート
::
::  CAUTION
::	・/r オプションで指定するディレクトリは, ..\..\ を起点として指定する.
::	・/r オプションで指定したディレクトリがすでに存在するときは, その内容
:: 	  はすべて破棄される.
::	・Visual Studio 10.0 以降のみに対応
::
::  VERSION
::	Ver 1.0  2013/09/19 F.Kanehori	バッチファイルの再構成
::	Ver 2.0	 2014/02/19 F.Kanehori	result.log の出力行順序改訂
::	Ver 3.0  2017/10/26 F.Kanehori	新ツリー構造に対応
::	Ver 4.0  2017/12/13 F.Kanehori	GitHub 対応
::	Ver 4.1  2017/12/21 F.Kanehori	Log directory on web server changed.
::	Ver 4.11 2017/12/25 F.Kanehori	Bug fixed (History.log out path).
:: ============================================================================
set PROG=%~n0
set CWD=%cd%

::----------------------------------------------
:: 引数の処理
::
set TOOLSET_ID=%1
set CONFIGURATION=%2
set PLATFORM=%3
set TEST_REPOSITORY=%4

set ARGS=TOOLSET_ID CONFIGURATION PLATFORM TEST_REPOSITORY
for %%a in (%ARGS%) do (
	if "!%%a!" equ "" (
		echo %PROG%: missing argument %%a
		endlocal && set $status=-1
		exit /b
	)
)
rem if "%TOOLSET_ID%" equ "9" set TOOLSET_ID=9.0
rem if "%TOOLSET_ID%" equ "10" set TOOLSET_ID=10.0
rem if "%TOOLSET_ID%" equ "12" set TOOLSET_ID=12.0
if "%TOOLSET_ID%" equ "14" set TOOLSET_ID=14.0
rem echo TOOLSET_ID:      [%TOOLSET_ID%]
rem echo CONFIGURATION:   [%CONFIGURATION%]
rem echo PLATFORM:        [%PLATFORM%]
rem echo TEST_REPOSITORY: [%TEST_REPOSITORY%]

set SOLUTIONFILE_ID=%TOOLSET_ID%
rem if "%SOLUTIONFILE_ID%" equ "9.0" set SOLUTIONFILE_ID=9
rem if "%SOLUTIONFILE_ID%" equ "10.0" set SOLUTIONFILE_ID=10
rem echo SOLUTIONFILE_ID: [%SOLUTIONFILE_ID%]

::----------------------------------------------
:: 大域変数の初期化
::
set DATEORG=%DATE:~2,10%
set DATESTR=%DATEORG:/=.%
set LOGDIR=log

::----------------------------------------------
:: テストターゲットの設定
::
set TARGET_LIST=Stub	tests	Samples
set TRGTID_LIST=スタブ	empty	サンプル
set DO_BLD_LIST=yes	yes	yes
set DO_RUN_LIST=no	yes	no
call :list_to_array TARGETS %TARGET_LIST%
call :list_to_array TRGTIDS %TRGTID_LIST%
call :list_to_array DO_BLDS %DO_BLD_LIST%
call :list_to_array DO_RUNS %DO_RUN_LIST%
rem call :array_dump TARGETS
rem call :array_dump TRGTIDS
rem call :array_dump DO_BLDS
rem call :array_dump DO_RUNS

::----------------------------------------------
:: タグ登録の制御
::
set AT_LEAST_ONE_BLD_SUCC=0

::----------------------------------------------
:: ログファイル名の設定
::
set BLD_SUCC_LOG_LIST=StubBuild.log Build.log SamplesBuild.log
set BLD_FAIL_LOG_LIST=StubBuildError.log BuildError.log SamplesBuildError.log
set RUN_SUCC_LOG_LIST=dummy_s.log Run.log SamplesRun.log
set RUN_FAIL_LOG_LIST=dummy_f.log RunError.log SamplesRunError.log
set RESULT_LOG=result.log
set HISTORY_LOG=History.log
call :prepend_logdir BLD_SUCC_LOG_LIST %LOGDIR%
call :prepend_logdir BLD_FAIL_LOG_LIST %LOGDIR%
call :prepend_logdir RUN_SUCC_LOG_LIST %LOGDIR%
call :prepend_logdir RUN_FAIL_LOG_LIST %LOGDIR%
set RESULT_LOG=%LOGDIR%\%RESULT_LOG%
set HISTORY_LOG=%LOGDIR%\%HISTORY_LOG%
call :list_to_array BLD_SUCC_LOGS %BLD_SUCC_LOG_LIST%
call :list_to_array BLD_FAIL_LOGS %BLD_FAIL_LOG_LIST%
call :list_to_array RUN_SUCC_LOGS %RUN_SUCC_LOG_LIST%
call :list_to_array RUN_FAIL_LOGS %RUN_FAIL_LOG_LIST%
rem call :array_dump BLD_SUCC_LOGS
rem call :array_dump BLD_FAIL_LOGS
rem call :array_dump RUN_SUCC_LOGS
rem call :array_dump RUN_FAIL_LOGS
rem echo.  [RESULT_LOG]: %RESULT_LOG%
rem echo.  [HISTORY_LOG]: %HISTORY_LOG%

if exist %RESULT_LOG% del %RESULT_LOG%

rem ------------------------------
rem  必要なパスの設定
rem 
set DEVENVPATH=Microsoft Visual Studio %TOOLSET_ID%\Common7\IDE
set X64PATH="C:\Program Files (x86)\%DEVENVPATH%"
set X32PATH="C:\Program Files\%DEVENVPATH%"
set ADDPATH=..\..\Springhead2Test\src\Foundation
set PROG=devenv.exe
if exist "%X32PATH:"=%\%PROG%" set PATH=%X32PATH:"=%;%ADDPATH%;%PATH%
if exist "%X64PATH:"=%\%PROG%" set PATH=%X64PATH:"=%;%ADDPATH%;%PATH%

if exist "%X32PATH:"=%\%PROG%" echo System: 32 bits
if exist "%X64PATH:"=%\%PROG%" echo System: 64 bits

::----------------------------------------------
:: 実行制御変数
::
set DO_CONTROL_LIST=DAILYBUILD_EXECUTE_STUBBUILD
set DO_CONTROL_LIST=%DO_CONTROL_LIST% DAILYBUILD_EXECUTE_BUILDRUN
set DO_CONTROL_LIST=%DO_CONTROL_LIST% DAILYBUILD_EXECUTE_SAMPLEBUILD
call :list_to_array DO_CONTROLS %DO_CONTROL_LIST%
rem call :array_dump DO_CONTROLS

rem ** 下記ファイルの存在有無でターゲット全体の実行を制御できる **
rem **** ただし, FORCE_DONT_xx の方が優先する ********************
set FOECE_DO_BLD=dailybuild.do.build
set FORCE_DO_RUN=dailybuild.do.run
set FORCE_DONT_BLD=dailybuild.dont.build
set FORCE_DONT_RUN=dailybuild.dont.run

::
set SRCDIR=..\src
set SCRIPT=bat\BuildVC.bat

::----------------------------------------------
:: ビルド＆実行
::
set INDEX=0
for %%t in (%TARGET_LIST%) do (

	rem ------------------------------
	rem  ターゲットの識別
	rem 
	set /a INDEX+=1
	call :nth_element TARGET TARGETS !INDEX!
	call :nth_element DO_BLD DO_BLDS !INDEX!
	call :nth_element DO_RUN DO_RUNS !INDEX!

	if exist %SRCDIR%\%FORCE_DO_BLD% set DO_BLD=yes
	if exist %SRCDIR%\%FORCE_DO_RUN% set DO_RUN=yes
	if exist %SRCDIR%\%FORCE_DONT_BLD% set DO_BLD=no
	if exist %SRCDIR%\%FORCE_DONT_RUN% set DO_RUN=no
	set JOB=
	if "!DO_BLD!" equ "yes" call :append JOB build
	if "!DO_RUN!" equ "yes" call :append JOB run and
	rem echo TARGET: !TARGET!	^(!JOB!^)

	rem ------------------------------
	rem  変数の初期化
	rem 
	set BLD_SUCC_LIST_INIT=ビルド成功 (
	set BLD_FAIL_LIST_INIT=ビルド失敗 (
	set RUN_SUCC_LIST_INIT=実行成功 (
	set RUN_FAIL_LIST_INIT=実行失敗 (

	set BLD_SUCC=
	set BLD_FAIL=
	set BLD_SUCC_LIST=!BLD_SUCC_LIST_INIT!
	set BLD_FAIL_LIST=!BLD_FAIL_LIST_INIT!

	set RUN_SUCC=
	set RUN_FAIL=
	set RUN_SUCC_LIST=!RUN_SUCC_LIST_INIT!
	set RUN_FAIL_LIST=!RUN_FAIL_LIST_INIT!

	rem ------------------------------
	rem  ログファイルの初期化
	rem 
	call :nth_element TARGETID_STR TRGTIDS !INDEX!
	rem echo.  TARGETID_STR: !TARGETID_STR!

	if "!DO_BLD!" equ "yes" (
		call :nth_element BLD_SUCC_LOG BLD_SUCC_LOGS !INDEX!
		call :nth_element BLD_FAIL_LOG BLD_FAIL_LOGS !INDEX!
		rem echo.  BLD_SUCC_LOG: !BLD_SUCC_LOG!
		rem echo.  BLD_FAIL_LOG: !BLD_FAIL_LOG!
		call :init_logfile !BLD_SUCC_LOG! !TARGETID_STR!ビルド
		call :init_logfile !BLD_FAIL_LOG! !TARGETID_STR!ビルドエラー
	)
	if "!DO_RUN!" equ "yes" (
		call :nth_element RUN_SUCC_LOG RUN_SUCC_LOGS !INDEX!
		call :nth_element RUN_FAIL_LOG RUN_FAIL_LOGS !INDEX!
		rem echo.  RUN_SUCC_LOG: !RUN_SUCC_LOG!
		rem echo.  RUN_FAIL_LOG: !RUN_FAIL_LOG!
		call :init_logfile !RUN_SUCC_LOG! !TARGETID_STR!実行
		call :init_logfile !RUN_FAIL_LOG! !TARGETID_STR!実行エラー
	)

	call set CONTROL=%%DO_CONTROLS[!INDEX!]%%

	rem ------------------------------
	rem  INDEX により処理が異なる
	rem 
	if !INDEX! == 1 (
		rem ------------------------------
		rem  stub/library の作成
		rem 
		call :check_condition !CONTROL!
		if !$status! == 0 (
			echo making stubs/libraries
			set ARGS=%TOOLSET_ID% %CONFIGURATION% %PLATFORM%
			cmd /c bat\SwigAll.bat !ARGS!		>> !BLD_SUCC_LOG!
			type !BLD_SUCC_LOG! | find " error "	>> !BLD_FAIL_LOG!
			echo. >> !BLD_FAIL_LOG!
		)
	) else (
		rem ------------------------------
		rem  ビルド＆実行
		rem 
		call :check_condition !CONTROL!
		if !$status! == 0 (
			for /D %%p in (!SRCDIR!\!TARGET!\*) do (
				for /D %%n in (%%p\*) do (
					if exist %%n\*%SOLUTIONFILE_ID%.sln (
						set ARGS=%TOOLSET_ID%
						set ARGS=!ARGS! !TARGET!
						set ARGS=!ARGS! %%~np %%n
						set ARGS=!ARGS! %CONFIGURATION%
						set ARGS=!ARGS! %PLATFORM%
						set ARGS=!ARGS! !DO_BLD! !DO_RUN!
						set ARGS=!ARGS! !BLD_SUCC_LOG! !BLD_FAIL_LOG!
						set ARGS=!ARGS! !RUN_SUCC_LOG! !RUN_FAIL_LOG!
						call !SCRIPT! !ARGS!
					)
				)
				for %%n in (%%p\*%SOLUTIONFILE_ID%.sln) do (
					call :get_directory_name DIR %%n
					set ARGS=%TOOLSET_ID%
					set ARGS=!ARGS! !TARGET!
					set ARGS=!ARGS! %%~np !DIR!
					set ARGS=!ARGS! %CONFIGURATION%
					set ARGS=!ARGS! %PLATFORM%
					set ARGS=!ARGS! !DO_BLD! !DO_RUN!
					set ARGS=!ARGS! !BLD_SUCC_LOG! !BLD_FAIL_LOG!
					set ARGS=!ARGS! !RUN_SUCC_LOG! !RUN_FAIL_LOG!
					call !SCRIPT! !ARGS!
				)
			)
			if "!BLD_SUCC!" equ "ビルド成功" (
				rem ** タグ登録の予約をする **
				set AT_LEAST_ONE_BLD_SUCC=1
			)
			call :append_r_paren BLD_SUCC_LIST
			call :append_r_paren BLD_FAIL_LIST
			call :append_r_paren RUN_SUCC_LIST
			call :append_r_paren RUN_FAIL_LIST
			rem echo BLD_SUCC_LIST: [!BLD_SUCC_LIST!]
			rem echo BLD_FAIL_LIST: [!BLD_FAIL_LIST!]
			rem echo RUN_SUCC_LIST: [!RUN_SUCC_LIST!]
			rem echo RUN_FAIL_LIST: [!RUN_FAIL_LIST!]

			rem %RESULT_LOG% に出力する順序は次のとおりとする
			rem   ※ ウェブページの作成(plugin)でこの順序が仮定されている
			echo !TARGET!>> %RESULT_LOG%
			echo !BLD_SUCC_LIST!>>%RESULT_LOG%
			echo !BLD_FAIL_LIST!>>%RESULT_LOG%
			echo !RUN_SUCC_LIST!>>%RESULT_LOG%
			echo !RUN_FAIL_LIST!>>%RESULT_LOG%
			echo. >> %RESULT_LOG%
		)
	)
)

::----------------------------------------------
:: 履歴情報を出力
::
call :check_condition DAILYBUILD_GEN_HISTORY
if %$status% == 0 (
	rem ** 履歴情報を出力 **
	cd bin
	python VersionControlSystem.py -g all > ..\%HISTORY_LOG%
	cd ..
)

::----------------------------------------------
:: ログを Samba にコピーする
::
call :check_condition DAILYBUILD_COPYTO_BUILDLOG
if %$status% == 0 (
	set SMBBASE=\\haselab\HomeDirs\WWW\docroots\springhead\dailybuild\log
	echo copying logs to !SMBBASE!
	del !SMBBASE!\*.log
	xcopy /C/F/I/Y log\*.log !SMBBASE!
)

endlocal && set $status=0
exit /b 0


:: ============================================================================
::----------------------------------------------
:: リストを擬似配列にする（作業変数 _I, _J を使用）
::	arg1:	擬似配列名
::	arg2:	擬似配列に設定する値のリスト
::
:list_to_array
	set _I=0
	for %%a in (%*) do (
		set _J=!_I!
		if not !_I! == 0 (set %1[!_J!]=%%a)
 		set /a _I+=1
	)
exit /b

::----------------------------------------------
:: 擬似配列の n 番目の要素を取出す
::	arg1:	値を返す環境変数名
::	arg2:	擬似配列名
::	arg3:	n
::
:nth_element
	setlocal
	call set VAL=%%%2[%3]%%
	if "%VAL%" equ "empty" set VAL=
	endlocal && set %1=%VAL%
exit /b

::----------------------------------------------
:: ログファイル名にディレクトリ名を前置する
::	arg1:	ログファイルのリスト名
::	arg2:	ログディレクトリ名
::
:prepend_logdir
	setlocal
	call set OLDLIST=%%%1%%
	set NEWLIST=
	for %%f in (%OLDLIST%) do (
		set NEWLIST=!NEWLIST! %2\%%f
	)
	set NEWLIST=%NEWLIST:~1%
	endlocal && set %1=%NEWLIST%
exit /b

::----------------------------------------------
:: ログファイルの初期化
::	arg1:	ログファイル名
::	arg2:	ログの種別（識別用）
::
:init_logfile
	echo 日付 : %DATESTR% > %1
	echo --- %2のログ --- >> %1
	echo. >> %1
exit /b

::----------------------------------------------
:: ディレクトリ名の取出し
::	arg1:	結果を返す変数名
::	arg2:	パス名
::
:get_directory_name
	setlocal
	set DIR=%~pd2
	set DIR=%DIR:~0,-1%
	endlocal && set %1=%DIR%
exit /b

::----------------------------------------------
:: 文字列のアペンド
::	arg1:	環境変数名
::	arg2:	追加する文字列
::	arg3:	区切り記号文字
::
:append
	setlocal
	call set TEXT=%%%1%%
	if "%TEXT%" equ "" (
 		set TEXT=%2
	) else (
 		set TEXT=%TEXT% %3 %2
	)
	endlocal && set %1=%TEXT%
exit /b

::----------------------------------------------
:: 文字列の末尾に')'を付加する（末尾の','は除去）
::	arg1:	環境変数名
::
:append_r_paren
	setlocal
	call set LIST=%%%1%%
	if "%LIST:~-1%" equ "," set LIST=%LIST:~,-1%
	endlocal && set %1=%LIST%^)
exit /b

::----------------------------------------------
:: 環境変数による実行制御
::	arg1:	実行環境制御変数名
::	$status	実行結果
::
:check_condition
	setlocal
	set RET=0
	call set VAL=%%%1%%
	if "%VAL%" equ "skip" (
		if "%2%" neq "silent" (
			echo skip ..%1..
		)
		set RET=-1
	) 
	endlocal && set $status=%RET%
exit /b

::----------------------------------------------
:: 擬似配列のダンプ（デバッグ用）
::	arg1:	擬似配列名
::
:array_dump
	setlocal
	echo %1:
	set I=1
:array_dump_1
	call set V=%%%1[%I%]%%
	if "!V!" neq "" (
 		echo.  [!I!]: !V!
 		set /a I+=1
 		goto :array_dump_1
	)
	endlocal
exit /b

::end TestAllGit.bat
