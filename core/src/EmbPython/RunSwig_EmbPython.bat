@echo off
:: ****************************************************************************
::  File:
::      RunSwig_EmbPython.bat
::
::  Description:
::      ファイルの依存関係を調べて、EmbPythonSwig.bat を最適に実行する.
::	実行するプロジェクトは ..\..\src\RunSwig\do_swigall.projs に定義する.
::      ただしプロジェクト Base は定義の有無に関わりなく実行する.
::
:: ****************************************************************************
::  Version:
::	Ver 1.0  2014/10/29 F.Kanehori  初版
::	Ver 1.1  2017/06/15 F.Kanehori  依存リストに ".i" ファイルを追加
::	Ver 2.0  2017/10/04 F.Kanehori  Visual Studio 2015 対応
::	Ver 3.0  2018/12/15 F.Kanehori  Visual Studio 2017 対応
::					nmake がパスに入っていることを要請
:: ****************************************************************************
setlocal enabledelayedexpansion
set PROG=%~n0
set CWD=%cd%
set DEBUG=0

:: ----------
::  各種定義
:: ----------
:: ディレクトリの定義
::
set BASEDIR=..\..
set BINDIR=%BASEDIR%\bin
set SRCDIR=%BASEDIR%\src
set INCDIR=%BASEDIR%\include
set ETCDIR=%SRCDIR%\RunSwig

set EMBPYTHONDIR=.

:: 依存関係にはないと見做すファイルの一覧
::
set EXCLUDES=

:: makefile に出力するときのパス
set INCDIROUT=..\..\include
set SRCDIROUT=..\..\src
set EMBDIROUT=..\..\src\EmbPython

:: 使用するファイル名
::
set PROJFILE=do_swigall.projs
set MAKEFILE=Makefile_EmbPython.swig

:: 使用するプログラム名
::
set MAKE=nmake
set SWIG=EmbPythonSwig.bat

:: 使用するパス
::
rem set X32=c:\Program Files
rem set X64=c:\Program Files (x86)
rem set ARCH=
rem if exist "%X32%" set ARCH=%X32%
rem if exist "%X64%" set ARCH=%X64%
for /f "usebackq tokens=*" %%a in (`where %MAKE%`) do set MAKEBIN=%%a
call :dirpart MAKEPATH "%MAKEBIN%"
if "%MAKEPATH%" equ "" (
    echo %PROG%: can not find '%MAKE%' path.
    exit /b
)
rem set SWIGPATH=%BINDIR%\swig
set SWIGPATH=%EMBPYTHONDIR%
set PATH=.;%SWIGPATH%;%MAKEPATH%;%PATH%

:: ------------------------------
::  処理するモジュール一覧を作成
:: ------------------------------
set PROJECTS=Base
for /f "tokens=1,*" %%m in (%ETCDIR%\%PROJFILE%) do set PROJECTS=!PROJECTS! %%m
if %DEBUG% == 1 echo Projects are: %PROJECTS%

