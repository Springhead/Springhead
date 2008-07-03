#include "MYApp.h"
#include <Framework/SprFWAppGLUT.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define ESC 27

//=======================================================================================================
// コンストラクタ・デストラクタ
MYApp::MYApp(){
	instance	= this;
	dt			= 0.05;
	nIter		= 20;
	numWindow	= 5;
	winName[0]  = "Window A";
	winName[1]  = "Window B";
	for(int i = 0; i < 2; i++){
		window[i]	= NULL;
		fwScene[i]	= NULL;
		camAngle[i] = 0.0f;
		camZoom[i]  = 2.0f;
	}
}

//=======================================================================================================
// クラス内の関数定義
void MYApp::MyRenderInit(FWWin* window, int winNumber){
	GRCameraDesc cam;
	cam.size = Vec2f(0.05f, 0.0);
	GRDebugRenderIf* render = window->render->Cast();
	render->SetCamera(cam);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	GRLightDesc light[2];
	light[0].position = Vec4f(10.0,  20.0, 20.0, 1.0);
	light[1].position = Vec4f(10.0, 20.0, -20.0, 1.0);
	for(int i = 0 ; i < 2; i++){
		render->PushLight(light[i]);
		//view[i].Pos() = Vec3f(6.0f*cos(Rad(camAngle[i])), 3.0f, 6.0f*sin(Rad(camAngle[i]))) * camZoom[i];	//カメラの座標を指定する
		//view[i].LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1000.0, 0.0));
	}
	render->SetViewMatrix(view[winNumber].inv());
}
//=======================================================================================================
// 上位階層で宣言された関数のオーバーロード

void MYApp::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();
	GetSdk()->SetDebugMode(true);
	for(int i = 0; i < numWindow ; i++){
		FWAppGLUTDesc winDesc;
		{
			winDesc.width			= 480;
			winDesc.height			= 320;
			winDesc.left			= 10 + 480*(i/2);
			winDesc.top				= 30 + 360*i;
			winDesc.parentWindow	= 0;
			winDesc.fullscreen		= false;
			if(winName[i]){
				winDesc.title		= winName[i];
			}else{
				winDesc.title		= "window";
			}
		}
		window[i] = CreateWin(winDesc);

		PHSceneDesc phDesc;
		{
			phDesc.timeStep			= dt;
			phDesc.numIteration		= nIter;
		}
		fwScene[i] = GetSdk()->CreateScene(phDesc, GRSceneDesc());
		
		window[i]->SetScene(fwScene[i]);
		window[i]->SetRender(GetSdk()->CreateRender());

		MyRenderInit(window[i], i);		
	
		PHSolidDesc descSolid;													//床の初期化用のディスクリプタの宣言
		{
			descSolid.dynamical = false;										//床だから物理法則を切る
		}
		
		PHSolidIf* soFloor = window[i]->GetScene()->GetPHScene()->CreateSolid(descSolid);			//剛体インタフェースにディスクリプタの中身を渡した”剛体という概念”を登録する
		soFloor->SetName("Floor");
		CDBoxDesc descBox;														//床の衝突判定用のディスクリプタの宣言
		{
			descBox.boxsize			= Vec3f(30*i, 2, 30);						//箱のサイズの指定
			descBox.material.mu		= (float) 1.0;
			descBox.material.mu0	= (float) 1.0;
		}
		soFloor->AddShape(window[i]->GetScene()->GetPHScene()->GetSdk()->CreateShape(descBox));		//先ほど登録した”剛体という概念”に衝突判定できる実体を与える
		soFloor->SetFramePosition(Vec3f(0, 0, 0));								//実体を持つ剛体の設置場所を指定する
	}
	return;
}

void MYApp::Keyboard(int key, int x, int y){
	if(key == 'q'){
		exit(0);
	} else if(key == ESC){
		exit(0);
	} else{
		;
	}
}

//void MYApp::Display(){
//	if(!GetCurrentWin()->GetRender())return;
//	fwSdk->SwitchScene(GetCurrentWin()->GetScene());
//	fwSdk->SwitchRender(GetCurrentWin()->GetRender());
//	fwSdk->GetRender()->ClearBuffer();
//	fwSdk->GetRender()->BeginScene();
//	if(fwSdk->GetScene())
//		fwSdk->GetScene()->Draw(fwSdk->GetRender(), true);
//	fwSdk->GetRender()->EndScene();
//}