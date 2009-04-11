#include "FWAppSample.h"
/**
\page pageFWAppSample フレームワークSDKを使った一番簡単なサンプル
 Springhead2/src/Samples/FWAppSample

- プログラムの機能
 - 床の上に箱が載っているだけのシーン。
 - 描画は \ref pagePhysics のデバッグ表示のみ。
 - i キーで、接触力や接触部情報の表示のON/OFF。
- ソースについて
 - シーンの構築は、C++言語でAPIを呼び出して行う。
 - \ref pageFramework と \ref pagePhysics のみを使用
*/

FWAppSample app;
int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.Start();
	return 0;
}
