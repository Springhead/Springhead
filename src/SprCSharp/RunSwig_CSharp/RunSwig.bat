@echo off
:: ===================================================================================
::  File:
::	RunSwig.bat
::
::  Description:
::	Springhead のライブラリ(DLL) を C# から利用するためのコードを生成する。
::
::  Version:
::	Ver 1.0	 2015/01/26 F.Kanehori	初版
:: ===================================================================================
setlocal enabledelayedexpansion
set CWD=%cd%
set DUMPTREE=0
if "%1" equ "dumptree" set DUMPTREE=1

:: ----------
::  各種定義
:: ----------
:: ディレクトリの定義
::
call :set_abspath SRCDIR ..\..\..\src
set SPRCS=SprCSharp
set CS_SRC=SprCSharp
set CS_IMP=SprImport
set CS_EXP=SprExport
echo using src directory: %SRCDIR%

:: 処理対象となるディレクトリ
::
set PROJECTS=Base Collision Creature FileIO Foundation Framework Graphics HumanInterface Physics

:: 使用するパス
::
call :set_abspath SWIGPATH ..\..\..\bin\swig
set PATH=%SWIGPATH%;%PATH% 

:: 使用するプログラム名
::
set SWIG=swig
set ARGS=-sprcs -c++ -I%SWIGPATH%\Lib -w305,312,319,325,401,402

:: ----------
::  処理開始
:: ----------
rem echo start at %CWD%
for %%p in (%PROJECTS%) do (
    echo.
    echo *** %%p ***
    :: 各ディレクトリへ移動して make を実行する
    cd %SRCDIR%\%%p
    if exist %%p.i (
	if %DUMPTREE% == 1 (
	    cmd /c %SWIG% -debug-module 1 %%p.i > %%p.i.tree
	) else (
	    cmd /c %SWIG% %ARGS% -DSWIG_%%p %%p.i
	    copy .\%%pCs.cs  %SRCDIR%\%SPRCS%\%CS_SRC% > NUL
	    copy .\%%pCs.csp %SRCDIR%\%SPRCS%\%CS_IMP% > NUL
	    copy .\%%pCs.cpp %SRCDIR%\%SPRCS%\%CS_EXP% > NUL
	    echo %CS_SRC%\%%pCs.cs  created
	    echo %CS_IMP%\%%pCs.csp created
	    echo %CS_EXP%\%%pCs.cpp created
	)
    ) else (
	echo "%%p.i" not found
    )
    cd %CWD%
)

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:set_abspath
    set %1=%~f2
exit /b
