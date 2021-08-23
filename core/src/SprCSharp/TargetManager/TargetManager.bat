@echo off
setlocal enabledelayedexpansion
:: =============================================================================
::  SYNOPSIS:
::	TargetManager target
::	    target:		ターゲット名
::
::  DESCRIPTION:
::	指定されたターゲットが必ず実行されるように制御する.
::	具体的には, 指定されたターゲット名がファイル target.last に記録されて
::	いるものとが異なるならば, ターゲット RunSwig_CSharp が生成するファイル
::	を削除する.
::
:: -----------------------------------------------------------------------------
::  VERSION:
::    Ver 1.0	 2016/12/07 F.Kanehori	初版
::    Ver 1.1	 2017/01/16 F.Kanehori	NameManger の導入.
::    Ver 1.1.1	 2021/07/28 F.Kanehori	見直し.
:: =============================================================================
set PROG=%~n0
set DEBUG=0

:: ----------------------------------------------------------------------
::  このスクリプトは "<SprTop>/core/src/SprCSharp/TargetManager" に置く
::
set CWD=%cd%
cd /d %~dp0\..\..\..\..
set SprTop=%CD%
cd %CWD%
set CspDir=%SprTop%\core\src\SprCSharp

:: ------------------------
::  共通環境変数を読み込む
:: ------------------------
set NAMEMANAGER=%CspDir%\NameManager\NameManager.bat
if not exist %NAMEMANAGER% (
	:: NameManager.bat が存在しないときは何もしない
	echo "NameManager.bat" not found.
	exit /b
)
call %NAMEMANAGER%
if %DEBUG% == 1 (
	echo TARGET FILE:  %TARGETFILE%
	echo CLEAN SCRIPT: %CSHARPCLEAN%
)

:: ------------
::  引数の処理
:: ------------
set TARGET=%1
if "%TARGET%" equ "" (
	echo %PROG%: Error: missing arg "target"
	echo.
	call :usage
	exit /b
)

:: ----------------------------------
::  記録されたターゲット名を読み出す
:: ----------------------------------
for /f %%t in (%TARGETFILE%) do set LASTTARGET=%%t
if %DEBUG% == 1 (
	echo LASTTARGET: %LASTTARGET%
)

:: --------------------------
::  比較を行ない処理を決める
:: --------------------------
if "%TARGET%" equ "%LASTTARGET%" (
	:: 一致したので何もしなくて良い
	echo+  %PROG%: %LASTTARGET% -^> %TARGET%
) else (
	:: 異なったのでファイルの削除を行なう
	echo+  %PROG%: %LASTTARGET% -^> %TARGET%, clearing files
	%CSHARPCLEAN%
	echo %TARGET% > %TARGETFILE%
)

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:: ---------------------------------------------------------------------
:usage
	echo Usage: %PROG% target
	echo            target:	ALL ^| Physics ^| ...
exit /b

::end TargetManager.bat
