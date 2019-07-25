@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS	SetMakePath
::
::  ARGUMENTS
::	なし
::
::  DESCRIPTION
::	nmake を使用するための path を求め, 環境変数 MAKEPATH に設定する.
::
::  TODO
::	"src/RunSwig/do_swigall.py" にも同様のパス設定がある。
::	きちんとまとめる必要あり。
::
::  VERSION
::	Ver 1.1  2019/08/25 F.Kanehori	検索パス追加 (15.0) 【暫定】
::	Ver 1.0  2017/01/16 F.Kanehori	初版
:: ============================================================================
set PROG=%~n0

set X32=C:\Program Files
set X64=C:\Program Files (x86)
set ARCH=
if exist "%X32%" set ARCH=%X32%
if exist "%X64%" set ARCH=%X64%

for %%v in (14.0 12.0 10.0) do (
	set MAKEPATH="%ARCH%\Microsoft Visual Studio %%v\VC\bin"
	if exist !MAKEPATH! goto :done
)
set MSVC="%ARCH%\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC"
set CWD=%CD%
for %%v in (15.0) do (
	cd /d %MSVC%
	for /d %%d in (*) do (
		if exist "%X32%" set MAKEPATH=%MSVC%\%%d\bin\Hostx64\x64
		if exist "%X64%" set MAKEPATH=%MSVC%\%%d\bin\Hostx86\x86
		if exist !MAKEPATH!\nmake.exe goto :done
	)
)
set MAKEPATH=

:done
endlocal && (set MAKEPATH=%MAKEPATH%)
exit /b

::end SetMakePath.bat