:: ----------
::  処理開始
:: ----------
for %%p in (%PROJECTS%) do (
    echo   Project: %%p
    set MKFILE=%MAKEFILE%.%%p
    call :collect_headers %%p
    call :make_makefile %%p !MKFILE! "!INTERFS!" "!INCHDRS!" "!SRCHDRS!"
    cmd /c %MAKE% -f !MKFILE!
)

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:: ----------------------------------------------------------------------------
::  引数1 で与えられたプロジェクトのヘッダ情報を収集する
:: ----------------------------------------------------------------------------
:collect_headers
    set PROJECT=%1

    :: 依存ファイル情報を集める
    ::
    ::   swig interface files
    set INTERFS1=
    for %%f in (*.i) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set INTERFS1=!INTERFS1! %%~nxf
    )
    call :add_prefix "!INTERFS1!" %EMBDIROUT%
    set INTERFS1=%$string:\=/%
    set INTERFS2=
    for %%f in (Utility\*.i) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set INTERFS2=!INTERFS2! %%~nxf
    )
    call :add_prefix "!INTERFS2!" %EMBDIROUT%\Utility
    set INTERFS2=%$string:\=/%
    set INTERFS=!INTERFS1! !INTERFS2!
    if %DEBUG% == 1 echo INTERFS [%INTERFS%]

    ::  header files
    set INCHDRS=
    for %%f in (%INCDIR%\%PROJECT%\*.h) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set INCHDRS=!INCHDRS! %%~nxf
    )
    call :add_prefix "!INCHDRS:~1!" %INCDIROUT%\%PROJECT%
    set INCHDRS=%$string:\=/%
    if %DEBUG% == 1 echo INCHDRS [%INCHDRS%]

    ::  source files
    set SRCHDRS=
    for %%f in (%SRCDIR%\%PROJECT%\*.h) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set SRCHDRS=!SRCHDRS! %%~nxf
    )
    call :add_prefix "!SRCHDRS:~1!" %SRCDIROUT%\%PROJECT%
    set SRCHDRS=%$string:\=/%
    if %DEBUG% == 1 echo SRCHDRS [%SRCHDRS%]

exit /b

:: ----------------------------------------------------------------------------
::  makefile を作成する
::      引数1   モジュール名
::      引数2   makefile 名
::      引数3   "依存インターフェイスファイル名リスト"
::      引数4   "依存ヘッダファイル名リスト"
::      引数5   "依存ソースファイル名リスト"
:: ----------------------------------------------------------------------------
:make_makefile
    setlocal enabledelayedexpansion
    set MODULE=%1
    set MKFILE=%2
    set INTFILES=%~3
    set INCFILES=%~4
    set SRCFILES=%~5
    if %DEBUG% == 1 (
        echo MODULE   [%MODULE%]
        echo MKFILE   [%MKFILE%]
        echo INTFILES [%INTFILES%]
        echo INCFILES [%INCFILES%]
        echo SRCFILES [%SRCFILES%]
    )

    set TARGET=%EMBPYTHONDIR%/EP%MODULE%.cpp
    set DEPENDENCIES=%INTFILES% %INCFILES% %SRCFILES%
    if %DEBUG% == 1 (
        echo TARGET       [%TARGET%]
        echo DEPENDENCIES [%DEPENDENCIES%]
    )

    echo #  Do not edit. RunSwig_EmbPython.bat will update this file.   > %MKFILE%
    echo #  File: %MKFILE%  >> %MKFILE%
    echo.                   >> %MKFILE%
    echo all:	%TARGET%    >> %MKFILE%
    echo %TARGET%:	%DEPENDENCIES%  >> %MKFILE%
    echo.	call %SWIG% %MODULE%    >> %MKFILE%
    echo.                   >> %MKFILE%
    for %%f in (%DEPENDENCIES%) do (
        echo %%f:	    >> %MKFILE%
    )
    endlocal
exit /b

:: ----------------------------------------------------------------------------
::  引数2 で与えられた名前が 引数1 で与えられたリスト中にあるか調べる
::  結果は $result に返す（yes または no）
:: ----------------------------------------------------------------------------
:one_of
    set $result=no
    for %%f in (%~1) do (if "%2" equ "%%f" (set $result=yes&& exit /b))
exit /b

:: ----------------------------------------------------------------------------
::  引数1 で与えられた変数に、引数2 で指定された prefix を追加する
::  結果は $string に返す
:: ----------------------------------------------------------------------------
:add_prefix
    set $string=
    for %%f in (%~1) do (set $string=!$string! %2\%%f)
    set $string=%$string:~1%
exit /b

:: ----------------------------------------------------------------------------
::  引数2で与えられたパスのうち、ディレクトリ部分を引数1に設定する
:: ----------------------------------------------------------------------------
:dirpart
    set %1=%~dp2
exit /b

::end RunSwig_EmbPython.bat
