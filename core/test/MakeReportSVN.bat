@echo off
setlocal enabledelayedexpansion
:: ==============================================================================
::  SYNOPSIS
::	MakeReport [-k] [-h] [-v] [-s] [-D level]
::
::  DESCRIPTION:
::	daily_build のレポートファイルを作成する. 次のコマンドがローカルで動作
::	する必要がある (これらは bin ディレクトリに置くこと).
::	    gawk.exe	( awk の代替物ならなんでもよい )
::	    nkf.exe
::
::  VERSION:
::	Ver 1.0	 2012/11/03 F.Kanehori	初版 (Unix 呼出し版)
::	Ver 2.0  2013/02/07 F.Kanehori	Windows 移植初版
::	Ver 3.0  2017/10/18 F.Kanehori	新レポジトリ構造対応
::	Ver 3.1  2017/11/22 F.Kanehori	開始/終了時刻を表示
::	Ver 3.11 2017/12/06 F.Kanehori	Bug fixed
:: ==============================================================================
set PROG=%~n0
echo %PROG%: started at %date% %time%

::----------------------------------------------
::  下位の処理プログラムに渡すオプション
::    -k  作業ファイルを残す
::    -V  バーバス情報出力（デフォルト）
::    -D  デバッグ情報出力
::  このバッチスクリプトで使用するオプション
::    -c  レポートファイルをウェブにコピーする
::    -h  使用方法の表示
::    -v  バーバス情報出力（デフォルト）
::    -s  バーバス情報出力抑制（全スクリプト）
::----------------------------------------------
set OPT_C=0
set OPT_K=0
set OPT_S=0
set OPT_V=1
set OPT_W=0
set OPT_D=0
set VAL_D=0
set ARGC=0
:next_arg
       if "%1" == "-c" ( set OPT_C=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-k" ( set OPT_K=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if /i "%1" == "-h" ( call :usage && exit /b
) else if /i "%1" == "/h" ( call :usage && exit /b
) else if "%1" == "-V" ( set OPT_W=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-v" ( set OPT_V=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-s" ( set OPT_S=1 && shift && set /a ARGC-=1 && goto :next_arg
) else if "%1" == "-D" ( set OPT_D=1 && set VAL_D=%2&& shift && shift && set /a ARGC-=2 && goto :next_arg
) else (
    set ARG=%1
    if "!ARG:~0,1!" == "-" (
	echo %PROG%: invalid option -- [%1]
	call :usage && exit /b
    )
)
if %OPT_S% == 1 ( set OPT_V=0 && set OPT_W=0)
for %%a in ( %* ) do set /a ARGC+=1
if %ARGC% gtr 1 (
    call :usage && exit /b
)
set ARGS=
if %OPT_K% == 1 (set ARGS=%ARGS% -k)
if %OPT_V% == 1 (set ARGS=%ARGS% -v)
if %OPT_W% == 1 (set ARGS=%ARGS% -V)
if %OPT_D% == 1 (set ARGS=%ARGS% -D %VAL_D%)
if "%ARGS:~0,1%" == " " (
    set ARGS=!ARGS:~1!
)
if %ARGC% == 1 (set ARGS=%ARGS% %1)

::----------------------------------------------
::  日付情報の取得
::
set YYYY=%DATE:~0,4%
set MM=%DATE:~5,2%
set DD=%DATE:~8,2%
set TODAY=%YYYY%-%MM%%DD%

::----------------------------------------------
::  使用するプログラム
::
set BASEDIR=.\Monitoring
set BINDIR=.\bin
set CMND=build_monitor_SVN

::----------------------------------------------
::  ローカルホスト上でコマンドを実行
::
set CWD=%cd%
cd %BASEDIR%
cmd /c %BINDIR%\%CMND% %ARGS%
cd %CWD%

::----------------------------------------------
::  コピー元/先のディレクトリの定義
::
set SELFPATH=%~dp0
set REPBASE=%SELFPATH%report
set WEBBASE=\\haselab\HomeDirs\WWW\docroots\springhead\daily_build
set WEBREPT=%WEBBASE%\report

::----------------------------------------------
::  ウェブにコピーするファイルの定義
::
set REPFILE=%TODAY%.report
set BLDDIFF=%TODAY%.bldlog.diff
set STBDIFF=%TODAY%.stblog.diff
set RUNDIFF=%TODAY%.runlog.diff
set SPLDIFF=%TODAY%.spllog.diff

:: ウェブ上での名称
::
set WEB_REPFILE=Test.report
set WEB_BLDDIFF=Build.log.diff
set WEB_STBDIFF=StubBuild.log.diff
set WEB_RUNDIFF=Run.log.diff
set WEB_SPLDIFF=SamplesBuild.log.diff

::----------------------------------------------
::  ウェブにコピー
::
if %OPT_C% == 1 (
    if %OPT_V% == 1 ( echo copying files to the web ... )
    copy /Y %REPBASE%\%REPFILE% %WEBBASE%\%WEB_REPFILE%
    copy /Y %REPBASE%\%BLDDIFF% %WEBREPT%\%WEB_BLDDIFF%
    copy /Y %REPBASE%\%STBDIFF% %WEBREPT%\%WEB_STBDIFF%
    copy /Y %REPBASE%\%RUNDIFF% %WEBREPT%\%WEB_RUNDIFF%
    copy /Y %REPBASE%\%SPLDIFF% %WEBREPT%\%WEB_SPLDIFF%
    if %OPT_V% == 1 ( echo done )
)

::----------------------------------------------
::  ローカルのファイルを削除
::
if not %OPT_K% == 1 (
    if %OPT_V% == 1 ( set /p=clearing directory %REPBASE% ... < NUL )
    rem del %REPBASE%\%REPFILE% 2> NUL
    del %REPBASE%\%BLDDIFF% 2> NUL
    del %REPBASE%\%STBDIFF% 2> NUL
    del %REPBASE%\%RUNDIFF% 2> NUL
    del %REPBASE%\%SPLDIFF% 2> NUL
    if %OPT_V% == 1 ( echo done )
)

echo %PROG%: teminated at %date% %time%
endlocal
exit /b

::----------------------------------------------
::  使用方法の表示
::----------------------------------------------
:usage
    echo Usage: %PROG% [options] [xxxx:yyyy]
    echo 	xxxx: old-revision-number
    echo 	yyyy: new-revision-number
    echo 	options:
    echo 	  -c:	copy report file to the web
    echo 	  -k:	keep temporary files
    echo 	  -h:	show usage
    echo 	  -v:	set verbose mode
    echo 	  -V:	set precise verbose mode
    echo 	  -s:	set silent mode
    echo 	  -D n:	set debug level to 'n'
exit /b

:: end: MkaeReport.bat
