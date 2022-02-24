@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	hook.bat test-repository
::	    tset-repository	テストレポジトリ名. 絶対パスで与えること.
::
::  DESCRIPTION
::	DailyBuild.bat から呼び出され, dailybuild 実行時に使用するファイルを
::	すげ替える仕組みを提供する. この仕組を利用して github にコミットする
::	ことなくデバッグ操作を仕込むことを可能とする.
::
::	このファイルがある場所を <hook>, テストレポジトリを <test> とするとき
::	"<hook>/core" 以下にあるファイルを "<test>/core" 以下の対応する場所に
::	コピーする.
::
::	このファイルは, demo マシンの次のディレクトリにコピーして使用する.
::	     "I:\DailyBuild\DailyBuildHook"
::	ただし, "I:\DailyBuild" から呼び出すこと.
::
:: ============================================================================
set PROG=%~n0
set CWD=%CD%
echo [%PROG%] was called from "%CWD%".
cd /d %~dp0

if "%1" == "" (
	echo Usage: %PROG% test-repository
	exit /b 1
)
set REPOSITORY=%1

if exist "ignore" (
	:: このファイルが存在したら何もせずに抜ける
	echo %PROG%: nothing applied ^("ignore" exists^).
	cd %CWD%
	exit /b
)

:: core 以下にあるファイルをコピーする
::
for /r core %%f in (*) do (
	set fname=%%f
	set fname=!fname:%CD%\=!
	set /p X=%PROG%: applying ".\!fname!" ... <NUL
	copy %%f %CWD%\%REPOSITORY%\!fname! >NUL 2>&1
	if !ERRORLEVEL! equ 0 (
		echo OK
	) else (
		echo NG
	)
)

cd %CWD%
endlocal
exit /b

:: end: hook.bat
