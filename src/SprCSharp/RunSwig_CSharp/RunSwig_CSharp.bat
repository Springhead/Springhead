@echo off
:: ***********************************************************************************
::  File:
::      RunSwig_CSharp.bat
::
::  SYNOPSIS:
::	RunSwig_CSharp [:export]
::
::  Description:
::      ファイルの依存関係を調べて、CSharpSWig.bat を最適に実行する.
::	※ このスクリプトで作成した makefile を実行する.
::
::    　実行するプロジェクトは ..\..\src\RunSwig\do_swigall.projs に定義されている
::      ものを使用する. ただしプロジェクト Base は定義の有無に関わりなく実行する.
::
::  ARGUMENTS:
::	:export		共通に使用する名前を export する.
::	
:: ***********************************************************************************
::  Version:
::	Ver 2.0	  2016/02/08	F.Kanehori  wrapper file をまとめた
::	Ver 1.0	  2015/03/18	F.Kanehori  初版
:: ***********************************************************************************
setlocal enabledelayedexpansion
set PROG=%~n0
set CWD=%cd%
set DEBUG=0

:: ----------
::  各種定義
:: ----------
:: ディレクトリの定義
::
set TOPDIR=..\..\..
set BINDIR=%TOPDIR%\bin
set SRCDIR=%TOPDIR%\src
set INCDIR=%TOPDIR%\include
set ETCDIR=%SRCDIR%\RunSwig

set CSBASE=.
set SUBDIR_SRC=SprCSharp
set SUBDIR_IMP=SprImport
set SUBDIR_EXP=SprExport
set CS_SRC=%CSBASE%/%SUBDIR_SRC%
set CS_IMP=%CSBASE%/%SUBDIR_IMP%
set CS_EXP=%CSBASE%/%SUBDIR_EXP%
if %DEBUG% == 1 (
    call :show_abs_path INCDIR %INCDIR%
    call :show_abs_path SRCDIR %SRCDIR%
    call :show_abs_path ETCDIR %ETCDIR%
    call :show_abs_path CS_SRC %CS_SRC%
    call :show_abs_path CS_IMP %CS_IMP%
    call :show_abs_path CS_EXP %CS_EXP%
    echo. 
)

:: ---------------------------------------------
::  RunSwig_CSharp.bat と共通で使用する環境変数
::
::  wrapper file が作成されたことを示すファイル名
set WRAPPERS_BUILT=wrappers.built
::
::  モジュール毎に作成する wrapper file の名前
set MODULE_WRAPPER_SRC=wrapper.cs
set MODULE_WRAPPER_IMP=wrapper.cs
set MODULE_WRAPPER_EXP=wrapper.cpp
::
if "%1" equ ":export" goto :export
:: ---------------------------------------------

:: 依存関係にはないと見做すファイルの一覧
::
set EXCLUDES=

:: makefile に出力するときのパス
set INCDIROUT=..\..\include
set SRCDIROUT=..\..\src

:: 使用するファイル名
::
set PROJFILE=do_swigall.projs
set MAKEFILE=Makefile_CSharp.swig
::
::  wrapper file の名前
set WRAPPERFILE_SRC=module.wrapper.cs
set WRAPPERFILE_IMP=module.wrapper.cs
set WRAPPERFILE_EXP=module.wrapper.cpp
::
::  関数の多重生成を防ぐためにモジュール間で情報を受け渡すファイル
set SIGNATURE=..\swig_sprcs.signature

:: 使用するプログラム名
::
set MAKE=nmake
set SWIG=RunSwig_CSharp\CSharpSwig.bat

:: 使用するパス
::
set X32=c:\Program Files
set X64=c:\Program Files (x86)
set ARCH=
if exist "%X32%" set ARCH=%X32%
if exist "%X64%" set ARCH=%X64%
set VSVER=
if exist "%ARCH%\Microsoft Visual Studio 10.0" set VSVER=10.0
if exist "%ARCH%\Microsoft Visual Studio 12.0" set VSVER=12.0
set MAKEPATH="%ARCH%\Microsoft Visual Studio %VSVER%\VC\bin"
if not exist %MAKEPATH% (
    echo %PROG%: can not find '%MAKE%' path.
    exit /b
)
set SWIGPATH=%BINDIR%\swig
set PATH=.;%SWIGPATH%;%MAKEPATH%;%PATH%

:: ------------------------------
::  処理するモジュール一覧を作成
:: ------------------------------
set PROJECTS=Base
for /f "tokens=1,*" %%m in (%ETCDIR%\%PROJFILE%) do set PROJECTS=!PROJECTS! %%m
if %DEBUG% == 1 echo Projects are: %PROJECTS%

