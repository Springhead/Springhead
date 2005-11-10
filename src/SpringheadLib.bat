@echo off
@echo ライブラリをマージします．VisualC++のライブラリ管理ツール LIB.EXE にパスを通しておいてください．

if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD
if "%2"=="7" set EXT=7%EXT%

set OUTPUT=Springhead%EXT%.lib
set INPUT=Base/Base%EXT%.lib Collision/Collision%EXT%.lib Device/Device%EXT%.lib
set INPUT=%INPUT% FileIO/FileIO%EXT%.lib Graphics/Graphics%EXT%.lib HIS/HIS%EXT%.lib
set INPUT=%INPUT% ImpD3D/ImpD3D%EXT%.lib Physics/Physics%EXT%.lib SceneGraph/SceneGraph%EXT%.lib
set INPUT=%INPUT% WinBasis/WinBasis%EXT%.lib WinDX/WinDX%EXT%.lib Framework/Framework%EXT%.lib
set INPUT=%INPUT% GraphicsGL/GraphicsGL%EXT%.lib

if "%INPUT%"=="" echo 環境変数用メモリの空きが不足しています．
if "%INPUT%"=="" echo 空きを増やしてもう一度実行します．
if "%INPUT%"=="" echo このメッセージが続けて表示される場合は，
if "%INPUT%"=="" echo コマンドプロンプトの設定を修正してください．
if "%INPUT%"=="" command /e:4096 /c%0 %1
if not "%INPUT%"=="" LIB /OUT:%OUTPUT% %INPUT%
