#include "#{APPLICATIONNAME}.h"
#include <iostream>
#include <ctime>
#include <string>
#include <GL/glut.h>

#define ESC 27

#{APPLICATIONNAME} app;

#{APPLICATIONNAME}::#{APPLICATIONNAME}(){
	dt			= 0.01;
	gravity		= Vec3d(0, -9.8f, 0);
	nIter		= 15;
	bGravity	= true;
	bDebug		= false;
	bStep		= true;
}

void #{APPLICATIONNAME}::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();											// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	DesignObject();												// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	FWWinDesc windowDesc;
	windowDesc.title = "FWApp#{APPLICATIONNAME}";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void #{APPLICATIONNAME}::Reset(){
	GetSdk()->Clear();											// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	DesignObject();												// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration(nIter);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	InitCameraView();
}
void #{APPLICATIONNAME}::InitCameraView(){
	cameraInfo.view[0][0] = 0.9996;
	cameraInfo.view[0][1] = 0.0107463;
	cameraInfo.view[0][2] = -0.0261432;
	cameraInfo.view[0][3] = -0.389004;
	cameraInfo.view[1][0] = -6.55577e-010;
	cameraInfo.view[1][1] = 0.924909;
	cameraInfo.view[1][2] = 0.380188;
	cameraInfo.view[1][3] = 5.65711;
	cameraInfo.view[2][0] = 0.0282657;
	cameraInfo.view[2][1] = -0.380037;
	cameraInfo.view[2][2] = 0.92454;
	cameraInfo.view[2][3] = 13.7569;
	cameraInfo.view[3][0] = 0;
	cameraInfo.view[3][1] = 0;
	cameraInfo.view[3][2] = 0;
	cameraInfo.view[3][3] = 1;
}

void #{APPLICATIONNAME}::DesignObject(){
	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
	// 
	// シーンの内容はここで作成します．
	// 
	// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
}

void #{APPLICATIONNAME}::Step(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	if(bStep) phscene->Step();
	else if (bOneStep){
		phscene->Step();
		bOneStep = false;
	}
	glutPostRedisplay();
}

void #{APPLICATIONNAME}::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	render->SetRenderMode(true, false);
	// render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(bDebug);

	// カメラ座標の指定
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		window->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GRRenderIf*curRender =  GetCurrentWin()->GetRender();
	FWSceneIf* curScene = GetCurrentWin()->GetScene();

	GetSdk()->SwitchScene(curScene);
	GetSdk()->SwitchRender(curRender);

	if(!curRender) return;
	curRender->ClearBuffer();
	curRender->BeginScene();

	if (curScene) curScene->Draw(curRender, GetSdk()->GetDebugMode());

	//	光源の追加
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);
	render->PopLight();	//	光源の削除

	curRender->EndScene();
	glutSwapBuffers();
}

void #{APPLICATIONNAME}::Keyboard(int key, int x, int y){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:
		case  'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		case 'w':
			InitCameraView();
			DSTR << "InitCameraView" << endl;
			break;
		case 'd':
			if(bDebug){
				bDebug = false;
				DSTR << "Debug Mode OFF" << endl;
			}else{
				bDebug = true;
				DSTR << "Debug Mode ON" << endl;
			}
			break;
		case 'g':
			if(bGravity){
				bGravity = false;
				Vec3d zeroG = Vec3d(0.0, 0.0, 0.0);
				phscene->SetGravity(zeroG);
				DSTR << "Gravity OFF" << endl;
			}else{
				bGravity = true;
				phscene->SetGravity(gravity);
				DSTR << "Gravity ON" << endl;
			}
			break;
		case 's':
			bStep = false;
			bOneStep = true;
			break;
		case 'a':
			bStep = true;
			break;
		

			// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
			// 
			// 独自のキーファンクションはここで定義します．
			// 
			// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

		default:
			break;
	}
}
