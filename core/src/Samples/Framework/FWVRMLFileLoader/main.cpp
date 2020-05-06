#include "FWVrmlFileLoader.h"
/**
	シンプルなXfileローダー
	シミュレーションはglutIdleFuncで進める
	q, ESC	: アプリケーション終了
	r		: ファイルのリロード
	w		: カメラ行列の初期化
	d		: デバック表示切り替え
*/

FWVrmlfileLoader app;
int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
