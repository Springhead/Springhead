@echo off
:: ***********************************************************************************
::  File:
::	RunSwig.bat
::
::  SYNOPSIS:
::	RunSwig module
::
::  Description:
::	Springhead のライブラリ(DLL) を C# から利用するためのコードを生成する。
::
::  ARGUMENTS:
::	module		モジュール名
::
:: ***********************************************************************************
::  Version:
::	Ver 2.0	 2016/02/08	F.Kanehori	wrapper file をまとめた
::	Ver 1.0	 2015/01/26	F.Kanehori	初版
:: ***********************************************************************************
setlocal enabledelayedexpansion
set DEBUG=0
if %DEBUG% == 1 (
    set CS_INFO=SprInfo
    if not exist !CS_INFO! mkdir !CS_INFO!
)

set MODULE=%1
if "%MODULE%" equ "" (
    echo Error: Module name MUST be specified.
    exit /b
)

set DUMPTREE=0
if "%1" equ "dumptree" set DUMPTREE=1

:: ----------
::  各種定義
:: ----------
:: ディレクトリの定義
::
call :set_abspath SRCDIR ..\..\src
set CS_SRC=SprCSharp
set CS_IMP=SprImport
set CS_EXP=SprExport

echo. 
echo *** %MODULE% ***
echo using src directory: %SRCDIR%

:: 使用するパス
::
set SWIGPATH=..\..\bin\swig
set PATH=%SWIGPATH%;%PATH% 

:: 使用するプログラム名
::
set SWIG=swig_sprcs
set ARGS=-sprcs -DSWIG_CS_SPR -c++ -I%SWIGPATH%\Lib -w305,312,319,325,401,402

:: 使用するファイル名
::	RunSwig_CSharp.bat から読み出す
::
call RunSwig_CSharp\RunSwig_CSharp :export

:: ----------
::  処理開始
:: ----------
if exist %MODULE%.i (
    if %DUMPTREE% == 1 (
        cmd /c %SWIG% -debug-module 4 !MODULE!.i > CS%MODULE%.tree
    ) else (
	cmd /c %SWIG% %ARGS% -DSWIG_%MODULE% !MODULE!.i
	move /Y !MODULE!Cs.cs  %CS_SRC%\CS%MODULE%.cs  > NUL 2>&1
	move /Y !MODULE!CsP.cs %CS_IMP%\CS%MODULE%.cs  > NUL 2>&1
	move /Y !MODULE!Cs.cpp %CS_EXP%\CS%MODULE%.cpp > NUL 2>&1
	if not exist %CS_SRC%\tmp mkdir %CS_SRC%\tmp
	if not exist %CS_IMP%\tmp mkdir %CS_IMP%\tmp
	if not exist %CS_EXP%\tmp mkdir %CS_EXP%\tmp
	move /Y !MODULE!CsWrap.cs  %CS_SRC%\tmp\CS%MODULE%.%MODULE_WRAPPER_SRC% > NUL 2>&1
	move /Y !MODULE!CsPWrap.cs %CS_IMP%\tmp\CS%MODULE%.%MODULE_WRAPPER_IMP% > NUL 2>&1
	move /Y !MODULE!CsWrap.cpp %CS_EXP%\tmp\CS%MODULE%.%MODULE_WRAPPER_EXP% > NUL 2>&1
	echo %CS_SRC%\CS%MODULE%.cs  created
	echo %CS_IMP%\CS%MODULE%.cs  created
	echo %CS_EXP%\CS%MODULE%.cpp created
	echo %CS_SRC%\tmp\CS%MODULE%.%MODULE_WRAPPER_SRC% created
	echo %CS_IMP%\tmp\CS%MODULE%.%MODULE_WRAPPER_IMP% created
	echo %CS_EXP%\tmp\CS%MODULE%.%MODULE_WRAPPER_EXP% created
	if %DEBUG% == 1 (
	    move /Y %MODULE%CS.info %CS_INFO%\CS%MODULE%.info >NUL 2>&1
	) else (
	    del %MODULE%CS.info 2>NUL
	)
	type NUL > %WRAPPERS_BUILT%
    )
) else (
    echo "%MODULE%.i" not found
)
echo. 

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:set_abspath
    set %1=%~f2
exit /b
