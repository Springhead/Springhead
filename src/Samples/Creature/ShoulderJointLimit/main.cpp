/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageShoulderJointLimit
 Springhead2/src/Samples/Creature/ShoulderJointLimit/main.cpp

\ref 肩関節可動域制限のデモ

\secntion secSpecShoulderJointLimit
肩関節の可動域制限をスプライン閉曲線で指定するデモです．
*/

#include <vector>
#include "../../SampleApp.h"

#include <Creature/CRBodyGenerator/CRBallHumanBodyGen.h>

#pragma  hdrstop

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class ShoulderJointLimitApp : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_RESET,
		ID_UP,
		ID_DOWN,
		ID_LEFT,
		ID_RIGHT,
	};

	int argc;
	char** argv;

	ShoulderJointLimitApp(){
		appName = "Shoulder Joint Limit Sample";
		numScenes   = 1;

		AddAction(MENU_SCENE, ID_UP, "Pointer move Up");
		AddHotKey(MENU_SCENE, ID_UP, 'i');

		AddAction(MENU_SCENE, ID_DOWN, "Pointer move Down");
		AddHotKey(MENU_SCENE, ID_DOWN, 'k');

		AddAction(MENU_SCENE, ID_LEFT, "Pointer move Left");
		AddHotKey(MENU_SCENE, ID_LEFT, 'j');

		AddAction(MENU_SCENE, ID_RIGHT, "Pointer move Right");
		AddHotKey(MENU_SCENE, ID_RIGHT, 'l');
	}
	~ShoulderJointLimitApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		curScene = 0;

		GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
		GetCurrentWin()->GetTrackball()->SetPosition(Vec3f(3,3,5));
		GetFWScene()->EnableRenderAxis(true, false, false);
		GetFWScene()->EnableRenderLimit(true);
	}

	PHSolidIf*         soTarget;

	// シーン構築
	virtual void BuildScene() {
		PHSdkIf* phSdk = GetFWScene()->GetPHScene()->GetSdk();

		PHSolidDesc descSolid;

		CDSphereDesc descSphere;
		descSphere.radius  = 0.1;

		// Pointer
		PHSolidIf* so4 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(1.2,0,0));
		so4->AddShape(phSdk->CreateShape(descSphere));
		so4->SetDynamical(false);

		soTarget = so4;

		CRSdkIf* crSdk = CRSdkIf::CreateSdk();
		CRCreatureDesc descCreature;
		CRCreatureIf* crCreature = crSdk->CreateCreature(descCreature);
		crCreature->AddChildObject(GetFWScene()->GetPHScene());
		CRBallHumanBodyGenDesc descBody;
		CRBallHumanBodyGen bodyGen(descBody);
		CRBodyIf* body = bodyGen.Generate(crCreature);

		body->FindByLabel("waist")->GetPHSolid()->SetDynamical(false);

		// ----- ----- ----- ----- -----

		PHBallJointSplineLimitDesc descSplineLimit;
		// descSplineLimit.poleTwist = Vec2d(Rad(-90), Rad(0));
		descSplineLimit.limitDir  = Vec3d(0,0,-1);
		descSplineLimit.spring    = 5.0;
		descSplineLimit.damper    = 0.5;

		PHBallJointIf* joLShoulder = body->FindByLabel("left_upper_arm")->GetPHJoint()->Cast();
		PHBallJointSplineLimitIf* limitL = joLShoulder->CreateLimit(descSplineLimit)->Cast();
		limitL->AddNode(Rad(0),Rad(90),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(48.0775),Rad(87.8079),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(132.51),Rad(65.1153),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(226.245),Rad(173.518),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(232.853),Rad(55.2006),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(247.286),Rad(62.1539),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(291.705),Rad(147.814),Rad(50),Rad(10),Rad(0),Rad(-90));
		limitL->AddNode(Rad(360),Rad(90),Rad(50),Rad(10),Rad(0),Rad(-90));

		if (joLShoulder) {
			joLShoulder->SetDamper(0.5);
			joLShoulder->SetSpring(1.0);
		}
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE){
			if(id == ID_UP){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.0,  0.1,  0.0));
			}

			if(id == ID_DOWN){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.0, -0.1,  0.0));
			}

			if(id == ID_LEFT){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d(-0.1,  0.0,  0.0));
			}

			if(id == ID_RIGHT){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.1,  0.0,  0.0));
			}
		}

		SampleApp::OnAction(menu, id);
	}
} app;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int __cdecl main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
