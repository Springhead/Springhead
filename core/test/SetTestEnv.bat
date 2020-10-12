@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	SetTestEnv [/k] [/a] [/h]
::
::  DESCRIPTION
::	daily_build の実行を制御するための“環境変数”設定を行なう.
::	使用する環境変数には、接頭辞“DAILYBUILD_”を付与する.
::
::  ARGUMENTS
::	/k	現在の環境変数値をデフォルト値とする
::	/a	全ステップ実行をデフォルト値とする
::	/h	使用方法の表示
::     [/b]	作業バッチファイルを残す (for debug)
::
::  CAUTION
::	環境変数の使い方
::	・通常はここで扱う環境変数には何も設定されていないものとする
::	・環境変数に"skip"と設定した時のみ対応部分をスキップする
::
::  VERSION
::	Ver 1.00  2013/10/24 F.Kanehori	バッチファイルの再構築
::	Ver 1.01  2013/11/20 F.Kanehori	新変数導入：MAKE_ARCHIVE
::	Ver 1.02  2016/06/02 F.Kanehori	新制御変数の導入 (.._ELIMINATE_CLOSED)
::	Ver 1.03  2017/08/07 F.Kanehori	変更 (.._SPRINGHEAD2 -> .._SPRINGHEAD)
::	Ver 1.04  2017/12/21 F.Kanehori	変更 (.._COPYTO_TAGS -> .._GEN_HISTORY)
::	Ver 1.05  2017/12/25 F.Kanehori	TestAll 実行条件変更
::	Ver 1.06  2018/04/26 F.Kanehori	新制御変数の導入 (.._COMMIT_RESULTLOG)
::	Ver 1.07  2020/10/12 F.Kanehori	新制御変数の導入 (.._EXECUTE_EMBPYTHON)
:: ============================================================================
set PROG=%~n0

::----------------------------------------------
:: 制御用環境変数識別接頭辞の定義
::
set PREFIX=DAILYBUILD_

::----------------------------------------------
:: 使用する環境変数の定義（%PREFIX%を除いた部分）
::
rem for TestMain.bat
set ENVMAIN1=%PREFIX%UPDATE_SPRINGHEAD %PREFIX%CLEANUP_WORKSPACE %PREFIX%ELIMINATE_CLOSED
set ENVMAIN2=%PREFIX%EXECUTE_MAKEDOC %PREFIX%COPYTO_WEBBASE
rem for TestALL.bat
set ENVTEST1=%PREFIX%EXECUTE_STUBBUILD %PREFIX%EXECUTE_EMBPYTHON %PREFIX%EXECUTE_BUILDRUN %PREFIX%EXECUTE_SAMPLEBUILD
set ENVTEST2=%PREFIX%GEN_HISTORY %PREFIX%COMMIT_RESULTLOG %PREFIX%COPYTO_BUILDLOG
::
:: (these are SVN version only)
rem set ENVMAIN2=%PREFIX%EXECUTE_MAKEDOC %PREFIX%COPYTO_WEBBASE %PREFIX%MAKE_ARCHIVE
rem set ENVTEST2=%PREFIX%GEN_HISTORY %PREFIX%COMMIT_BUILDLOG %PREFIX%COPYTO_BUILDLOG
::
set ENV_ALL=%ENVMAIN1% %ENVTEST1% %ENVTEST2% %ENVMAIN2%
set ENVMAIN=%ENVMAIN1% %ENVMAIN2%
set ENVTEST=%ENVTEST1% %ENVTEST2%
set ENVEXEC=%PREFIX%EXECUTE_TESTALL
::
call :max_length LEN %ENV_ALL% %ENVEXEC%
set MAXWIDTH=%LEN%

::----------------------------------------------
:: オプション変数の初期値
::
set EXECUTE_ALL=
set KEEP_VALUES=

::----------------------------------------------
:: 引数の処理
::
call :get_args %*
if not %$status% == 0 goto :done
rem echo EXECUTE_ALL: [%EXECUTE_ALL%]
rem echo KEEP_VALEUS: [%KEEP_VALUES%]

::----------------------------------------------
:: 環境変数の初期設定
::
for %%v in (%ENV_ALL% %ENVEXEC%) do (
    if "%KEEP_VALUES%" equ "yes" (
	    set VAR=%%v
	    call set VAL=%%!VAR!%%
	    set VALUES[%%v]=!VAL!
    ) else if "%EXECUTE_ALL%" equ "yes" (
	    set VALUES[%%v]=
    ) else (
	    set VALUES[%%v]=skip
    )
)
echo 初期値は次のとおり
call :show STDOUT

::----------------------------------------------
:: 実行指定の入力
::
echo.
echo 実行するものだけ選んでください [e^(xec^)/s^(kip^)]
for %%v in (%ENVMAIN1%) do (call :input %%v)
echo.
for %%v in (%ENVTEST1%) do (call :input %%v)
for %%v in (%ENVTEST2%) do (call :input %%v)
echo.
for %%v in (%ENVMAIN2%) do (call :input %%v)

