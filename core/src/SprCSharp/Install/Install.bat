@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	Install src-path dst-path [dry-run]
::
::  DESCRIPTION
::	src-path で指定したファイルを dst-path にコピーする.
::	ただし, dst-path を置くべきディレクトリが存在しなければ何もしない.
::
::  ARGUMENTS
::	src-oath	コピー元のファイルパス
::	dst-path	コピー先のディレクトリパスまたはファイルパス
::	control		経過措置用引数：
::			    "copy" 以外を指定するとコピーは行なわない.
::
::  VERSION
::	Ver 1.0  2017/08/09 F.Kanehori	初版
::	Ver 1.1  2017/11/08 F.Kanehori	コピー先ディレクトリがなければ作成する.
:: ============================================================================
set PROG=%~n0

::----------------------------------------------
:: 引数の処理
::----------------------------------------------
set SRC=%~1
set DST=%~2
set COPYCONTROL=%3
if "%SRC%" == ""    ( call :usage && exit /b )
if "%DST%" == ""    ( call :usage && exit /b )
if exist %SRC%\	    ( call :error '%1' is not a file && exit /b )
if not exist %SRC%  ( call :error '%1': no such file && exit /b )

if exist %DST%\ (
	set DST_DIR=%DST%
	set DST=%DST%\%~nx1
) else (
	set DST_DIR=%~dp2
)

::----------------------------------------------
:: コピー先のディレクトリがなければ作成する
::----------------------------------------------
if not exist %DST_DIR% (
	echo creating directory "%DST_DIR%"
	mkdir %DST_DIR%
)

::----------------------------------------------
:: コピーの実行
::----------------------------------------------
set DO_COPY=
if "%COPYCONTROL%" == ""	set DO_COPY=yes
if "%COPYCONTROL%" == "copy"	set DO_COPY=yes

if "%DO_COPY%" == "yes" ( 
	echo copying %SRC% to %DST%
	copy %SRC% %DST% > NUL 2>&1
)
exit /b


::----------------------------------------------
:: エラーメッセージと使用方法を出力する
::----------------------------------------------
:error
	echo Error: %*
	echo.
	call :usage
exit /b 

::----------------------------------------------
:: 使用方法を出力する
::----------------------------------------------
:usage
	echo Usage: %PROG% src-path dst-path
	echo     src-path:    file path to be copied.
	echo     dst-path:    file path or dorectory path to copy.
exit /b

::end: Install.bat
