@echo off
setlocal enabledelayedexpansion
rem ===========================================================================
rem @echo ライブラリをマージします．VisualC++のライブラリ管理ツール LIB.EXE にパスを通しておいてください．

rem 引数の処理
rem	第1引数：　platform (x86|win32 または x64|win64)
rem	第2引数：　ライブラリ名 ([VS-version]+[configuration]+[platform])
rem 
set SUBDIR=%1
if "!%SUBDIR!"=="x64" set SUBDIR=win64 
set EXT=%2

rem 出力先
rem	$(SPR_TOP)/generated/lib/{win32|win64}
rem 
set GENTOP=..\..\generated\lib
set GENDIR=%GENTOP%\%SUBDIR%
call :create_dir OUTDIR %GENDIR%
if not exist %OUTDIR% (
    mkdir %OUTDIR%
)
set OUTPUT=%OUTDIR%/Springhead%EXT%.lib

echo off
rem echo param [%1],[%2]
rem echo GENIR,EXT [%GENDIR%],[%EXT%]

rem 入力ファイル
rem 
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


rem libの実行
rem 
if not "%INPUT%"=="" (
    LIB /OUT:%OUTPUT% %INPUT%
    echo Springhead: %OUTPUT% created.
)

endlocal
exit /b

rem ===========================================================================
rem  絶対パスの設定
rem 
:create_dir
    set %1=%~f2
exit /b

