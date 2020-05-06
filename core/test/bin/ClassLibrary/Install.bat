@echo off
setlocal enabledelayedexpansion
rem ====================================================================
rem  Install class libray files to Springhead2 bin-directory.
rem ====================================================================

set FILES=Error.py FileOp.py Fio.py KvFile.py Proc.py TextFio.py Util.py
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
