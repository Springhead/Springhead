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
	zP			= 1.0;
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

	/*
	{
		PHSolidDesc descSolid;
		CDBoxDesc descBox;
		CDRoundConeDesc descRC;
		CDCapsuleDesc descCap;

		descSolid.dynamical = false;
		PHSolidIf* soFloor = phScene->CreateSolid(descSolid);
		soFloor->SetFramePosition(Vec3f(0,-2.5,0));
		descBox.boxsize = Vec3f(10,5,10);
		soFloor->AddShape(phSdk->CreateShape(descBox));

		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		soRC = phScene->CreateSolid(descSolid);
		soRC->SetFramePosition(Vec3f(0,5,0));
		soRC->SetOrientation(Quaterniond::Rot(Rad(45.0), 'x'));

		descRC.length = 1.0;
		descRC.radius = Vec2f(0.3, 0.5);
		soRC->AddShape(phSdk->CreateShape(descRC));
	}
	*/

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// ユーザの指
	PHSolidDesc descSolid;
	CDSphereDesc descSphere;
	descSolid.dynamical = false;
	soCursor = phScene->CreateSolid(descSolid);
	soCursor->SetFramePosition(Vec3f(0,5,0));
	descSphere.radius = 0.1;
	soCursor->AddShape(phSdk->CreateShape(descSphere));

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// クリーチャの作成
	/// クリーチャ
	CRCreatureDesc descCreature;
	creature = (DBG_NEW CRCreature(descCreature, phScene))->Cast();

	/// ボディ
	body = creature->CreateBody(descBody)->Cast();
	PHSolidIf* waist = body->GetSolid(CRBallHumanBodyDesc::SO_WAIST);
	waist->SetDynamical(false);

	// body->GetIKNode(CRBallHumanBodyDesc::JO_LEFT_WRIST)->AddControlPoint(body->GetControlPoint(2*CRBallHumanBodyDesc::SO_LEFT_HAND));
	body->GetIKNode(CRBallHumanBodyDesc::JO_LEFT_ELBOW)->AddControlPoint(body->GetControlPoint(2*CRBallHumanBodyDesc::SO_LEFT_HAND));
	body->GetIKNode(CRBallHumanBodyDesc::JO_LEFT_SHOULDER)->AddControlPoint(body->GetControlPoint(2*CRBallHumanBodyDesc::SO_LEFT_HAND));
	body->GetIKNode(CRBallHumanBodyDesc::JO_ABDOMEN_CHEST)->AddControlPoint(body->GetControlPoint(2*CRBallHumanBodyDesc::SO_LEFT_HAND));
	body->GetIKNode(CRBallHumanBodyDesc::JO_WAIST_ABDOMEN)->AddControlPoint(body->GetControlPoint(2*CRBallHumanBodyDesc::SO_LEFT_HAND));

	/// 到達運動コントローラ
	CRReachingControllersDesc reachsDesc;
	reaches = creature->CreateController(reachsDesc)->Cast();

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	// Sceneの設定
	/// IKの設定
	phScene->GetIKEngine()->SetNumIter(10);
	phScene->GetIKEngine()->Enable(bIK);
	/// その他の設定
	// phScene->SetContactMode(PHSceneDesc::MODE_NONE);
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

	phScene->ClearForce();

	creature->Step();

	phScene->GenerateForce();
	phScene->Integrate();
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

	CRReachingControllerIf* rLHand = reaches->GetReachingController(body->GetSolid(CRBallHumanBodyDesc::SO_LEFT_HAND));
	rLHand->Render(curRender);

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

		case 'o':
			{
				CRReachingControllerIf* rLHand = reaches->GetReachingController(body->GetSolid(CRBallHumanBodyDesc::SO_LEFT_HAND));
				rLHand->Start(soCursor->GetPose().Pos(), Vec3d(), 3.0f);
			}
			break;

		case 'i':
			{
				body->GetSolid(CRBallHumanBodyDesc::SO_LEFT_HAND)->AddForce(Vec3d(0, -10.0, 0));
				// soRC->AddTorque(Vec3d(100,0,0));
			}
			break;

		case 'z':
			{
				zP += 0.2;
				UpdateCursor(mouseInfo.lastPos[0], mouseInfo.lastPos[1]);
			}
			break;

		case 'x':
			{
				zP -= 0.2;
				UpdateCursor(mouseInfo.lastPos[0], mouseInfo.lastPos[1]);
			}
			break;

		default:
			break;
	}
}

void VirtualHuman::UpdateCursor(int x, int y){
	GRRenderIf* render = window->render->Cast();
	Affinef mat = cameraInfo.view;
	Vec2f  vs = render->GetViewportSize();
	Vec2f hVS = vs / 2.0;
	double  r = (vs[0] / vs[1]);
	Vec3f  vF = mat * Vec3f((x-hVS[0])/hVS[0]*zP, -(y-hVS[1])/hVS[1]*zP, -1*zP);
	soCursor->SetCenterPosition(vF);
}

void VirtualHuman::MouseButton(int button, int state, int x, int y){
	mouseInfo.lastPos.x = x, mouseInfo.lastPos.y = y;
	if(button == LEFT_BUTTON)
		mouseInfo.left = (state == BUTTON_DOWN);
	if(button == MIDDLE_BUTTON)
		mouseInfo.middle = (state == BUTTON_DOWN);
	if(button == RIGHT_BUTTON)
		mouseInfo.right = (state == BUTTON_DOWN);
	if(state == BUTTON_DOWN)
		mouseInfo.first = true;
	int mod = GetModifier();
	mouseInfo.shift = (mod & ACTIVE_SHIFT) != 0;
	mouseInfo.ctrl  = (mod & ACTIVE_CTRL) != 0;
	mouseInfo.alt   = (mod & ACTIVE_ALT) != 0;
}

void VirtualHuman::MouseMove(int x, int y){
	if (mouseInfo.middle) {
		UpdateCursor(x, y);
	} else {
		FWAppGLUT::MouseMove(x,y);
	}
}
