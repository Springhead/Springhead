@echo off

set CWD=%CD%
cd ..\..\..\SpringheadTest
call :get_sprtop %CD%
cd %CWD%

set DAILYBUILD_UPDATE_SPRINGHEAD=skip
set DAILYBUILD_CLEANUP_WORKSPACE=skip

echo DailyBuild.bat -U %TESTREPOSITORY% DailyBuildResult/Result
DailyBuild.bat -U %TESTREPOSITORY% DailyBuildResult/Result

exit /b

:get_sprtop
	set TESTREPOSITORY=%~n1
	exit /b
