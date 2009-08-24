#include "FWXfileLoader.h"
#include <iostream>
#include <sstream>


#define ESC 27

FWXfileLoader::FWXfileLoader(){
	fileName = "./xFiles/sceneSample.x";
	bDebug		= false;
}

void FWXfileLoader::Init(int argc, char* argv[]){
	SetGRAdaptee(TypeGLUT);
	GetGRAdaptee()->Init(argc, argv);				// SDKの作成
	CreateSdk();
	GetSdk()->Clear();								// SDKの初期化
	GetSdk()->LoadScene(fileName);

	FWWinDesc windowDesc;							// GLのウィンドウディスクリプタ
	windowDesc.title = "FWXfileLoader";				// ウィンドウのタイトル
	CreateWin(windowDesc);							// ウィンドウの作成
	InitWindow();
	InitCameraView();								// カメラビューの初期化
}

void FWXfileLoader::Reset(){
	GetSdk()->Clear();								// SDKの初期化
	GetSdk()->LoadScene("./xFiles/sceneSample.x");
}

void FWXfileLoader::Timer(){
//	GetGRAdaptee()->Timer();
}
void FWXfileLoader::IdleFunc(){
	Step();
}
void FWXfileLoader::Step(){
	GetSdk()->Step();
	glutPostRedisplay();
}

void FWXfileLoader::Display(){
	/// 描画モードの設定
	GetSdk()->SetDebugMode(bDebug);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
//	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(bDebug);

	/// カメラ座標の指定
	GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
	}

	/// 描画の実行
	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();
	glutSwapBuffers();
}

void FWXfileLoader::InitCameraView(){
	///	Affinef 型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.999816 -0.0126615 0.0144361 -0.499499)"
		"(6.50256e-010 0.751806 0.659384 13.2441)"
		"(-0.019202 -0.659263 0.751667 10.0918)"
		"(     0      0      0      1))"
		);
	issView >> cameraInfo.view;
}

void FWXfileLoader::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			exit(0);
			break;
//		case 'r':
//			Reset();			// ファイルの再読み込み(タイマを止めてないと落ちるので未実装)
//			break;
		case 'w':				// カメラ初期化
			InitCameraView();	
			break;
		case 'd':				// デバック表示
			bDebug = !bDebug;
			break;
		default:
			break;
	}
}