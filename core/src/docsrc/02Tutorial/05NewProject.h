/** \page pageNewProject	新規プロジェクト

\contents

ここでは，Visual Studio 2010 を用いて，新しくSpringhead2を用いたプログラムを作成する方法を説明します．
説明にしたがって作成すると，新しいソリューション first ができます．

\section secCreateNewProj 新規プロジェクトの作成
Visual Studio 2010 を立ち上げて，ファイル(F)-新規作成(N)-プロジェクト(P)を選択して，
下のような「新しいプロジェクト」ダイアログを立ち上げ，「Win32コンソールアプリケーション」
を選択してください．
\image html newProj.png
プロジェクト名や作成場所を指定して，OKでプロジェクトを作成してください．
上の図では，first.sln, first.vcxproj, first.cpp などが作成されます．

\section secAddHeaders インクルードディレクトリの追加
作成したプロジェクトを右クリックし，プロパティ(R)でプロパティを表示し，
\image html addIncludeFolder.png 
のように，「すべての構成」について「C/C++ 全般」の「追加のインクルードディレクトリ」に
「\$(Springhead2)/include」と「\$(Springhead2)/src」を追加してください．
これで，Springheadのヘッダファイルがインクルードできるようになります．
ここで「\$(Springhead2)」は，Springhead2 をチェックアウトしたフォルダを示します．

\section secSetOption オプションの設定
作成したプロジェクトを右クリックし，プロパティ(R)でプロパティを表示し，
C C++-コード生成を選び，ランタイムライブラリを MT(リリース版) または MTd(デバッグ版) に設定します．

\section secAddLibs ライブラリの追加
Springheadを使うためには，ライブラリを追加する必要があります．
新しく作ったプロジェクト (下図のfirst) を右クリックして 追加(D)-既存項目の追加(G)を選び，
\image html addLib.png
<a href="../../doc/SprManual/main.pdf">使い方の説明 (Springhead Users Manual)</a> の 第2章 GettingStarted 
で作成したライブラリファイル lib/win32/Springhead10MD.lib および lib/win32/Springhead10M.lib を追加します．<br>
　(注意) 追加するライブラリファイルは，使用する Visual Studio のバージョンによって異なります．
　<ul>
    <li>Visual Studio 2008 の場合： lib/win32/Springhead9D.lib および lib/win32/Springhead9.lib
    <li>Visual Studio 2010 の場合： lib/win32/Springhead10MD.lib および lib/win32/Springhead10M.lib
    <li>Visual Studio 2012 の場合： lib/win32/Springhead11.0DWin32.lib および lib/win32/Springhead11.0Win32.lib
　</ul>
これらは，それぞれデバッグ版，リリース版のライブラリなので，それぞれを右クリックして
プロパティ(R)でプロパティを表示し，下図のようにDebug版では Springhead10M.lib をビルドから除外し，
Release版では逆に Springhead10MD.lib をビルドから除外してください．
\image html nonBuildSetting.png

\section secEditSrc ソースファイルの編集
あとは，ソースファイルを編集して自分のプログラムを作れば終わりです．
すごく単純な例を出すと：
<pre>
// first.cpp : コンソール アプリケーションのエントリ ポイントを定義します．
//
##include "stdafx.h"
##include <springhead.h>
using namespace Spr;

int _tmain(int argc, _TCHAR* argv[])
{
	PHSdkIf* sdk = PHSdkIf::CreateSdk();		//	物理エンジンSDKの作成
	PHSceneIf* scene = sdk->CreateScene();		//	シーンの作成
	PHSolidIf* solid = scene->CreateSolid();	//	剛体の作成
	for(int i=0; i<10; ++i){
		//	シミュレーションを進める
		scene->Step();
		//	剛体の位置を出力：自由落下の様子が数値で出力される
		std::cout << solid->GetPose().Pos() << std::endl;
	}
	return 0;
}
</pre>
のようになります．
*/
