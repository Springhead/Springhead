@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	python_adapter.bat script [-SprTop dir] [args..]	（Windows 専用）
::
::  DESCRIPTION
::	Python script を実行するための Windows 用アダプタ。
::	"Springhead/buildtool" が利用できるときは、それを使って実行する。
::	さもなければ、デフォルトで利用できる python を使って実行する。
::	デフォルトで python が利用できない場合、または python のバージョンが
::	3 より古い場合は、メッセージを表示して処理を中止する。
::	
::	このファイルは Windows 専用である。unix の場合はデフォルトで Python が
::	使用できるようにしておかなければならない。
::
::  VERSION
::	Ver 1.0  2017/07/24 F.Kanehori	初版
::	Ver 1.1  2017/08/02 F.Kanehori	引数 -SprTop 追加.
::	Ver 2.0  2017/09/07 F.Kanehori	パスの検索方法を変更.
::	Ver 2.01 2017/09/11 F.Kanehori	同上（コードの整理）.
::	Ver 3.0  2017/11/06 F.Kanehori	directory名変更 (buildtools -> buildtool)
:: ============================================================================
set verbose=0

::----------------------------------------------
::  buildtool の相対パス
::
set TOOLPATH=..\..\..\buildtool\win32
if "%1" equ "-SprTop" (
	set TOOLPATH=%2\buildtool\win32
	shift && shift
)
:: 引数の調整
set ARGS=
:next_arg
if "%1" == "" goto :end_arg
	rem echo (((%1)))
	set ARGS=!ARGS! %1
	shift
	goto :next_arg
:end_arg
if "%ARGS%" neq "" set ARGS=!ARGS:~1!

::----------------------------------------------
::  Python を実行できるようにする
::
if exist "%TOOLPATH%\python.exe" (
	PATH=!TOOLPATH!;!PATH!
) else (
	where python >NUL 2>& 1
	if !ERRORLEVEL! neq 0 (
		echo Python not found.
		exit /b
	)
	for /f "tokens=*" %%a in ('python -V') do set OUT=%%a
	set VER=!OUT:Python =!
	set MAJOR=!VER:~0,1!
	set MINOR=!VER:~2,1!
	if !MAJOR! neq 3 (
		echo !OUT! found.
		echo Use Python 3 or later version.
		endlocal
		exit /b
	)
)
if %verbose% geq 0 (
	rem where python
	rem python -V
)

::----------------------------------------------
::  CMake のための特別処理
::	バッチファイルの引数に "a=b" と書いても1つの引数とは見做されない。
::	そのため "a@b" という引数があったらそれを "a=b" と書き直すこととする。
::
set ARGS=%ARGS:@=^=%

::----------------------------------------------
::  Python を実行する
::
if %verbose% geq 2 (
	echo cwd: %CD%
	echo python %ARGS%
)
if %verbose% geq 1 (
	echo.
)
rem where python
rem python -V
rem echo python %ARGS%
python %ARGS%
rem echo %ERRORLEVEL%

endlocal
exit /b
