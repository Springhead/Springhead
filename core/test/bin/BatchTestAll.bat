@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	BatchTestAll [-E]
::	  -E:	シリアライズデータファイルがあれば、それを削除してから
::		テストを実行する。
::
::  DESCRIPTION
::	src/tests 及び src/Samples にあるすべてのソリューションについて、
::	platform と configuration のすべての組合せに対してテストを実施する。
::
::	注意：テスト実行中は、テストを起動したウィンドウからフォーカスを移動
::	　　　しないこと。さもないと、プログラムに入力データが渡らないために
::	　　　実行結果の判定に間違いが生じる恐れがる。
::
:: ----------------------------------------------------------------------------
::  VERSION
::	Ver 1.0  2017/09/21 F.Kanehori	初版
:: ============================================================================
set PROG=%~n0
set CWD=%CD%

:: -------------------------------------------------------------
::  Constants
:: -------------------------------------------------------------
set SCRIPT=BatchTestControl.py
set REPORT=MakeReport.py
set LOGDIR=log
set LOGFILE=BatchTestAll.log
set CSVFILE=BatchTestAll.csv

:: -------------------------------------------------------------
::  Test parameters
:: -------------------------------------------------------------
set TOOLSET=-t 14.0
set LOG=-l %LOGDIR%/%LOGFILE%
set DIRS=-d src/tests -d src/Samples
set OPTS=
:next_arg
if /i "%1" equ "-h" (call :usage & exit /b)
if    "%1" equ "-E" (set OPTS=!OPTS! -E & shift & goto :next_arg)

set PARAMS=%TOOLSET% %LOG% %OPTS% %DIRS%

:: -------------------------------------------------------------
::  Test Go
:: -------------------------------------------------------------
python %SCRIPT% %PARAMS%
python %REPORT% -o %LOGDIR%/%CSVFILE% %LOGDIR%/%LOGFILE%

endlocal
exit /b

:: -------------------------------------------------------------
::  Print usage
:: -------------------------------------------------------------
:usage
	echo Usage: %prog% [options]
	echo options:
	echo.    -E:     Erase serialized data file before test starts.
	exit /b

:: end: BatchTestAll.bat
