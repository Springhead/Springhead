@echo off
:: ***********************************************************************************
::  FILE:
::      RunSwig_Clean.bat
::
::  DESCRIPTION:
::      RunSwig_CSharp で作成されたファイルを削除する.
::      プロジェクト RunSwig_CSharp の [ソリューションのクリーン] から呼ばれる.
::
:: ***********************************************************************************
::  Version:
::	Ver 1.0	 2015/03/18 F.Kanehori	初版
::	Ver 2.0	 2016/12/05 F.Kanehori	リンク構成指定実装
::	Ver 2.1	 2017/01/16 F.Kanehori	NameManger 導入
:: ***********************************************************************************
setlocal enabledelayedexpansion
set PROG=%~n0
set CWD=%cd%
set DEBUG=1

:: ------------------------
::  共通環境変数を読み込む
:: ------------------------
call ..\NameManager\NameManager.bat
if %DEBUG% == 1 (
    echo %PROG%
    echo CWD: %CWD%
    call :show_abspath TOPDIR %TOPDIR%
    call :show_abspath SRCDIR %SRCDIR%
    call :show_abspath ETCDIR %ETCDIR%
    call :show_abspath CS_SRC %CS_SRC%
    call :show_abspath CS_IMP %CS_IMP%
    call :show_abspath CS_EXP %CS_EXP%
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
    del %CS_SRC%\module.wrapper.cs  > NUL 2>&1
    del %CS_IMP%\module.wrapper.cs  > NUL 2>&1
    del %CS_EXP%\module.wrapper.cpp > NUL 2>&1
    del %CS_SRC%\tmp\*.cs  > NUL 2>&1
    del %CS_IMP%\tmp\*.cs  > NUL 2>&1
    del %CS_EXP%\tmp\*.cpp > NUL 2>&1
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
:show_abspath
    echo %1:  [%~f2]
exit /b

::end RunSwig_Clean.bat