::----------------------------------------------
:: TestAll 実行の判定
::
set COUNT=0
for %%v in (%ENVTEST1% %ENVTEST2%) do (
    set VAL=VALUES[%%v]
    call set VAL=%%!VAL!%%
    if "!VAL!" neq "skip" set /a COUNT+=1
)
if %COUNT% equ 0 (
    set VALUES[%ENVEXEC%]=skip
) else (
    set VALUES[%ENVEXEC%]=
)

::----------------------------------------------
:: 確認
::
echo.
echo 指定されたパラメータは次のとおりです.
call :show STDOUT

echo.
set YN=
:try_again
set /p YN="指定した値を設定しますか[y/n] ? "
if /i "%YN%" neq "y" if /i "%YN%" neq "n" goto :try_again
::
:: 環境変数の設定は外部の作業バッチファイルを介して行なう.
::   バッチファイルの実行時と削除時にもファイル名を指定する必要があるが,
:: 　途中でいったんグローバルスコープに抜けるため,
::   ここで設定したファイル名はその時点で無効となってしまう.
:: 　再度ファイル名を指定するときは, ここでの指定と矛盾しないよう注意が必要.
::   (169行目及び170行目)
::
set BATFILE=%~n0.tmp.bat
if exist %BATFILE% del %BATFILE%

if /i "%YN%" neq "y" (
    set MSG=設定は以前のままです.
    set CMD=rem set
) else (
    set MSG=次のように設定しました.
    set CMD=set
)

echo @echo off > %BATFILE%
for %%v in (%ENV_ALL% %ENVEXEC%) do (
    set VAL=VALUES[%%v]
    call set VAL=%%!VAL!%%
    echo %CMD% %%v=!VAL!>> %BATFILE%
)
echo echo. >> %BATFILE%
echo echo %MSG%>> %BATFILE%
call :show %BATFILE%
echo exit /b>> %BATFILE%
echo :display_string >> %BATFILE%
echo if "%%2" equ "skip" ^(>> %BATFILE%
echo.  set %%1=.... skip>> %BATFILE%
echo ^) else ^(>> %BATFILE%
echo.  set %%1=exec>> %BATFILE%
echo ^)>> %BATFILE%
echo exit /b>>%BATFILE%

::----------------------------------------------
:: 作業バッチファイルの実行と削除
:: 　バッチファイルはグローバルスコープで実行する.
::   呼び出すバッチファイル名は, 133行目で指定したもの(BATFILE)と
::   矛盾しないように注意すること.
::
endlocal
call %~n0.tmp.bat
del %~n0.tmp.bat
setlocal

:: 終了
::
set $status=0
:done
endlocal && set $status=%$status%
exit /b

::------------------------------------------------------------------------------
::------------------------------------------------------------------------------
:: コマンド引数の処理
::
:get_args
    set $status=0
:get_args_loop
    if "%1" equ "" goto :get_args_exit
    if /i "%1" equ "/a" ((set EXECUTE_ALL=yes) && shift && goto :get_args_loop)
    if /i "%1" equ "/k" ((set KEEP_VALUES=yes) && shift && goto :get_args_loop)
    if /i "%1" equ "/h" (call :usage && goto :get_args_exit)
    echo bad arg [%1] && set $status=-1 && shift
    goto :get_args_loop
:get_args_exit
exit /b

::----------------------------------------------
:: 環境変数値の表示
::	arg1:	STDOUT: 画面に表示
::		その他: ファイルに出力
::
:show
    setlocal
    set OUTTO=%1
    set /a MAX_MAIN=%MAXWIDTH%+1
    set /a MAX_TEST=%MAX_MAIN%-2
    call :values_dump %OUTTO% 2 %MAX_MAIN% %ENVMAIN1%
    call :values_dump %OUTTO% 2 %MAX_MAIN% %ENVEXEC%
    call :values_dump %OUTTO% 4 %MAX_TEST% %ENVTEST1%
    call :values_dump %OUTTO% 4 %MAX_TEST% %ENVTEST2%
    call :values_dump %OUTTO% 2 %MAX_MAIN% %ENVMAIN2%
    endlocal
exit /b

::----------------------------------------------
:: 実行指定の入力処理
::	arg1:	環境変数名
::	入力した値は, VALUES[arg1] に格納される
::
:input
    setlocal
    set VAR=%1
    set VAL=VALUES[%VAR%]
    call set VAL=%%%VAL%%%
    if not defined VAL set VAL=exec
    call :fixlen VAR %MAXWIDTH%
    call :fixlen VAL 4
    set YN=
    set /p YN=%VAR% (%VAL%)? 
    set RET=%VAL%
    if /i "%YN%" equ "e" (
	    set RET=
    ) else if /i "%YN%" equ "y" (
	    set RET=
    ) else if /i "%YN%" equ "s" (
	    set RET=skip
    ) else if /i "%YN%" equ "n" (
	    set RET=skip
    ) else if "%KEEP_VALUES%" neq "yes" (
	    set RET=skip
    )
    endlocal && set VALUES[%1]=%RET%
