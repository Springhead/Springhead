@echo off

set CWD=%CD%
set Sflag=

if "%1" equ "-h" (
	call :usage %0
	exit /b
)
if "%1" equ "-S" (
	set Sflag=-S
	shift
)
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
rem set TESTREPOSITORY=SpringheadTest
cd %CWD%
echo test directory: %TESTREPOSITORY%

set DAILYBUILD_UPDATE_SPRINGHEAD=skip
set DAILYBUILD_CLEANUP_WORKSPACE=skip

echo DailyBuild.bat -U %Sflag% %TESTREPOSITORY% DailyBuildResult/Result
DailyBuild.bat -U %Sflag% %TESTREPOSITORY% DailyBuildResult/Result

exit /b

:get_sprtop
	set TESTREPOSITORY=%~n1
	exit /b

:usage
	echo Usage: %~n1 -h
	echo        %~n1 [-S] [test repository]	(-S: force clean setup)
	exit /b

