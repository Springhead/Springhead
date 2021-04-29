@echo off
setlocal enabledelayedexpansion

set FLAGS=
set Sflag=
:loop
if "%1" neq "" (
	if "%1" equ "-S" (
		set Sflag=%1
	) else (
		set FLAGS=!FLAGS! %1
	)
	shift & goto :loop
)
call :to_abspath ..\..\buildtool
set TOOLPATH=%ABSPATH%
if exist %TOOLPATH%\win32\python.exe (
	PATH=%TOOLPATH%;%TOOLPATH%\win32;!PATH!
)

python DailyBuild.py -u %FLAGS%
echo.
python DailyBuild.py -U %Sflag% %FLAGS%

endlocal
exit /b

:to_abspath
	set ABSPATH=%~f1
exit /b
