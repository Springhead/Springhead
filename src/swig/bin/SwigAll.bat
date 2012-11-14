@echo off
:: *******************************************************************************
::  File:
::	SwigAll.bat
::
::  Description:
::	他のすべてのプロジェクトのビルドに先立って、必要な *stub.cpp を作成
::	するためのスクリプト.
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
:: *******************************************************************************
::  Version:
::	Ver 1.0	  2012/10/17	F.Kanehori
::	Ver 1.0a  2012/11/14	F.Kanehori  このスクリプトの格納場所を変更
::					    PATHの設定の変更 (x64版にも対応)
:: *******************************************************************************
setlocal
set CWD=%cd%

set X64PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE
set X32PATH=C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE
set DEVENV=devenv.exe
if exist "%X32PATH%\%DEVENV%" set PATH=%X32PATH%;%PATH%
if exist "%X64PATH%\%DEVENV%" set PATH=%X64PATH%;%PATH%

set SOLUTION=..\src\Springhead10.sln
devenv %SOLUTION% /build  Debug

cd %CWD%
endlocal
exit /b
