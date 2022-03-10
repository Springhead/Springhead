@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	BatchTestAll
::
::  DESCRIPTION
::
::	注意：Debug でテストするためには、Python35_d.lib/dll が必要
::	      x64 は標準装備だが、Win32 はテスト前にコピーする必要あり！！
::
::  ARGUMENTS
::
::  VERSION
::	Ver 1.0  2017/08/10 F.Kanehori	初版
:: ============================================================================
set PROG=%~n0
set CWD=%CD%

:: -------------------------------------------------------------
::  Scripts to use
:: -------------------------------------------------------------
set PYTHON=python
set SCRIPT=BatchTestControl.py

:: -------------------------------------------------------------
::  Test parameters
:: -------------------------------------------------------------
set TOOLSET=-t 14.0
set PLATFORM=-P x64
set CONFIG=-C Release
set DIRS=-d tests -d Samples
set LOG=-l log/dailybuild.log

set PARAMS=%PLATFORM% %CONFIG% -u %LOG%

:: -------------------------------------------------------------
::  Test Go
:: -------------------------------------------------------------
%PYTHON% %SCRIPT% %PARAMS%





endlocal
exit /b

:: end: BatchTestAll.bat
