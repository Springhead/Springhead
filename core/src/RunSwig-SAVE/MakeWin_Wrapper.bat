@echo off
:: ----------------------------------------------------------------------------
::  SYNOPSIS
::	MakeWin_Wrapper.bat {-build | -rebuild | -clean}
::
::  DESCRIPTION
::	RunSwig/Makefile.win を Visual Studio から実行するためのラッパ。
::	Python が実行できる環境を整えてから各処理を呼び出す。
::
:: ----------------------------------------------------------------------------
::  Version:
::    Ver 1.0    2020/12/10 F.Kanehori	初版
::    Ver 2.0    2021/03/04 F.Kanehori	Python環境設定はdo_python.batで行なう.
:: ----------------------------------------------------------------------------
setlocal enabledelayedexpansion
set CWD=%cd%
set DEBUG=0

rem echo [[[[ MakeWin_Wrapper: %* ]]]]
:: -------------------------------------------------------------
::  このスクリプトは "<SprTop>/core/src/RunSwig" に置く
::
cd /d %~dp0
set ScriptDir=%CD%
set SrcDir=%ScriptDir%\..
cd %CWD%

:: 作業ファイル名
set TMPF=MakeWin_Wrapper.tmp

:: -------------------------------------------------------------
::  引数の解析
::
if "%1" equ ""	( call :usage & exit /b 1)
if "%2" neq ""	( call :usage & exit /b 1)
if "%1" neq "-build" if "%1" neq "-rebuild" if "%1" neq "-clean" (
	call :usage & exit /b 1
)

:: -------------------------------------------------------------
::  コマンドを実行する
::
set python=%SrcDir%\do_python.bat
if "%1" equ "-build" (
	%python% make_manager.py -A -c && nmake -f Makefile.win
)
if "%1" equ "-rebuild" (
	%python%  make_manager.py -A -d -c && nmake -f Makefile.win
)
if "%1" equ "-clean" (
	%python%  make_manager.py -A -d
)

endlocal
exit /b 0

:: ----------------------------------------------------------------------------
:usage
	echo Usage: %~n0 { -build ^| -rebuild ^| -clean }
	exit /b

:end MakeWin_Wrapper.bat
