@echo off
setlocal enabledelayedexpansion
rem ====================================================================
rem  Install helper procedures to Springhead2 bin-directory.
rem ====================================================================

set INCFILES=dailybuild_SEH_Handler.h
set PRGFILES=GenKbEvent\Release\GenKbEvent.exe CatArgs.py
set INCINSDIR=..\..\include
set PRGINSDIR=..\..\src\RunSwig\pythonlib

call :to_abspath %INCINSDIR%
echo target directory: %ABSPATH%

for %%f in (%INCFILES%) do (
	echo   copying: %%f
	copy /V /Y %%f %ABSPATH% > NUL
)
echo.

call :to_abspath %PRGINSDIR%
echo target directory: %ABSPATH%

for %%f in (%PRGFILES%) do (
	call :get_leaf %%f
	echo   copying: %%f
	copy /V /Y %%f %ABSPATH%\!LEAFNAME! > NUL
)

endlocal
exit /b

rem --------------------------------------------------------------------
rem  Convert relative path to absolute path.
rem
:to_abspath
	set ABSPATH=%~f1
exit /b

rem --------------------------------------------------------------------
rem  Get leaf name.
rem
:get_leaf
	set LEAFNAME=%~n1%~x1
exit /b
