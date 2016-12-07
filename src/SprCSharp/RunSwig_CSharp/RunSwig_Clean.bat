@echo off
:: ***********************************************************************************
::  File:
::      RunSwig_Clean.bat
::
::  Description:
::      RunSwig_CSharp で作成されたファイルを削除する.
::      プロジェクト RunSwig_CSharp の [ソリューションのクリーン] から呼ばれる.
::
::    　実行するプロジェクトは ..\..\src\RunSwig\do_swigall.projs に定義されている
::      ものを使用する. ただしプロジェクト Base は定義の有無に関わりなく実行する.
::
:: ***********************************************************************************
::  Version:
::	Ver 2.0	 2016/12/05	F.Kanehori  ターゲット指定実装
::	Ver 1.0	 2015/03/18	F.Kanehori  初版
:: ***********************************************************************************
setlocal enabledelayedexpansion
set PROG=%~n0
set CWD=%cd%
set DEBUG=1

set TOPDIR=..\..\..
set SRCDIR=%TOPDIR%\src
set ETCDIR=%SRCDIR%\RunSwig

set CSBASE=..
set CS_SRC=%CSBASE%\SprCSharp
set CS_IMP=%CSBASE%\SprImport
set CS_EXP=%CSBASE%\SprExport

if %DEBUG% == 1 (
    echo %PROG%
    echo CWD: %CWD%
    call :show_abs_path TOPDIR %TOPDIR%
    call :show_abs_path SRCDIR %SRCDIR%
    call :show_abs_path ETCDIR %ETCDIR%
    call :show_abs_path CSBASE %CSBASE%
    call :show_abs_path CS_SRC %CS_SRC%
    call :show_abs_path CS_IMP %CS_IMP%
    call :show_abs_path CS_EXP %CS_EXP%
    echo. 
)

:: --------------------
::  使用するファイル名
:: --------------------
set PROJFILE=do_swigall.projs
set TARGETFILE=..\TargetManager\target.last

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
    del %CS_SRC%\CS%%p*.cs  > NUL 2>&1
    del %CS_IMP%\CS%%p*.cs  > NUL 2>&1
    del %CS_EXP%\CS%%p*.cpp > NUL 2>&1
    del %CS_SRC%\module_wrapper.cs  > NUL 2>&1
    del %CS_IMP%\module_wrapper.cs  > NUL 2>&1
    del %CS_EXP%\module_wrapper.cpp > NUL 2>&1
)
echo. > %TARGETFILE%

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:: -----------------------------------------------------------------------------------
::  デバッグ用
:: -----------------------------------------------------------------------------------
:show_abs_path
    echo %1:  [%~f2]
exit /b

::end RunSwig_Clean.bat
