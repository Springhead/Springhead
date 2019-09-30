@echo off

set CWD=%CD%
cd ..\..
call :get_sprtop %CD%
cd %CWD%

echo DailyBuild.bat -U %SPRTOP% DailyBuildResult/Result
DailyBuild.bat -U %SPRTOP% DailyBuildResult/Result

exit /b

:get_sprtop
	set SPRTOP=%~n1
	exit /b
