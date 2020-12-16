@echo off

set CWD=%CD%
if "%1" neq "" (
	if not exist ..\..\..\%1 (
		echo %1: no such directory
		exit /b 1
	)
	cd ..\..\..\%1
) else (
	cd ..\..
)
call :get_sprtop %CD%
cd %CWD%
echo test directory: %TESTREPOSITORY%

set DAILYBUILD_UPDATE_SPRINGHEAD=skip
set DAILYBUILD_CLEANUP_WORKSPACE=skip

echo DailyBuild.bat -U %TESTREPOSITORY% DailyBuildResult/Result
DailyBuild.bat -U %TESTREPOSITORY% DailyBuildResult/Result

exit /b

:get_sprtop
	set TESTREPOSITORY=%~n1
	exit /b