exit /b

:: ----------------------------------------------------
::  変数に指定した数の空白文字列を設定する
::	arg1:	変数名 <$string>
::	arg2:	空白の数
:: 
:spaces
    setlocal
    set VAR=$string
    set CHK=%2
    if defined CHK set VAR=%1&&shift
    set $result=
    for /l %%n in (1,1,%1) do set $result= !$result!
    endlocal && call set %VAR%=%$result%
exit /b

:: ----------------------------------------------------
::  変数の内容を指定した長さに調整する
::	arg1:	変数名
::	arg2:	長さ
::     [arg3:]	配置（'left' or 'right') <'left'>
::     [arg4:]	充填文字列 <空白文字列>
:: 
:fixlen
    setlocal
    set SPACES=%4
    if not defined SPACES call :spaces SPACES %2
    if "%3" equ "right" (
	call set $string=%SPACES%%%%1%%
	    set CMD=$string:~-%2
    ) else (
	    call set $string=%%%1%%%SPACES%
	    set CMD=$string:~0,%2
    )
    call set $string=%%!CMD!%%
    endlocal && set %1=%$string%
exit /b

:: ----------------------------------------------------
::  変数の長さを求める
::     [arg1]:	値を返す変数名（省略可：省略時は $result）
::	arg2:	変数名
:: 
:strlen
    setlocal
    set RET=%1
    set STR=%2
    if not defined STR (
	    set RET=$result
	    set STR=%1
    )
    set LEN=0
:strlen_loop
    if defined STR (
	    set /a LEN+=1
	    set STR=!STR:~0,-1!
	    goto :strlen_loop
    )
    endlocal && set %RET%=%LEN%
exit /b

::----------------------------------------------
:: 変数値の表示
::	arg1:	左インデント数
::	arg2:	変数名最大幅 (0=最大変数幅)
::	arg3:	変数のリスト
::
:values_dump
    setlocal
    set OUTTO=%1&& shift
    set INDENT=%1&& shift
    rem set /a MAXWIDTH=%1+2 && shift
    set MAXWIDTH=%1 && shift
    set LIST=%1 && shift
:values_dump_loop
    if "%1" neq "" (
	    set LIST=!LIST! %1
	    shift
	    goto :values_dump_loop
    )
    if %MAXWIDTH% leq 0 call :max_length MAXWIDTH %LIST%
        set /a MAXWIDTH+=2
        call :spaces SPACES %INDENT%
        if "%OUTTO%" equ "STDOUT" (
	        for %%v in (%LIST%) do (
	            set VAR=%%v
	            call :fixlen VAR %MAXWIDTH%
		        set VAL=VALUES[%%v]
		        call set VAL=%%!VAL!%%
		        call :display_string STR !VAL!
		        echo %SPACES%!VAR! !STR!
	        )
        ) else (
	    echo setlocal enabledelayedexpansion>> %OUTTO%
	    for %%v in (%LIST%) do (
	        set VAR=%%v
	        call :fixlen VAR %MAXWIDTH%
	        echo set VAL=%%%%v%%>> %OUTTO%
	        echo call :display_string STR %%VAL%%>> %OUTTO%
	        rem echo echo %SPACES%!VAR! %%%%v%% >> %OUTTO%
	        echo echo %SPACES%!VAR! %%STR%%>> %OUTTO%
	    )
	    echo endlocal>> %OUTTO%
    )
    endlocal
exit /b

::----------------------------------------------
:: 最大文字列長を求める
::	arg1:	結果を格納する変数名
::	arg2..:	変数名のリスト
::
:max_length
    setlocal
    set VAR=%1&& shift
    set LIST=
:max_length_loop
    if "%1" neq "" (
	    set LIST=!LIST! %1
	    shift
	    goto :max_length_loop
    )
    set LIST=%LIST:~1%
    set MAXLEN=0
    for %%v in (%LIST%) do (
	    call :strlen LEN %%v
	    if !LEN! gtr !MAXLEN! set MAXLEN=!LEN!
    )
    rem set /a MAXLEN+=1
    endlocal && set %VAR%=%MAXLEN%
exit /b

::----------------------------------------------
:: 環境変数値表示文字列の作成
::	arg1:	結果を格納する変数名
::	arg2:	環境変数値
::
:display_string
    if "%2" equ "skip" (
	    set %1=.... skip
    ) else (
	    set %1=exec
    )
exit /b

::----------------------------------------------
:: 使用方法の表示
::
:usage
    echo Usage: %PROG% [/a] [/k] [/h]
    echo.        /a      Executes all steps.
    echo.        /k      Use current values as a base set.
    echo.        /h      Print this.
    set $status=-1
exit /b
