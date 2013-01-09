@echo off
:: ***********************************************************************************
::  File:
::	do_swigall.bat
::
::  Description:
::	処理対象となる各プロジェクトのディレクトリを巡り, 次の処理を行なう.
::	  (1) nmake -f Makefile.swig の実行
::	  (2) make_manager.bat -r の実行
::
::	(1) 条件が満たされた場合には,
::	　　(a) make_manager.bat -t を実行して, 必要なディレクトリを再スキャンした
::		makefile (=Makefile.swig.tmp) をテンポラリに作成する.
::	　　(b) RunSwig.bat を実行する.
::
::	(2) (1)-(a) で作成したテンポラリの Makefile.swig.tmp を, 正式の makefile で
::	　　ある Makefile.swig とする.
::	　　※ RunSwig.bat の実行より先に makefile を再作成する必然性はないが, 実行
::	　　　 順序の違和感をなくすためにこうする.
::
::	RunSwig プロジェクトの構成プロパティ ([NMake → 全般] のコマンドライン) に
::	次の設定をしておくこと.
::	　　ビルド		make_manager.bat -A -c && nmake -f Makefile
::	　　すべてリビルド	make_manager.bat -A -d -c && nmake -f Makefile
::	　　クリーン		make_manager.bat -A -d
::
::	依存関係を定義したファイル do_swigall.projs (=%PROJFILE%) の書式：
::	    <line> ::= <project> ' ' <dependency-list>
::	    <project> ::= 処理の対象とするプロジェクト名 (=モジュール名)
::	    <dependency-list> ::= ε| <dependent-project> ',' <dependency-list>
::	    <dependent-project> ::= 依存関係にあるプロジェクト名 (=モジュール名)
::	    ※ 注意
::		<project>と<dependency-list>の間区切り文字は「空白1個」
::		<dependency-list>の要素間の区切り文字は「カンマ1個」
::
:: ***********************************************************************************
::  Version:
::	Ver 1.0	  2012/10/25	F.Kanehori
::	Ver 1.0a  2012/11/14	F.Kanehori	PATHの設定の変更 (x64版にも対応)
::	Ver 1.0b  2012/12/19	F.Kanehori	PATHの設定の変更 (do_swig.batを移動)
::						ファイル名の変更
::						(swigprojes.list -> do_swigall.projs)
::	Ver 2.0	  2013/01/07	F.Kanehori	全面改訂
:: ***********************************************************************************
setlocal enabledelayedexpansion
set CWD=%cd%
set DEBUG=0

:: ----------
::  各種定義
:: ----------
:: ディレクトリの定義
::
set BINDIR=%CWD%
set ETCDIR=%CWD%
set SRCDIR=..\..\src

:: 使用するファイル名
::
set PROJFILE=do_swigall.projs
set MAKEFILE=Makefile.swig

:: 使用するプログラム名
::
set MAKE=nmake
set MAKEMANAGER=make_manager.bat

:: 使用するパス
::
set X32=C:\Program Files
set X64=C:\Program Files (x86)
set ARCH=
if exist "%X32%" set ARCH=%X32%
if exist "%X64%" set ARCH=%X64%
if "%ARCH%"=="" (
    echo do_swigall: can not find %MAKE% path.
    exit /b
)
set MAKEPATH=%ARCH%\Microsoft Visual Studio 10.0\VC\bin
set SWIGPATH=%SRCDIR%\Foundation
set PATH=%BINDIR%;%SWIGPATH%;%MAKEPATH%;%PATH%

:: ----------
::  処理開始
:: ----------
rem echo start at %CWD%
for /f "tokens=1,*" %%p in (%ETCDIR%\%PROJFILE%) do (
    echo.
    echo *** %%p ***
    :: 各ディレクトリへ移動して make を実行する
    cd %SRCDIR%\%%p
    cmd /c %MAKE% -f %MAKEFILE%
    cmd /c %MAKEMANAGER% -r
    cd %CWD%
)

:: ----------
::  処理終了
:: ----------
endlocal
exit /b
