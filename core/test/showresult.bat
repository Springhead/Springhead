@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	showresult.bat [options]
::
::  OPTIONS
::	-l dir,  --log dir		Log directory name [log].
::	-o file, --outfile file		Output file name [stdout].
::	-r file, --resultfile file	Test result file name [result.log].
::	-t dir, --topdir dir		Top directory name of source tree.
::
::  DESCRIPTION
::	Convert format of "result.log" to a little readble.
::	Adapter of ShowResult.py.
::
:: ---------------------------------------------------------------------
::  VERSION
::     Ver 1.0	 2021/07/05 F.Kanehori	Start version control.
::     Ver 1.1	 2021/09/27 F.Kanehori	Introduce options.
:: ============================================================================
set ScriptDir=%~dp0

:: Python
::
set TOOLPATH=..\..\buildtool
if exist %TOOLPATH%\win32\python.exe (
	PATH=%TOOLPATH%;%TOOLPATH%\win32;!PATH!
)

set topdir=.
set logdir=log
set outfile=stdout
set resultfile=result.log
set verbose=0
:loop
	if "%1" == "-h"	(
		python %ScriptDir%\ShowResult.py --help
		exit /b
	)
	if "%1" == "-l"	(set logdir=%2     & shift & shift & goto :loop)
	if "%1" == "-o"	(set outfile=%2    & shift & shift & goto :loop)
	if "%1" == "-r"	(set resultfile=%2 & shift & shift & goto :loop)
	if "%1" == "-t"	(set topdir=%2     & shift & shift & goto :loop)
	if "%1" == "-v"	(set verbose=%1    & shift         & goto :loop)

set args=%verbose% -t %topdir% -l %logdir% -r %resultfile% -o %outfile%
python %ScriptDir%\ShowResult.py %args%

endlocal
exit /b
