@echo off
:: ----------------------------------------------------------------------------
::  SYNOPSIS
::	MakeWin_Wrapper.bat {-build | -rebuild | -clean}
::
::  DESCRIPTION
::	RunSwig/Makefile.win を Visual Studio から実行するためのラッパ。
::	Python が実行できる環境を整えてから各処理を呼び出す。
::
:: ----------------------------------------------------------------------------
::  Version:
::	Ver 1.0	  2020/12/10	F.Kanehori	初版
:: ----------------------------------------------------------------------------
setlocal enabledelayedexpansion
set CWD=%cd%
set DEBUG=0

echo [[[[ MakeWin_Wrapper: %* ]]]]
:: -------------------------------------------------------------
::  このスクリプトは "<SprTop>/core/src/RunSwig" に置く
::
cd /d %~dp0
set ScriptDir=%CD%
cd %CWD%

:: 作業ファイル名
set TMPF=MakeWin_Wrapper.tmp

:: -------------------------------------------------------------
::  引数の解析
::
if "%1" equ ""	( call :usage & exit /b 1)
if "%2" neq ""	( call :usage & exit /b 1)
if "%1" neq "-build" if "%1" neq "-rebuild" if "%1" neq "-clean" (
	call :usage & exit /b 1
)

:: -------------------------------------------------------------
::  Python を選択してパスに加える
::
call :abspath %ScriptDir%\..\..\..\buildtool\win32
set ToolPath=%$result%
if exist %ToolPath%\python.exe (
	rem buildtool がある場合
	call :set_path %ToolPath%
	echo %ToolPath%\python.exe > %TMPF%.1
) else (
	rem デフォルトのpyhtonを使う場合
	rem	call :set_old_python
	where python > %TMPF%.1 2>&1
	if !ERRORLEVEL! neq 0 (
		echo Python not found.
		echo We need python to build Springhead Library.
		call :remove %TMPF%.1
		exit /b 2
	)
	for /f "tokens=*" %%a in ('python -V') do set OUT=%%a
	if "!OUT!" equ "" (
		rem 結果が標準エラーに出力されている
		python -V > %TMPF%.2 2>&1
		for /f "tokens=1,2" %%a in (%TMPF%.2) do set OUT=%%a %%b
	)
	set VERSION=!OUT:Python =!
	set MAJOR=!VERSION:~0,1!
	if !MAJOR! neq 3 (
		echo !OUT! found.
		echo Use Python 3 or later version.
		call :remove %TMPF%.1
		call :remove %TMPF%.2
		exit /b 2
	)
)
set /p=using <NUL
python --version
call :get_cmake_version
if "%cmake_version%" neq "none" (echo using %cmake_version%)
if exist ..\setup.conf (echo using setup info)
call :remove %TMPF%.1
call :remove %TMPF%.2

:: -------------------------------------------------------------
::  セットアップファイルがあれば登録された情報を追加する
::
set SetupFile=%ScriptDir%\..\setup.conf
call :abspath %SetupFile%
echo   %$result%
if exist %SetupFile% (
	set ProgPaths=
	python MakeWin_Wrapper_helper.py %SetupFile% %TMPF%.3
	for /f "tokens=1,2,*" %%a in (%TMPF%.3) do (
		if "%%a" equ "PROG" (
			call :set_envvar %%b "%%c"
			set ProgPaths=!ProgPaths! "%%c"
		)
		if "%%a" equ "PATH" (
			call :set_envvar %%b "%%c"
		)
		rem 以下は移行期間終了後には削除する
		rem ---- ここから --------------------
		if "%%a" equ "DATA" (
			call :set_envvar %%b "%%c"
		)
		rem ---- ここまで --------------------
	)
	call :remove %TMPF%.3
	echo using:
	for %%a in (!ProgPaths!) do (
		set tmp=%%a
		echo   !tmp:^"=!
	)
)
if %DEBUG% neq 0 ( exit /b 0 )

:: -------------------------------------------------------------
::  コマンドを実行する
::
if "%1" equ "-build" (
	python make_manager.py -A -c && nmake -f Makefile.win
)
if "%1" equ "-rebuild" (
	python  make_manager.py -A -d -c && nmake -f Makefile.win
)
if "%1" equ "-clean" (
	python  make_manager.py -A -d
)

endlocal
exit /b 0


:: ----------------------------------------------------------------------------
:set_envvar
	rem  call :set_envvar name value
	endlocal && set _set_name_=%1 & set _set_value=%~2
	set %_set_name_%=%_set_value%
	setlocal enabledelayedexpansion
	exit /b

:set_path
	rem  call set_path path
	call :abspath %1
	endlocal && set _prog_path_=%$result%
	path=%_prog_path_%;%path%
	setlocal enabledelayedexpansion
	exit /b

:abspath
	set $result=%~f1
	exit /b

:get_cmake_version
	setlocal enabledelayedexpansion
	set version=
	cmake --version > %TMPF%.c 2> NUL
	for /f "tokens=*" %%a in (%TMPF%.c) do (
		set version=%%a
		goto :end_loop_c
	)
	:end_loop_c
	if "%version%" equ "" (set version=none)
	call :remove %TMPF%.c
	endlocal && set cmake_version=%version%
	exit /b

:remove
	if exist %1 del %1 >NUL 2>&1
	exit /b

:usage
	echo Usage: %~n0 { -build ^| -rebuild ^| -clean }
	exit /b

rem :set_old_python
rem 	endlocal
rem 	path=F:\OLD-Python\Python27;%path%
rem 	setlocal enabledelayedexpansion
rem 	exit /b

:end MakeWin_Wrapper.bat
