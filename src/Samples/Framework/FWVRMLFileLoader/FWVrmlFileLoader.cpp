#include "FWVrmlFileLoader.h"
#include <iostream>
#include <sstream>


#define ESC 27

FWVrmlfileLoader::FWVrmlfileLoader(){
	fileName = "./VrmlFiles/sceneSample.wrl";
	bDebug		= false;
}

void FWVrmlfileLoader::Init(int argc, char* argv[]){
	/// シミュレーションの初期化
	CreateSdk();
	GetSdk()->Clear();						// SDKの初期化
	GetSdk()->LoadScene(fileName);			// ファイルのロード

	/// 描画モードの設定
	SetGRHandler(TypeGLUT);					// GLUTで描画
	GRInit(argc, argv);		// 初期化

	/// ウィンドウの作成
	FWWinDesc windowDesc;					// GLのウィンドウディスクリプタ
	windowDesc.title = "FWVrmlFileLoader";	// ウィンドウのタイトル
	CreateWin(windowDesc);					// ウィンドウの作成
	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	
	CreateTimer();
}

void FWVrmlfileLoader::Reset(){
	GetSdk()->Clear();		
	GetSdk()->LoadScene("./xFiles/sceneSample.x");
	GetCurrentWin()->SetScene(GetSdk()->GetScene());
}


void FWVrmlfileLoader::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			exit(0);
			break;
		case 'r':
			Reset();			// ファイルの再読み込み
			break;
		case 'w':				// カメラ初期化	
			break;
		case 'd':				// デバック表示
			bDebug = !bDebug;
			break;
		default:
			break;
	}
}