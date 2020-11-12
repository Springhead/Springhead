@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	install [-n]
::	  -n	Show copy command but do not execute.
::
::  DESCRIPTION
::	Install newer files to destination directory.
::
:: ----------------------------------------------------------------------------
::  VERSION
::	Ver 1.00 2020/11/12 F.Kanehori	Revised version.
:: ============================================================================
set PROG=%~n0
set CWD=%CD%

:: -------------------------------------------------------------
::  Get command line option
:: -------------------------------------------------------------
set DRYRUN=0
if "%1" == "-n"	(set DRYRUN=1)

:: -------------------------------------------------------------
::  Directories
:: -------------------------------------------------------------
set SRCDIR=.
set INSDIR=..\..\..\src\RunSwig\pythonlib

:: -------------------------------------------------------------
::  Candidate filesecho %%f
:: -------------------------------------------------------------
set CANDIDATES=
for %%f in (*.py) do (
	if "%%f" neq "MakeDoc.py" (
		set CANDIDATES=!CANDIDATES! %%f
	)
)

:: -------------------------------------------------------------
::  Compare file times
:: -------------------------------------------------------------
for %%f in (%CANDIDATES%) do (
	call :get_mtime %SRCDIR%\%%f
	set SRC_TIME=!$mtime!
	if exist %INSDIR%\%%f (
		call :get_mtime %INSDIR%\%%f
		set INS_TIME=!$mtime!
		rem echo [!SRC_TIME!] %%f
		if "!SRC_TIME!" GTR "!INS_TIME!" (
			rem echo [!SRC_TIME!] [!INS_TIME!] %%f
			rem  Destination file is OLDER
			set FILES=!FILES! %%f
		)
	) else (
		rem Destination file does not exist
		set FILES=!FILES! %%f
	)
)

:: -------------------------------------------------------------
::  Copy files
:: -------------------------------------------------------------
set COUNT=0
for %%f in (%FILES%) do (
	set SRCFILE=%%f
	echo copy !SRCFILE! %INSDIR:\=/%/!SRCFILE!
	if %DRYRUN% neq 1 (
		copy /V /Y %%f %INSDIR%\%%f > nul 2>&1
		if !ERRORLEVEL! equ 0 (
			set /a COUNT=!COUNT! + 1
		)
	)
)
if %DRYRUN% neq 1 ( echo.  %COUNT% file^(s^) copied )

endlocal
exit /b 0

:: -------------------------------------------------------------
::  Get file mtime
:: -------------------------------------------------------------
:get_mtime
	set $mtime=%~t1
	rem echo %$mtime% : %1
exit /b

rem end: install.bat
