@echo off
:: ***********************************************************************************
::  File:
::      TargetManager.bat
::
::  SYNOPSIS:
::	TargetManager target
::
::  Description:
::      ファイル"target.last"に記録されたターゲット名と引数で指定されたターゲット名
::	とが異なっていたならば、ターゲット RunSwig_CSharp が生成するファイルを削除
::	することでこのターゲットが必ず実行されるようにする.
::
::  ARGUMENTS:
::	target:		ターゲット名
::	
:: ***********************************************************************************
::  Version:
::	Ver 1.0	 2016/12/07	F.Kanehori  初版
:: ***********************************************************************************
setlocal enabledelayedexpansion
set PROG=%~n0
set DEBUG=0

:: ------------
::  引数の処理
:: ------------
set TARGET=%1
if "%TARGET%" equ "" (
	echo %PROG%: Error: need arg "target"
	exit /b
)

:: --------------------
::  使用するファイル名
:: --------------------
set TARGETFILE=..\TargetManager\target.last

:: --------------------
::  使用するスクリプト
:: --------------------
set CLEAN=..\RunSwig_CSharp\RunSwig_Clean.bat

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
	%CLEAN% 
)

:: ----------
::  処理終了
:: ----------
endlocal

::end TargetManager.bat
