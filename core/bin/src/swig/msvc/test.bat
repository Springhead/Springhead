@echo off
setlocal enabledelayedexpansion

set PROG=%0
set TOOLSET=14.0
set CONFIG=Debug
set PLATFORM=Win32
:next_arg
	if /i "%1" equ "-h" ( call :usage %0 && exit /b )
	if /i "%1" equ "/h" ( call :usage %0 && exit /b )
	if /i "%1" equ "-t" ( set TOOLSET=%2&& shift && shift )
	if /i "%1" equ "/t" ( set TOOLSET=%2&& shift && shift )
	if /i "%1" equ "-c" ( set CONFIG=%2&& shift && shift )
	if /i "%1" equ "/c" ( set CONFIG=%2&& shift && shift )
	if /i "%1" equ "-p" ( set PLATFORM=%2&& shift && shift )
	if /i "%1" equ "/p" ( set PLATFORM=%2&& shift && shift )
	if "%1" neq "" (
		if "%2" neq "" ( goto :next_arg )
	)

if "%1" neq ""		( call :usage %PROG% && exit /b )
if "%TOOLSET%"  equ ""	( call :usage %PROG% && exit /b )
if "%PLATFORM%" equ ""	( call :usage %PROG% && exit /b )
if "%CONFIG%"   equ ""	( call :usage %PROG% && exit /b )

set SWIG=swig%TOOLSET%

set TESTDIR=%cd%
set SWIGEXE=%TESTDIR%\%PLATFORM%\%CONFIG%%TOOLSET%\%SWIG%.exe

set SPRTOP=..\..\..\..\..
call :to_abspath SWIGDIR %SPRTOP%\core\bin\swig
call :to_abspath CSDIR %SPRTOP%\core\src\SprCSharp

set PATH=%SWIGDIR%;%PATH%
set PROJS=Base Collision Creature FileIO Foundation Framework Graphics HumanInterface Physics
set DEFS=-DSWIG_CS -DSWIG_CS_SPR

del All.err  > NUL 2>&1
del All.cs   > NUL 2>&1
del All.p.cs > NUL 2>&1
del All.cpp  > NUL 2>&1
del AllWrap.cs   > NUL 2>&1
del AllWrap.p.cs > NUL 2>&1
del AllWrap.cpp  > NUL 2>&1
if "%CONFIG%" equ "Debug" (
	del All.info > NUL 2>&1
)

cd %CSDIR%
echo cwd: %cd%
call :init_signature
for %%d in (%PROJS%) do (
	echo.
	echo [ %%d ]
	%SWIGEXE% -v -sprcs %DEFS% -c++ -I%SWIGDIR%\Lib %%d.i 2>%%dCS.err
	call :add_files %%d %cd% %TESTDIR%
)
cd %TESTDIR%

echo.
echo ----
echo TOOLSET:  %TOOLSET%
echo PLATFORM: %PLATFORM%
echo CONFIG:   %CONFIG%
echo SWIGEXE:  %SWIGEXE%
echo CWD:      %cd%

endlocal
exit /b

:add_files
	set PROJECT=%1
	set BUILDDIR=%2
	set TESTDIR=%3
	call :add_files_sub %PROJECT% %BUILDDIR%\%PROJECT%Cs.err  %TESTDIR%\ALL.err
	call :add_files_sub %PROJECT% %BUILDDIR%\%PROJECT%Cs.cs   %TESTDIR%\ALL.cs
	call :add_files_sub %PROJECT% %BUILDDIR%\%PROJECT%CsP.cs  %TESTDIR%\ALL.p.cs
	call :add_files_sub %PROJECT% %BUILDDIR%\%PROJECT%Cs.cpp  %TESTDIR%\ALL.cpp
	echo [[%PROJ%]] >> %TESTDIR%\ALLWrap.cs
	for %%f in (%BUILDDIR%\SprCSharp\tmp\*.cs) do (
		call :add_files_sub %PROJECT% %%f %TESTDIR%\ALLWrap.cs
	)
	echo [[%PROJ%]] >> %TESTDIR%\ALLWrap.p.cs
	for %%f in (%BUILDDIR%\SprImport\tmp\*.cs) do (
		call :add_files_sub %PROJECT% %%f %TESTDIR%\ALLWrap.p.cs
	)
	echo [[%PROJ%]] >> %TESTDIR%\ALLWrap.cpp
	for %%f in (%BUILDDIR%\SprExport\tmp\*.cpp) do (
		call :add_files_sub %PROJECT% %%f  %TESTDIR%\ALLWrap.cpp
	)
	if "%CONFIG%" equ "Debug" (
		call :add_files_sub %PROJECT% %BUILDDIR%\%PROJECT%Cs.info %TESTDIR%\ALL.info
	)
exit /b

:add_files_sub
	set PROJ=%1
	set WRKFILE=%2
	set ACCFILE=%3
	type %WRKFILE% >> %ACCFILE%
	echo. >> %ACCFILE%
	del %WRKFILE%
exit /b

:init_signature
	set SIGNATUREFILE=swig_sprcs.signature
	if exist %SIGNATUREFILE% (
		del /F %SIGNATUREFILE%
	)
	type NUL > %SIGNATUREFILE%
exit /b

:to_abspath
	set %1=%~f2
exit /b

:usage
	echo Usage: %1 [-t toolset] [-p platform] [-c config]
	echo    toolset:  toolset ID { ^<14.0^> }
	echo    platform: platform ID { ^<Win32^> ^| x64 }
	echo    config:   configuration { ^<Debug^> ^| Release }
exit /b
