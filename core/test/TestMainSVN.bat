@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	TestMain [/t toolset_id] [/c config] [/p platform] [/r test_rep] [/h]
::
::  DESCRIPTION
::	dailybuild を実行する.
::	
::  ARGUMENTS
::	/t toolset_id	ツールセットの識別 { 14.0 <default> | .. }
::	/c config	ビルド構成 { Debug | Release <default> | .. }
::	/p platform	ビルドプラットフォーム { Win32 | x64 <default> }
::	/r test_rep	作業リポジトリのルート <default: SpringheadTest>
::	/h              使用方法の表示
::
::  CAUTION
::	・/r オプションで指定するディレクトリは, ..\..\ を起点として指定する.
::	・/r オプションで指定したディレクトリがすでに存在するときは, その内容
::	　はすべて破棄される.
::	・Visual Studio 10.0 以降のみに対応
::
::  VERSION
::	Ver 1.0  2013/09/18 F.Kanehori	バッチファイルの再構成
::	Ver 1.1	 2013/10/09 F.Kanehori	svn co に失敗したら処理を中止する
::	Ver 1.2	 2013/11/20 F.Kanehori	外部公開用アーカイブを作成する
::	Ver 1.3	 2014/10/23 F.Kanehori	引数 /y 追加
::	Ver 1.4  2014/11/20 F.Kanehori	デフォルト変更 (toolset_id, python_ver)
::	Ver 1.5  2016/06/02 F.Kanehori	新制御変数の導入 (.._ELIMINATE_CLOSED)
::	Ver 2.0  2017/07/31 F.Kanehori	Directory構造及びデフォルトの変更
::	Ver 2.0  2017/08/30 F.Kanehori	デフォルト変更 (x64, Release)
::	Ver 3.0  2017/09/04 F.Kanehori	新ツリー構造に対応, python_ver 廃止
::	Ver 3.1  2017/11/20 F.Kanehori	ドキュメント生成場所変更に対応
::	Ver 3.11 2017/11/22 F.Kanehori	開始/終了時刻を表示
::	Ver 3.12 2017/11/22 F.Kanehori	Bug fixed.
:: ============================================================================
set PROG=%~n0
set CWD=%cd%
echo %PROG%: started at %date% %time%

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
if not exist ..\..\..\%TEST_REPOSITORY% (
    echo %PROG%: test repository "%TEST_REPOSITORY%" does not exist!
    rem set $status=-1
    rem goto :done
)

echo Test parameters:
echo.   TOOLSET_ID:      [%TOOLSET_ID%]
echo.   CONFIGURATION:   [%CONFIGURATION%]
echo.   PLATFORM:        [%PLATFORM%]
echo.   TEST_REPOSITORY: [%TEST_REPOSITORY%]

::----------------------------------------------
:: Springhead
::
cd ..\..
call :check_condition DAILYBUILD_UPDATE_SPRINGHEAD
if %$status% == 0 (
    echo updating local repository
    svn up
)

::----------------------------------------------
:: SpringheadTest にソースツリーを取得する
::
cd ..
call :check_condition DAILYBUILD_CLEANUP_WORKSPACE
if %$status% == 0 (
    echo clearing workspace
    rmdir /s /q %TEST_REPOSITORY% 2>NUL
    mkdir %TEST_REPOSITORY%
    set URL=http://springhead.info/spr2/Springhead/trunk
    svn co !URL! %TEST_REPOSITORY%
    if ERRORLEVEL 1 (
	    echo %PROG%: abort ^(svn checkout failed^)
	    set $status=%ERRORLEVEL%
	    goto :done
    )
)

::----------------------------------------------
:: ビルドとテストは次のフォルダで行なう
::
echo changing directory to [%TEST_REPOSITORY%]
cd /D %TEST_REPOSITORY%
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
    call bat\TestAll.bat %TOOLSET_ID% %CONFIGURATION% %PLATFORM% %TEST_REPOSITORY%
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
)

::----------------------------------------------
:: ソースツリー(%TEST_REPOSITORY%以下)を Web にコピー
:: ************************************************
::　ソースツリーのコピーは GitHub 移行後は廃止する
:: ************************************************
::
set WEBBASE=\\haselab\HomeDirs\WWW\docroots\springhead\daily_build

call :check_condition DAILYBUILD_COPYTO_WEBBASE
if %$status% == 0 (
    echo copying source tree to web
    cd ..\src
    call DelAll.bat

    cd ..
    set DIRLIST=bin doc include resource ruby src test tools
    set FILELIST=archive.bat make.header Makefile MakePack.bat ReadMe.txt
    echo WEBBASE:  [!WEBBASE!]
    echo DIRLIST:  [!DIRLIST!]
    echo FILELIST: [!FILELIST!]
    for %%d in (!DIRLIST!) do call :copy_dir %%d !WEBBASE!
    for %%f in (!FILELIST!) do call :copy_file %%f !WEBBASE!
    cd test
)

::----------------------------------------------
:: dailybuild で生成されたファイルを Web にコピー
::
set WEBBASE_G=\\haselab\HomeDirs\WWW\docroots\springhead\dailybuild_generated

call :check_condition DAILYBUILD_COPYTO_WEBBASE
if %$status% == 0 (
    echo copying generated files to web

    cd ..\..\generated
    set DIRLIST=bin doc lib
    set FILELIST=
    echo WEBBASE:  [!WEBBASE_G!]
    echo DIRLIST:  [!DIRLIST!]
    echo FILELIST: [!FILELIST!]
    for %%d in (!DIRLIST!) do call :copy_dir %%d !WEBBASE_G!
    for %%f in (!FILELIST!) do call :copy_file %%f !WEBBASE_G!
    cd ..\core\test
)

::----------------------------------------------
:: 外部公開用のアーカイブを作る
::
call :check_condition DAILYBUILD_MAKE_ARCHIVE
if %$status% == 0 (
    echo making archive file
    cd ..
    call MakePack.bat
    call :copy_file pack\Springhead.tgz %WEBBASE%
    cd test
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
echo %PROG%: teminated at %date% %time%
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
	rmdir /s /q %2\%1 > NUL
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
    echo Usage: %PROG% [/t toolset] [/c config] [/p platform] [/r test_rep] [/h]
    echo        /t toolset         ツールセットの識別 { 14.0 ^<default^> ^| .. }
    echo        /c configuration   ビルド構成 { Debug ^| Release ^<default^> ^| .. }
    echo        /p platform        ビルドプラットフォーム { Win32 ^| x64 ^<default^>}
    echo        /r test_rep        作業リポジトリのルート ^<default: SpringheadTest^>
    echo        /h                 使用方法の表示
exit /b

