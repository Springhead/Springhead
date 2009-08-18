#include "Framework/SprFWAppSimple.h"
#include <iostream>
#include <sstream>
#include <GL/glut.h>

using namespace std;

FWAppSimple::FWAppSimple(){
	bDrawInfo=false;
}

void FWAppSimple::Init(int argc, char* argv[]){
	SetGRAdaptee(TypeGLUT);
	GetGRAdaptee()->Init(argc, argv);						// Sdkの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.01);

	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "Springhead2";						// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();
	InitCameraView();										// カメラビューの初期化

	BuildObject();											// 剛体を作成

}

void FWAppSimple::Timer(){
	GTimer* timer0 = CreateTimerFunc();			// タイマーの生成
	GetTimerFunc(0)->Set(TimerFunc);			// 呼びだす関数
	GetTimerFunc(0)->Create(GetGRAdaptee());	// GLUT型でタイマーを作成
}
void FWAppSimple::TimerFunc(int id){
	((FWAppSimple*)instance)->GetTimerFunc(0)->Loop();
	((FWAppSimple*)instance)->CallStep();
	((FWAppSimple*)instance)->GetGRAdaptee()->PostRedisplay();
}

void FWAppSimple::CallStep(){
	if(!vfBridge || !vfBridge->Step())
		Step();
}

void FWAppSimple::InitCameraView(){
	//	Affinef 型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWAppSimple::Step(){
	GetSdk()->Step();
}

void FWAppSimple::Display(){
	// 描画モードの設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDrawInfo);
	render->EnableRenderForce(bDrawInfo);
	render->EnableRenderContact(bDrawInfo);

	// カメラ座標の指定
	GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();
	glutSwapBuffers();
}

void FWAppSimple::Reset(){
	GetSdk()->GetScene()->GetPHScene()->Clear();
	BuildObject();
}