:: ------------------------------
::  .i ファイルの一覧を作成
:: ------------------------------
set IFILES=
for %%p in (%PROJECTS%) do (
    set IFILES=!IFILES! ./%%p.i
)
set IFILES=%IFILES:~1%

:: ------------------------------
::  モジュールにまたがる初期化
:: ------------------------------
call :truncate_file %SIGNATURE%
del /f ..\%WRAPPERS_BUILT% > NUL 2>&1

:: ----------
::  処理開始
:: ----------
for %%p in (%PROJECTS%) do (
    echo   Project: %%p
    set MKFILE=%MAKEFILE%.%%p
    call :collect_headers %%p
    call :make_makefile %%p ..\!MKFILE! "!INCHDRS!" "!SRCHDRS!" "%IFILES%"
    cd ..
    cmd /c %MAKE% -f !MKFILE!
    cd %CWD%
)

:: -------------------------
::  wrapper file をまとめる
:: -------------------------
if exist ..\%WRAPPERS_BUILT% (
    echo combining wrapper files
    set WF_SRC=%CS_SRC:/=\%\%WRAPPERFILE_SRC%
    set WF_IMP=%CS_IMP:/=\%\%WRAPPERFILE_IMP%
    set WF_EXP=%CS_EXP:/=\%\%WRAPPERFILE_EXP%
    type ..\!WF_SRC!.prologue > ..\!WF_SRC!
    type ..\!WF_IMP!.prologue > ..\!WF_IMP!
    type ..\!WF_EXP!.prologue > ..\!WF_EXP!
    for %%p in (%PROJECTS%) do (
        call :append_file ..\%SUBDIR_SRC%\tmp\CS%%p.%MODULE_WRAPPER_SRC% ..\!WF_SRC!
        call :append_file ..\%SUBDIR_IMP%\tmp\CS%%p.%MODULE_WRAPPER_IMP% ..\!WF_IMP!
        call :append_file ..\%SUBDIR_EXP%\tmp\CS%%p.%MODULE_WRAPPER_EXP% ..\!WF_EXP!
    )
    echo } >> ..\!WF_SRC!
    echo } >> ..\!WF_IMP!
    echo } >> ..\!WF_EXP!
)

:: ----------
::  処理終了
:: ----------

endlocal
exit /b

:: -----------------------------------------------------------------------------------
::  ファイルの初期化（空のファイルとする）
:: -----------------------------------------------------------------------------------
:truncate_file
    if exist %1 del /F %1
    type NUL > %1
exit /b

:: -----------------------------------------------------------------------------------
::  ファイルへの追加
:: -----------------------------------------------------------------------------------
:append_file
    if exist %1 type %1 >> %2
exit /b

:: -----------------------------------------------------------------------------------
::  引数1 で与えられたプロジェクトのヘッダ情報を収集する
:: -----------------------------------------------------------------------------------
:collect_headers
    set PROJECT=%1

    :: 依存ファイル情報を集める
    ::
    set INCHDRS=
    for %%f in (%INCDIR%\%PROJECT%\*.h) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set INCHDRS=!INCHDRS! %%~nxf
    )
    call :add_prefix "!INCHDRS:~1!" %INCDIROUT%\%PROJECT%
    set INCHDRS=%$string:\=/%
    if %DEBUG% == 1 echo INCHDRS  [%INCHDRS%]

    set SRCHDRS=
    for %%f in (%SRCDIR%\%PROJECT%\*.h) do (
        call :one_of "%EXCLUDES%" %%~nxf
        if "!$result!" equ "no" set SRCHDRS=!SRCHDRS! %%~nxf
    )
    call :add_prefix "!SRCHDRS:~1!" %SRCDIROUT%\%PROJECT%
    set SRCHDRS=%$string:\=/%
    if %DEBUG% == 1 echo SRCHDRS  [%SRCHDRS%]

exit /b

