@echo off
setlocal enabledelayedexpansion
:: ============================================================================
::  SYNOPSIS
::	SwigAll toolset_id configuration platform
::
::  DESCRIPTION
::	daily_build において、他のすべてのプロジェクトのビルドに先立って必要な
::	*stub.cpp を作成するためのスクリプト.
::
::	処理は RunSwig プロジェクト (make プロジェクト) で行なうため、stub の
::	作成が必要となるプロジェクトについては、次のことをしておこくと.
::	  (1) プロジェクトの依存関係: RunSwig プロジェクトに依存させる.
::	  (2) 処理の対象となるプロジェクト名を、次のファイルに記述する.
::		$(ETCDIR)/swigprojs.list
::	記述は、各プロジェクト1行ずつとする.
::	このファイル名(="swigprojs.list")は、次のスクリプト内で変更できる.
::		$(BINDIR)/do_swigall.bat
::
::	現状では次のようになっている.
::		$(BINDIR) = .../Springhead2/src/swig/bin
::		$(ETCDIR) = .../Springhead2/src/swig/etc
::	
::	処理の内容については、make プロジェクトを参照のこと.
::		※ 実際の処理は $(BINDIR)/do_swigall.bat が行なう.
::
::  ARGUMENTS
::	toolset_id	ツールセットの識別 { 10.0 | 11.0 | .. }
::	configuration	ビルド構成 { Debug | Release | .. }
::	platform	ビルドプラットフォーム { Win32 | x64 }
::
::  VERSION
::	Ver 1.0	  2012/10/17	F.Kanehori
::	Ver 1.0a  2012/11/14	F.Kanehori  このスクリプトの格納場所を変更
::					    PATHの設定の変更 (x64版にも対応)
::	Ver 1.0b  2012/11/21	F.Kanehori  スクリプトの格納場所を元に戻した
::					    PATHの設定の変更 (エラー検査追加)
::	Ver 2.0	  2013/09/11	F.Kanehori  バッチファイルの再構成
::	Ver 2.1	  2013/12/05	F.Kanehori  Visual Studio 2008 対応
::	Ver 2.1a  2016/06/02	F.Kanehori  コマンドライン情報をログに出力
:: ============================================================================
call :leaf_name PROG %0
set CWD=%cd%

::----------------------------------------------
:: 引数の処理
::
set TOOLSET_ID=%1
set CONFIGURATION=%2
set PLATFORM=%3

set ARGS=TOOLSET_ID CONFIGURATION PLATFORM
for %%a in (%ARGS%) do (
	if "!%%a!" equ "" (echo %PROG%: missing argument %%a && exit /b -1)
)

if "%TOOLSET_ID%" equ "9" set TOOLSET_ID=9.0
if "%TOOLSET_ID%" equ "10" set TOOLSET_ID=10.0
set SOLUTIONFILE_ID=%TOOLSET_ID%
if "%SOLUTIONFILE_ID%" equ "9.0" set SOLUTIONFILE_ID=9
if "%SOLUTIONFILE_ID%" equ "10.0" set SOLUTIONFILE_ID=10

rem ------------------------------
rem  必要なパスの設定
rem 
set DEVENVPATH=Microsoft Visual Studio %TOOLSET_ID%\Common7\IDE
set X64PATH="C:\Program Files (x86)\%DEVENVPATH%"
set X32PATH="C:\Program Files\%DEVENVPATH%"
set PROG=devenv.exe
set MAKEPATH=..\bin;.\bin\swig
if exist "%X32PATH:"=%\%PROG%" set PATH=%MAKEPATH%;%X32PATH:"=%;%DEVENVPATH%;%PATH%
if exist "%X64PATH:"=%\%PROG%" set PATH=%MAKEPATH%;%X64PATH:"=%;%DEVENVPATH%;%PATH%
rem if exist "%X32PATH:"=%\%PROG%" echo System: 32 bits
rem if exist "%X64PATH:"=%\%PROG%" echo System: 64 bits

set SOLUTIONDIR=..\src
set SOLUTIONFILE=Springhead%SOLUTIONFILE_ID%.sln
set SOLUTION=%SOLUTIONDIR%\%SOLUTIONFILE%

set BUILD_OPTION="%CONFIGURATION%|%PLATFORM%"
echo [ SwigAll.bat ]
echo %% devenv %SOLUTION% /build %BUILD_OPTION%
devenv %SOLUTION% /Build %BUILD_OPTION%

endlocal
exit /b


::----------------------------------------------
:: リーフ名の取出し
::	arg1:	値を返す環境変数名
::	arg2:	パス名
::
:leaf_name
	set %1=%~n2
exit /b
::end SwigAll.bat
