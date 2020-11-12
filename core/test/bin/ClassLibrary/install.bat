@echo off
setlocal enabledelayedexpansion
rem ====================================================================
rem  Install class libray files to Springhead bin directory.
rem ====================================================================

set n=1
for /f "usebackq tokens=1 delims==" %%o in (`where install.bat`) do (
	set global=%%o
)
:x
echo [%global%]
for /f "usebackq tokens=1 delims==" %%c in (`dir /b install.bat`) do set local=%%c
call :to_abspath %local%
set local=%ABSPATH%
echo [%local%]
exit /b

set FILES=Error.py FileOp.py Fio.py KvFile.py Proc.py TextFio.py Util.py
set FILES=%FILES% VisualStudio.py SetupFile.py
set INSDIR=..\..\..\src\RunSwig\pythonlib

call :to_abspath %INSDIR%
echo target directory: %ABSPATH%

for %%f in (%FILES%) do (
	echo   copying: %%f
	copy /V /Y %%f %ABSPATH% > NUL
)

endlocal
exit /b

rem --------------------------------------------------------------------
rem  Convert relative path to absolute path.
rem
:to_abspath
	set ABSPATH=%~f1
exit /b
