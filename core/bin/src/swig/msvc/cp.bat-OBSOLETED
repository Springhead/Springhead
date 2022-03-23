@echo off
setlocal enabledelayedexpansion

set PROG=%0
set TOOLSET=14.0
set PLATFORM=Win32
set CONFIG=Debug
:next_arg
	if /i "%1" equ "-h" ( call :usage %0 && exit /b )
	if /i "%1" equ "/h" ( call :usage %0 && exit /b )
	if /i "%1" equ "-t" ( set TOOLSET=%2&& shift && shift && goto :next_arg )
	if /i "%1" equ "/t" ( set TOOLSET=%2&& shift && shift && goto :next_arg )
	if /i "%1" equ "-c" ( set CONFIG=%2&& shift && shift && goto :next_arg )
	if /i "%1" equ "/c" ( set CONFIG=%2&& shift && shift && goto :next_arg )
	if /i "%1" equ "-p" ( set PLATFORM=%2&& shift && shift )
	if /i "%1" equ "/p" ( set PLATFORM=%2&& shift && shift )
	if "%1" neq "" (
		if "%2" neq "" ( goto :next_arg )
	)

if "%1" neq ""		( call :usage %PROG% && exit /b )
if "%TOOLSET%"  equ ""	( call :usage %PROG% && exit /b )
if "%PLATFORM%" equ ""	( call :usage %PROG% && exit /b )
if "%CONFIG%"   equ ""	( call :usage %PROG% && exit /b )

set SWIG=swig%TOOLSET%.exe

set TESTDIR=%cd%
set FILE=%PLATFORM%\%CONFIG%%TOOLSET%\%SWIG%

set SPRTOP=..\..\..\..\..
call :to_abspath DEST %SPRTOP%\core\bin\swig
rem set DEST=..\..\..\swig
echo copying %TESTDIR%\%FILE% to %DEST%\swig_sprcs.exe
copy /y %TESTDIR%\%FILE% %DEST%\swig_sprcs.exe

endlocal
exit /b

:to_abspath
	set %1=%~f2
exit /b

:usage
	echo %1 [-t toolset] [-p platform] [-c config]
	echo    toolset:  toolset ID { ^<14.0^> }
	echo    platform: platform ID { ^<Win32^> ^| x64 }
	echo    config:   configuration { ^<Debug^> ^| Release }
exit /b
