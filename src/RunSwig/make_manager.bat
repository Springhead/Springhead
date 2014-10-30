@echo off
:: ***********************************************************************************
::  File:
::	    make_manager.bat [-A] {-c -d -r -t}
::
::  Description:
::	    各プロジェクトで実行する make の制御を行なう.
::
::	引数：	-A	対象となるすべてのプロジェクトについて, 以降の引数で示された
::			処理を実行する.
::
::		以下, 個別のプロジェクトに対する処理
::		    -c	Makefile.swig がなければ作成する.
::		    -d	Makefile.swig を削除する.
::		    -r	Makefile.swig.tmp があれば, それを Makefile.swig とする.
::		    -t	Makefile.swig.tmp を作成する (テンポラリファイル).
::
::	    テンポラリファイルの作成モードがあるのは, make の実行中に Makefile.swig を
::	    書き換えてしまうのを回避するため.
::
:: ***********************************************************************************
::  Version:
::	Ver 1.0	 2013/01/07 F.Kanehori	初版
::	Ver 1.1	 2013/03/13 F.Kanehori	モジュール識別文字列一部変更
:: ***********************************************************************************
setlocal enabledelayedexpansion
set CWD=%cd%
set DEBUG=0

:: ----------
::  各種定義
:: ----------
:: 使用するプロシージャ
::
set MAKEMGR=make_manager.bat
set CREATEMKF=create_mkf.bat

:: 使用するファイル名
::	%MAKEFILE% は RunSwig.bat を実行するための makefile 名
::	%MAKETEMP% は makefile 更新のためのテンポラリファイル名
::	%PROJFILE% は 依存プロジェクト定義ファイル名
::
set MAKEFILE=Makefile.swig
set MAKETEMP=Makefile.swig.tmp
set PROJFILE=do_swigall.projs
set PROJFILEONE=do_swigone.projs

:: ディレクトリの定義
::
set SRCDIR=..\..\src
set BINDIR=..\..\src\RunSwig
set ETCDIR=..\..\src\RunSwig

:: ------------------------
::  -A オプションの処理
:: ------------------------
if "%1" equ "-A" (
    set NEEDONEFILE=
    if "%2" equ "-c" set NEEDONEFILE=1
    if "%3" equ "-c" set NEEDONEFILE=1
    if "%2" equ "-t" set NEEDONEFILE=1

    if !NEEDONEFILE! == 1 echo checking makefile existance...
    for /f "tokens=1,*" %%p in (%ETCDIR%\%PROJFILE%) do (
	    cd %SRCDIR%\%%p
	    if !NEEDONEFILE! == 1 (
	        echo.
	        echo     ***  %%p  ***
	        echo %%p %%q > %PROJFILEONE%
	    )
	    cmd /c %BINDIR%\%MAKEMGR% %2 %3 %4 %5 %6 %7 %8 %9
	    cd %CWD%
    )
    :: 終了
    exit /b
)

:: ------------------------
::  個別プロジェクトの処理
:: ------------------------
::  引数の解析
::
set ARG_C=0
set ARG_D=0
set ARG_R=0
set ARG_T=0
:ana_args
    if "%1" equ "-c" (set ARG_C=1
    ) else if "%1" equ "-d" (set ARG_D=1
    ) else if "%1" equ "-r" (set ARG_R=1
    ) else if "%1" equ "-t" (set ARG_T=1
    ) else goto :end_args
    shift
    goto :ana_args
:end_args

set /a NUM_ARGS=%ARG_C%+%ARG_D%+%ARG_R%+%ARG_T%
if %NUM_ARGS% == 0 (
    echo Usage: %0 {-A ^| -c ^| -d ^| -t ^| -r}
    exit /b
)

if %DEBUG% == 1 (
    set ARGS=
    if %ARG_C% == 1 set ARGS=!ARGS! -c
    if %ARG_D% == 1 set ARGS=!ARGS! -d
    if %ARG_R% == 1 set ARGS=!ARGS! -r
    if %ARG_T% == 1 set ARGS=!ARGS! -t
    echo arguments:!ARGS!
)

:: ------------------------
::  依存プロジェクトの取得
:: ------------------------
::    PROJ = プロジェクト名
::    DEPT = 依存プロジェクト名（カンマ区切り）
::
set /a INIT=%ARG_C%+%ARG_T%
set PROJ=
set DEPT=
if %INIT% gtr 0 (
    call :leaf_name %cd%
    for /f "tokens=1,*" %%p in (%PROJFILEONE%) do (
        if !PROJ! equ %%p (
            set DEPT="%%q"
	        if %DEBUG% == 1 (
		        echo.  project=!PROJ!
		        echo.  dependencies=!DEPT!
	        )
	        goto :exit_for
	    )
    )
    if "%DEPT%" equ "" (
	    echo make_manager: can not get dependencies.
	    exit /b
    )
)
:exit_for

:: ------------------------------------
::  引数 -d の処理： %MAKEFILE% を削除
:: ------------------------------------
::
if %ARG_D% == 1 (
    if exist %MAKEFILE% (
	    if %DEBUG% == 1 echo removing "%MAKEFILE%"
	    del %MAKEFILE%
    )
)

:: ------------------------------------
::  引数 -c の処理： %MAKEFILE% を作成
:: ------------------------------------
::
if %ARG_C% == 1 (
    if not exist %MAKEFILE% (
    	if %DEBUG% == 1 echo creating "%MAKEFILE%"
    	cmd /c %BINDIR%\%CREATEMKF% %MAKEFILE% %PROJ% %DEPT%
    )
)

:: ------------------------------------
::  引数 -t の処理： %MAKETEMP% を作成
:: ------------------------------------
::
if %ARG_T% == 1 (
    echo     temporary creation mode
    if exist %MAKETEMP% del %MAKETEMP%
    if %DEBUG% == 1 echo creating "%MAKETEMP%"
    cmd /c %BINDIR%\%CREATEMKF% %MAKETEMP% %PROJ% %DEPT%
)

:: --------------------------------------------------
::  引数 -r の処理： %MAKETEMP% を %MAKEFILE% に変更
:: --------------------------------------------------
::
if %ARG_R% == 1 (
    if exist %MAKETEMP% (
	    if %DEBUG% == 1 echo renaming "%MAKETEMP%" to "%MAKEFILE%"
	    if exist %MAKEFILE% del %MAKEFILE%
	    rename %MAKETEMP% %MAKEFILE%
    )
)

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:: ============================================
::  サブルーチン：
::	call :leaf_name path	リーフ名の取得
::	結果は環境変数 PROJ に設定する
:: ============================================
:leaf_name
    set PROJ=%~nx1
exit /b

:end make_manager.bat