:: -----------------------------------------------------------------------------------
::  makefile を作成する
::      引数1   モジュール名
::      引数2   makefile 名
::      引数3   "依存ヘッダファイル名リスト"
::      引数4   "依存ソースファイル名リスト"
:: -----------------------------------------------------------------------------------
:make_makefile
    setlocal enabledelayedexpansion
    set MODULE=%1
    set MKFILE=%2
    set INCHDRS=%~3
    set SRCHDRS=%~4
    set INTFILES=%~5
    if %DEBUG% == 1 (
        echo MODULE   [%MODULE%]
        echo MKFILE   [%MKFILE%]
        echo INCHDRS  [%INCHDRS%]
        echo SRCHDRS  [%SRCHDRS%]
        echo INTFILES [%INTFILES%]
    )

    set TARGET_SRC=%CS_SRC%/CS%MODULE%.cs
    set TARGET_ALL=%TARGET_SRC%
    rem ------------------------------------------------------
    rem set TARGET_IMP=%CS_IMP%/CS%MODULE%.cs
    rem set TARGET_EXP=%CS_EXP%/CS%MODULE%.cpp
    rem set TARGET_ALL=%TARGET_SRC% %TARGET_IMP% %TARGET_EXP%
    rem ------------------------------------------------------

    set DEPENDENCIES=%INCHDRS% %SRCHDRS% %INTFILES%
    if %DEBUG% == 1 (
        echo TARGET       [%TARGET%]
        echo DEPENDENCIES [%DEPENDENCIES%]
    )

    echo #  Do not edit. RunSwig_CSharp.bat will update this file.   > %MKFILE%
    echo #  File: %MKFILE%	>> %MKFILE%
    echo.			>> %MKFILE%
    echo INCHDRS=\>> %MKFILE%
    for %%f in (%INCHDRS%) do (
        echo %%f \>> %MKFILE%
    )
    echo.			>> %MKFILE%
    echo SRCHDRS=\>> %MKFILE%
    for %%f in (%SRCHDRS%) do (
        echo %%f \>> %MKFILE%
    )
    echo.			>> %MKFILE%
    echo INTFILES=\>> %MKFILE%
    for %%f in (%INTFILES%) do (
        echo %%f \>> %MKFILE%
    )
    echo.			>> %MKFILE%
    echo FIXED_WRAPPERS=\>> %MKFILE%
    echo ./SprCSharp/wrapper.cs		\>> %MKFILE%
    echo ./SprImport/wrapper.cs		\>> %MKFILE%
    echo ./SprExport/wrapper.cpp	\>> %MKFILE%
    echo.			>> %MKFILE%
    echo WRAPPERS=\>> %MKFILE%
    echo ./SprCSharp/module.wrapper.cs	\>> %MKFILE%
    echo ./SprImport/module.wrapper.cs	\>> %MKFILE%
    echo ./SprExport/module.wrapper.cpp	\>> %MKFILE%
    echo.			>> %MKFILE%

    echo all:	%TARGET_ALL%    >> %MKFILE%
    echo.			>> %MKFILE%
    echo %TARGET_SRC%:	$(INCHDRS) $(SRCHDRS) $(INTFILES) $(FIXED_WRAPPERS)>> %MKFILE%
    echo.	call %SWIG% %MODULE%	>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(INCHDRS):			>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(SRCHDRS):			>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(INTFILES):			>> %MKFILE%
    echo.				>> %MKFILE%
    echo $(FIXED_WRAPPERS):		>> %MKFILE%
    echo.				>> %MKFILE%
    endlocal
exit /b

:: -----------------------------------------------------------------------------------
::  引数2 で与えられた名前が 引数1 で与えられたリスト中にあるか調べる
::  結果は $result に返す（yes または no）
:: -----------------------------------------------------------------------------------
:one_of
    set $result=no
    for %%f in (%~1) do (if "%2" equ "%%f" (set $result=yes&& exit /b))
exit /b

:: -----------------------------------------------------------------------------------
::  引数1 で与えられた変数に、引数2 で指定された prefix を追加する
::  結果は $string に返す
:: -----------------------------------------------------------------------------------
:add_prefix
    set $string=
    for %%f in (%~1) do (set $string=!$string! %2\%%f)
    set $string=%$string:~1%
exit /b

:: -----------------------------------------------------------------------------------
::  共通に使用する名前を exportする
:: -----------------------------------------------------------------------------------
:export
    call :get_abs_path .
    set BASEDIR=%$ret%
    endlocal && (
	set MODULE_WRAPPER_SRC=%MODULE_WRAPPER_SRC%
	set MODULE_WRAPPER_IMP=%MODULE_WRAPPER_IMP%
	set MODULE_WRAPPER_EXP=%MODULE_WRAPPER_EXP%
	set WRAPPERS_BUILT=%WRAPPERS_BUILT%
    )
exit /b
:get_abs_path
    set $ret=%~dp1
exit /b

:: -----------------------------------------------------------------------------------
::  デバッグ用
:: -----------------------------------------------------------------------------------
:show_abs_path
    echo %1:  [%~f2]
exit /b

::end RunSwig_CSharp.bat
