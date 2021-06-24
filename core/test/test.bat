@echo off
setlocal enabledelayedexpansion

set CWD=%CD%
call :leafname ..\..
set ROOTDIR=%$result%

set CONF=Release
set PLAT=x64
set TOOL=15.0

set TESTREPOSITORY=%ROOTDIR%
set RESULTREPOSITORY=DailyBuildResult/Result

set ARGS=--do-not-clone -c %CONF% -p %PLAT% -t %TOOL%
set CMND=DailyBuild.bat %ARGS% %TESTREPOSITORY% %RESULTREPOSITORY%

echo %CMND%
%CMND%

endlocal
exit /b

:leafname
	rem set $result=%~f1
	set $result=%~n1
	exit /b
