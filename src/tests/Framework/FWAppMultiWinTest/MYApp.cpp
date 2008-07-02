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
		view[i].Pos() = Vec3f(6.0f*cos(Rad(camAngle[i])), 3.0f, 6.0f*sin(Rad(camAngle[i]))) * camZoom[i];	//カメラの座標を指定する
		view[i].LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1000.0, 0.0));
	}
	render->SetViewMatrix(view[winNumber].inv());
}
//=======================================================================================================
// 上位階層で宣言された関数のオーバーロード

void MYApp::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();

	for(int i = 0; i < 2 ; i++){
		FWAppGLUTDesc winDesc;
		winDesc.width			= 480;
		winDesc.height			= 320;
		winDesc.left			= 10;
		winDesc.top				= 30+360*i;
		winDesc.parentWindow	= 0;
		winDesc.fullscreen		= false;
		winDesc.title			= winName[i];
		window[i] = CreateWin(winDesc);

		fwScene[i] = GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
		window[i]->SetScene(fwScene[i]);
	
		MyRenderInit(window[i], i);		
	}
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
