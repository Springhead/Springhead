#include "VirtualHuman.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

#include <GL/glut.h>

#include "Physics/PHConstraint.h"
#include "Physics/PHSpatial.h"
#include "Physics/PHScene.h"

#define ESC 27

VirtualHuman app;

VirtualHuman::VirtualHuman(){
	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= false;

	gravity		= Vec3d(0.0, -9.8, 0.0);
}

void VirtualHuman::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());

	BuildScene();

	FWWinDesc windowDesc;
	windowDesc.title = "Virtual Human Sample";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void VirtualHuman::Reset(int sceneNum){
	GetSdk()->Clear();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());

	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= false;

	BuildScene(sceneNum);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	InitCameraView();
}

void VirtualHuman::InitCameraView(){
	istringstream iss(
		"((0.9996 0.00141267 -0.0282299 -0.0937266)"
		"(1.59067e-012 0.99875 0.0499792 0.165937)"
		"(0.0282652 -0.0499592 0.998351 3.31465)"
		"(	   0      0      0      1))"
		);
	iss >> cameraInfo.view;
}

void VirtualHuman::BuildScene(int sceneNum){
	PHSdkIf*	phSdk	= GetSdk()->GetPHSdk();
	PHSceneIf*	phScene	= GetSdk()->GetScene()->GetPHScene();

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// クリーチャの作成
	/// クリーチャ
	CRCreatureDesc descCreature;
	creature = (DBG_NEW CRCreature(descCreature, phScene))->Cast();

	/// ボディ
	body = creature->CreateBody(descBody)->Cast();
	PHSolidIf* waist = body->GetSolid(CRBallHumanBodyDesc::SO_WAIST);
	waist->SetDynamical(false);

	/// 到達運動コントローラ
	CRReachingControllersDesc reachsDesc;
	reaches = creature->CreateController(reachsDesc)->Cast();

	/// 視線コントローラ
	CREyeControllerDesc descEyeCtrl;
	eyeCtrl = creature->CreateController(descEyeCtrl)->Cast();

	/// 首コントローラ
	CRNeckControllerDesc descNeckCtrl;
	{
		descNeckCtrl.lowerAttractiveness = 0.0;
		descNeckCtrl.upperAttractiveness = 7.0;
	}
	neckCtrl = creature->CreateController(descNeckCtrl)->Cast();

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// Sceneの設定
	/// IKの設定
	phScene->GetIKEngine()->SetNumIter(10);
	phScene->GetIKEngine()->Enable(bIK);
	/// その他の設定
	phScene->SetContactMode(PHSceneDesc::MODE_NONE);
	phScene->SetGravity(gravity);
	phScene->SetTimeStep(0.01);
	phScene->SetNumIteration(15);
}

void VirtualHuman::Step(){
	if(bStep) {
		OneStep();
	} else if (bOneStep) {
		OneStep();
		bOneStep = false;
	}

	glutPostRedisplay();
}

void VirtualHuman::OneStep(){
	PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();

	phScene->Step();
}

void VirtualHuman::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDebug);
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
	render->PopLight();

	curRender->EndScene();
	glutSwapBuffers();
}

void VirtualHuman::Keyboard(int key, int x, int y){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:

		case 'q':
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
				phscene->SetGravity(Vec3d(0,0,0));
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

		case 'f':
			{
				bIK = !bIK;
				DSTR << (bIK ? "IK ON" : "IK OFF") << endl;
				phscene->GetIKEngine()->Enable(bIK);
			}
			break;

		case '1':
			Reset(0);
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
