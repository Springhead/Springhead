setlocal enabledelayedexpansion
@echo off
@echo ライブラリをマージします．VisualC++のライブラリ管理ツール LIB.EXE にパスを通しておいてください．

set CONVENTION=old
if /i "%1"=="win32" set CONVENTION=new
if /i "%1"=="win64" set CONVENTION=new

if "%CONVENTION%"=="new" (
    :: Visual Studio 2012 以降に適用
    set OUTDIR=%1
    set EXT=%2
)
if "%CONVENTION%"=="old" (
    rem
    rem Visual Studio 2010 以前に適用
    rem
    if /i "%1"=="RELEASE" set EXT=
    if /i "%1"=="DEBUG" set EXT=D
    if /i "%1"=="TRACE" set EXT=T
    if /i "%1"=="MFCDEBUG" set EXT=MD
    if /i "%1"=="MFCRELEASE" set EXT=M
    if /i "%1"=="DEBUGDLL" set EXT=MD
    if /i "%1"=="RELEASEDLL" set EXT=M
    if "%2"=="7" set EXT=7!EXT!
    if "%2"=="8" set EXT=8!EXT!
    if "%2"=="9" set EXT=9!EXT!
    if "%2"=="10" set EXT=10!EXT!
    rem 
    rem 第3引数を追加します（Platform - Defaultは"Win32"）
    if /i "%3"=="x64" (
	    set OUTDIR=win64
	    set EXT=!EXT!x64
    ) else (
	    set OUTDIR=win32
    )
)
rem echo param [%1],[%2]
rem echo OUTDIR,EXT [%OUTDIR%],[%EXT%]
rem exit /b

set OUTPUT=../Lib/%OUTDIR%/Springhead%EXT%.lib
set INPUT=Base/Base%EXT%.lib
set INPUT=%INPUT% Foundation/Foundation%EXT%.lib 
set INPUT=%INPUT% Collision/Collision%EXT%.lib
set INPUT=%INPUT% Physics/Physics%EXT%.lib
set INPUT=%INPUT% Graphics/Graphics%EXT%.lib
set INPUT=%INPUT% FileIO/FileIO%EXT%.lib
set INPUT=%INPUT% Framework/Framework%EXT%.lib
set INPUT=%INPUT% HumanInterface/HumanInterface%EXT%.lib
set INPUT=%INPUT% Creature/Creature%EXT%.lib

if "%INPUT%"=="" echo 環境変数用メモリの空きが不足しています．
if "%INPUT%"=="" echo 空きを増やしてもう一度実行します．
if "%INPUT%"=="" echo このメッセージが続けて表示される場合は，
if "%INPUT%"=="" echo コマンドプロンプトの設定を修正してください．
if "%INPUT%"=="" command /e:4096 /c%0 %1 %2 %3
if not "%INPUT%"=="" (
    LIB /OUT:%OUTPUT% %INPUT%
    echo Springhead: %OUTPUT% created.
)
endlocal
exit /b
