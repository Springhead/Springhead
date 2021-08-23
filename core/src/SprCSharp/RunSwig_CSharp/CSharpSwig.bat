@echo off
setlocal enabledelayedexpansion
:: =============================================================================
::  SYNOPSIS:
::	CSharpSwig module [swigmacro]
::	    module	モジュール名
::	    swigmacro	swig に渡すマクロ（#ifdef swigmacro として使う）
::
::  DESCRIPTION:
::	Springhead のライブラリ(DLL) を C# から利用するためのコードを生成する。
::
:: -----------------------------------------------------------------------------
::  VERSION:
::     Ver 1.0	 2015/01/26 F.Kanehori  初版
::     Ver 2.0	 2016/02/08 F.Kanehori  wrapper file 統合
::     Ver 3.0	 2016/12/01 F.Kanehori  ターゲット指定実装
::     Ver 3.1	 2016/12/15 F.Kanehori	ラッパファイル作成方式変更
::     Ver 3.2	 2017/01/16 F.Kanehori	NameManger 導入
::     Ver 4.0	 2021/07/19 F.Kanehori	見直し.
:: =============================================================================
set PROG=%~n0
set DEBUG=0

:: ----------------------------------------------------------------------
::  このスクリプトは "<SprTop>/core/src/SprCSharp/RunSwig_CSharp" に置く
::
set CWD=%cd%
cd /d %~dp0\..\..\..\..
set SprTop=%CD%
cd ..
set SprBase=%CD%
cd %CWD%
set CspDir=%SprTop%\core\src\SprCSharp

:: ------------------------
::  デバッグ情報出力の設定
:: ------------------------
if %DEBUG% == 1 (
    set CS_INFO=SprInfo
    if not exist !CS_INFO! mkdir !CS_INFO!
)

:: ------------
::  引数の処理
:: ------------
set MODULE=%1
if "%MODULE%" equ "" (
    echo %PROG%: Error: Module name MUST be specified.
    exit /b
)
set MACRO=%2
if "%MACRO%" neq "" set MACRO=-D%MACRO%

set DUMPTREE=0
if "%1" equ "dumptree" set DUMPTREE=1

if %DEBUG% == 1 (
    echo %~nx0
    echo+  MODULE:  [%MODULE%]
    echo+  MACRO:   [%MACRO%]
)

:: ------------------------
::  共通環境変数を読み込む
:: ------------------------
set NAMEMANAGER=%CspDir%\NameManager\NameManager.bat
if not exist %NAMEMANAGER% (
	:: NameManager.bat が存在しないときは何もしない
	echo "NameManager.bat" not found.
	exit /b
)
call %NAMEMANAGER%
if %DEBUG% == 1 (
    call :show_abspath INCDIR %INCDIR%
    call :show_abspath SRCDIR %SRCDIR%
    call :show_abspath ETCDIR %ETCDIR%
    call :show_abspath CS_SRC %CS_SRC%
    call :show_abspath CS_IMP %CS_IMP%
    call :show_abspath CS_EXP %CS_EXP%
    call :show_abspath TARGETFILE %TARGETFILE%
    echo+  SWIG     %SWIG%
)

echo+ 
echo+  *** %MODULE% ***
echo+  using src directory: %SRCDIR%
echo+  SWIG: %SWIG%

:: ----------
::  各種定義
:: ----------
set PATH=%SWIGDIR%;%PATH% 
set ARGS=-sprcs -DSWIG_CS_SPR -c++ -I%SWIGPATH%\Lib -w305,312,319,325,401,402

:: ----------
::  処理開始
:: ----------
if exist %MODULE%.i (
    if %DUMPTREE% == 1 (
        cmd /c %SWIG% -debug-module 4 %MODULE%.i > CS%MODULE%.tree
    ) else (
	cmd /c %SWIG% %ARGS% -DSWIG_%MODULE% %MACRO% %MODULE%.i
	move /Y %MODULE%Cs.cs  %CS_SRC%\CS%MODULE%.cs  > NUL 2>&1
	move /Y %MODULE%CsP.cs %CS_IMP%\CS%MODULE%.cs  > NUL 2>&1
	move /Y %MODULE%Cs.cpp %CS_EXP%\CS%MODULE%.cpp > NUL 2>&1
	echo %SUBDIR_SRC%\CS%MODULE%.cs  created
	echo %SUBDIR_IMP%\CS%MODULE%.cs  created
	echo %SUBDIR_EXP%\CS%MODULE%.cpp created
	if %DEBUG% == 1 (
	    move /Y %MODULE%CS.info %CS_INFO%\CS%MODULE%.info >NUL 2>&1
	) else (
	    del %MODULE%CS.info 2>NUL
	)
	type NUL > %WRAPPERSBUILTFILE%
    )
) else (
    echo "%MODULE%.i" not found
)
echo+ 

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

::-------------
::  デバッグ用
::-------------
:show_abspath
    echo+  %1:  %~f2
exit /b

::end CSharpSWig.bat
