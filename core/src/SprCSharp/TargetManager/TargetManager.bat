@echo off
:: ***********************************************************************************
::  FILE:
::      TargetManager.bat
::
::  SYNOPSIS:
::	TargetManager target
::	    target:		ターゲット名
::
::  DESCRIPTION:
::      ファイル"target.last"に記録されたターゲット名と引数で指定されたターゲット名
::	とが異なっていたならば、ターゲット RunSwig_CSharp が生成するファイルを削除
::	することでこのターゲットが必ず実行されるようにする.
::
:: ***********************************************************************************
::  Version:
::	Ver 1.0	 2016/12/07 F.Kanehori	初版
::	Ver 1.1	 2017/01/16 F.Kanehori	NameManger 導入
::	Ver 1.1a 2017/01/18 F.Kanehori	Bug fixed.
:: ***********************************************************************************
setlocal enabledelayedexpansion
set PROG=%~n0
set DEBUG=1

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

:: ------------------------
::  共通環境変数を読み込む
:: ------------------------
call ..\NameManager\NameManager.bat
if %DEBUG% == 1 (
	echo TARGET FILE:  %TARGETFILE%
	echo CLEAN SCRIPT: %CSHARPCLEAN%
)

:: ----------------------------------
::  記録されたターゲット名を読み出す
:: ----------------------------------
for /f %%t in (%TARGETFILE%) do set LASTTARGET=%%t
if %DEBUG% == 1 echo LASTTARGET: %LASTTARGET%

:: --------------------------
::  比較を行ない処理を決める
:: --------------------------
if "%TARGET%" equ "%LASTTARGET%" (
	:: 一致したので何もしなくて良い
	echo %PROG%: %LASTTARGET% -^> %TARGET%
) else (
	:: 異なったのでファイルの削除を行なう
	echo %PROG%: %LASTTARGET% -^> %TARGET%, clearing files
	%CSHARPCLEAN% 
)

:: ----------
::  処理終了
:: ----------
endlocal
exit /b

:: -----------------------------------------------------------------------------------
:: -----------------------------------------------------------------------------------
:usage
	echo Usage: %PROG% target
	echo            target:	ALL ^| Physics ^| ...
exit /b

::end TargetManager.bat
