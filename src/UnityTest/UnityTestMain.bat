@echo off
rem ======================================================================
rem  FILE: UnityTestMain.bat
rem
rem  SYNOPSIS:
rem	UnityTestMain [timeout_in_seconds]
rem
rem  DESCRIPTION:
rem	Assets/Scenes/ にあるシーンを全部実行してみる
rem	※ 一部実行できないものを除く
rem
rem  VERSION:
rem	Ver 1.0  2016/06/16 F.Kanehori	First release
rem ======================================================================
setlocal enabledelayedexpansion
set VERSION=1.0
set ARG0=%0
set SCRIPT=%ARG0:.bat=%

if "%1" equ "-V" (
	echo %SCRIPT%: Version %VERSION%
	exit /b
)
if "%1" equ "" (
	set TIMEOUT=15
) else (
	set TIMEOUT=%1
)
set UNITYTEST=python UnityTest.py
set INIFILE=UnityTest.ini
set SCENES=CharacterPronamaChan ConvexMeshCDTest FWApp IK Joint RigidBody windowTest

set OPTS=-T -i %INIFILE%
if "%1" neq "" (
	set OPTS=!OPTS! -t %1
)
echo %SCRIPT%: start
del bin\*.exe >NUL 2>&1
del log\*.log >NUL 2>&1
for %%s in (%SCENES%) do (
	%UNITYTEST% %OPTS% %%s 
	set RESULT=!ERRORLEVEL!
	if not !RESULT! equ 0 (
		cd bin
		set FILE=player.exe
		if exist !FILE! (
			echo   saving bin/!FILE! as bin/%%s.!FILE!
			rem del %%s.!FILE! >NUL 2>&1
			move /Y !FILE! %%s.!FILE! >NUL 2>&1
		)
		cd ..\log
		set FILE=build.log
		if exist !FILE! (
			echo   saving log/!FILE! as log/%%s.!FILE!
			rem del %%s.!FILE! >NUL 2>&1
			move /Y !FILE! %%s.!FILE! >NUL 2>&1
		)
		cd ..
	)
)
echo %SCRIPT%: end

endlocal
exit /b
