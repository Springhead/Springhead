@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	SetMakePath
::
::  DESCRIPTION
::	nmake を使用するための path を求め, 環境変数 MAKEPATH に設定する.
::
::  ARGUMENTS
::	なし
::
::  VERSION
::	Ver 1.0  2017/01/16 F.Kanehori	初版
:: ============================================================================
set PROG=%~n0

set X32=c:\Program Files
set X64=c:\Program Files (x86)
set ARCH=
if exist "%X32%" set ARCH=%X32%
if exist "%X64%" set ARCH=%X64%

for %%v in (14.0 12.0 10.0) do (
	set MAKEPATH="%ARCH%\Microsoft Visual Studio %%v\VC\bin"
	if exist !MAKEPATH! goto :done
)
set MAKEPATH=

:done
endlocal && (set MAKEPATH=%MAKEPATH%)
exit /b

::end SetMakePath.bat
