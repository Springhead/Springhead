@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	TestMainGit [/t toolset_id] [/c config] [/p platform] [/h]
::
::  DESCRIPTION
::	dailybuild を実行する.
::	
::  ARGUMENTS
::	/t toolset_id	ツールセットの識別 { 14.0 <default> | .. }
::	/c config	ビルド構成 { Debug | Release <default> | .. }
::	/p platform	ビルドプラットフォーム { Win32 | x64 <default> }
::	/r repository	テストレポジトリ指定
::	/h              使用方法の表示
::
::  VERSION
::	Ver 1.0  2017/12/03 F.Kanehori	GitHub 仮対応版（Python版を作るまで）。
::	Ver 1.1  2017/12/18 F.Kanehori	Springhead マニュアル作成追加
:: ============================================================================
set PROG=%~n0
set CWD=%cd%

::----------------------------------------------
:: 引数の処理
::
set TOOLSET_ID=14.0
set CONFIGURATION=Release
set PLATFORM=x64
set TEST_REPOSITORY=SpringheadTest
call :get_args %*
if not %$status% == 0 (
    goto :done
)
echo Test parameters:
echo.   TOOLSET_ID:      [%TOOLSET_ID%]
echo.   CONFIGURATION:   [%CONFIGURATION%]
echo.   PLATFORM:        [%PLATFORM%]
echo.   TEST_REPOSITORY: [%TEST_REPOSITORY%]

::----------------------------------------------
:: テストリポジトリのチェック
::
if not exist ..\..\..\%TEST_REPOSITORY% (
    echo %PROG%: test repository "%TEST_REPOSITORY%" does not exist!
    set $status=-1
    goto :done
)
cd ..\..\..\%TEST_REPOSITORY%

::----------------------------------------------
:: ビルドとテストは次のフォルダで行なう
::
set MAYBE_EMPTY=0
if not exist core\test       set MAYBE_EMPTY=1
if not exist core\test\bat   set MAYBE_EMPTY=1
if %MAYBE_EMPTY% == 1 (
    echo %PROG%: test repository "%TEST_REPOSITORY%\core" may be empty
    set $status=%ERRORLEVEL%
    goto :done
)

::----------------------------------------------
:: 公開できないファイルの削除と設定変更
::
call :check_condition DAILYBUILD_ELIMINATE_CLOSED
if %$status% == 0 (
    rem echo removing closed files
    rem del /F /S /Q closed
    echo unset USE_CLOSED_SRC flag
    echo #undef USE_CLOSED_SRC > core/include/UseClosedSrcOrNot.h
)

::----------------------------------------------
:: テストを行なう
::
cd core\test
call :check_condition DAILYBUILD_EXECUTE_TESTALL
if %$status% == 0 (
    call bat\TestAllGit.bat %TOOLSET_ID% %CONFIGURATION% %PLATFORM% %TEST_REPOSITORY%
    if not !$status! == 0 (
	goto :done
    )
)

::----------------------------------------------
:: ドキュメント(doxygen)を作る
::
call :check_condition DAILYBUILD_EXECUTE_MAKEDOC
if %$status% == 0 (
    echo making documents
    call bat\MakeDoc.bat

    path="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin";%path%
    cd ..\doc\SprManual
    make
    cd ..\..\test
)

::----------------------------------------------
:: dailybuild で生成されたファイルを Web にコピー
::
set WEBBASE=\\haselab\HomeDirs\WWW\docroots\springhead\dailybuild_generated

call :check_condition DAILYBUILD_COPYTO_WEBBASE
if %$status% == 0 (
    echo copying generated files to web

    cd ..\..\generated
    set DIRLIST=bin doc lib
    set FILELIST=
    echo DIRLIST:  [!DIRLIST!]
    echo FILELIST: [!FILELIST!]
    for %%d in (!DIRLIST!) do call :copy_dir %%d !WEBBASE!
    for %%f in (!FILELIST!) do call :copy_file %%f !WEBBASE!
    cd ..\core\test
)

::----------------------------------------------
:: OK
::
set $status=0
echo done.

::----------------------------------------------
:: 処理終了
::
:done
endlocal && set $status=%$status%
exit /b


::=============================================================================
::----------------------------------------------
:: コマンド引数の処理
::
:get_args
    set $status=0
:get_args_loop
    if "%1" equ "" goto :get_args_exit
    if /i "%1" equ "/t" ((set TOOLSET_ID=%2) && shift && shift && goto :get_args_loop)
    if /i "%1" equ "/c" ((set CONFIGURATION=%2) && shift && shift && goto :get_args_loop)
    if /i "%1" equ "/p" ((set PLATFORM=%2) && shift && shift && goto :get_args_loop)
    if /i "%1" equ "/r" ((set TEST_REPOSITORY=%2) && shift && shift && goto :get_args_loop)
    if /i "%1" equ "/h" (call :usage && set $status=-1 && goto :get_args_exit)
    echo bad arg [%1] && set $status=-1 && shift
    goto :get_args_loop
:get_args_exit
exit /b

::----------------------------------------------
:: ディレクトリ全体のコピー
::
:copy_dir
    echo copying directory %cd%\%1\ to %2\%1\
    if "%1" neq "" (
	rem rmdir /s /q %2\%1 > NUL
	if exist %2\%1 (
		cd %2\%1
		for /f %%d in ('dir /ad /b /w *') do rmdir /s /q %%d
		del /f /q *
		cd ..\..
	)
	xcopy /e/c/f/h/i/y %1 %2\%1 > NUL
    )
exit /b

::----------------------------------------------
:: ファイルのコピー
::
:copy_file
    echo copying file %cd%\%1 to %WEBBASE%\%1
    if "%1" neq "" (
	del %WEBBASE%\%1 > NUL
	copy /y %1 %WEBBASE%\%1 > NUL
    )
exit /b

::----------------------------------------------
:: 環境変数による実行制御
::	arg1:	実行環境制御変数名
::
:check_condition
    setlocal
    set RET=0
    call set VAL=%%%1%%
    if "%VAL%" equ "skip" (
	if "%2" neq "silent" (
	    echo skip ..%1..
	)
	set RET=-1
    ) 
    endlocal && set $status=%RET%
exit /b

::----------------------------------------------
:: 使用方法の表示
::
:usage
    echo Usage: %PROG% [/t toolset] [/c config] [/p platform] [/h]
    echo        /t toolset         ツールセットの識別 { 14.0 ^<default^> ^| .. }
    echo        /c configuration   ビルド構成 { Debug ^| Release ^<default^> ^| .. }
    echo        /p platform        ビルドプラットフォーム { Win32 ^| x64 ^<default^>}
    echo        /r                 テストリポジトリ (SpringheadTest ^<default^>)
    echo        /h                 使用方法の表示
exit /b

::end TestMainGit.bat
