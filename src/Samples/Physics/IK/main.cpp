/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageIKSample
 Springhead2/src/Samples/IK/main.cpp

\ref pagePhysics の逆運動学（IK）機能のデモプログラム。

\secntion secSpecIKSample 仕様
アームの先端剛体の中心位置ががポインタ（球体）の位置になるよう到達運動する．
第一・第二関節ともに可動域制限がかかっている．
- スペースキーでシミュレーション開始．
- 'i'をタイプするとポインタ位置が上に動く．
- 'k'で、下へ
- 'j'で、左へ
- 'l'で、右へ
*/

#include "../../SampleApp.h"

#pragma  hdrstop

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class IKSampleApp : public SampleApp{
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

	IKSampleApp(){
		appName = "IK Sample";
		numScenes   = 1;

		AddAction(MENU_SCENE, ID_UP, "Target move Up");
		AddHotKey(MENU_SCENE, ID_UP, 'i');

		AddAction(MENU_SCENE, ID_DOWN, "Target move Down");
		AddHotKey(MENU_SCENE, ID_DOWN, 'k');

		AddAction(MENU_SCENE, ID_LEFT, "Target move Left");
		AddHotKey(MENU_SCENE, ID_LEFT, 'j');

		AddAction(MENU_SCENE, ID_RIGHT, "Target move Right");
		AddHotKey(MENU_SCENE, ID_RIGHT, 'l');
	}
	~IKSampleApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		curScene = 0;

		GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
		GetCurrentWin()->GetTrackball()->SetPosition(Vec3f(6.5,6,20));

		GetCurrentWin()->GetTrackball()->SetPosition(Vec3d(0,0,50));
	}

	PHSolidIf*         soTarget;
	PHIKEndEffectorIf* ikeTarget;

	// シーン構築
	virtual void BuildScene() {
		PHSdkIf* phSdk = GetFWScene()->GetPHScene()->GetSdk();

		PHSolidDesc descSolid;

		CDRoundConeDesc descCapsule;
		descCapsule.radius = Vec2f(0.3, 0.3);
		descCapsule.length = 4;

		CDSphereDesc descSphere;
		descSphere.radius  = 0.5;

		CDBoxDesc descBox;
		descBox.boxsize = Vec3d(1.0, 0.5, 1.0);

		Posed shapePose; shapePose.Ori() = Quaterniond::Rot(Rad(90), 'x');

		// Base Link
		PHSolidIf* so0 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so0->SetDynamical(false);
		so0->AddShape(phSdk->CreateShape(descCapsule));
		so0->SetShapePose(0, shapePose);

		// Link 1
		PHSolidIf* so1 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so1->SetFramePosition(Vec3d(0,4,0));
		so1->AddShape(phSdk->CreateShape(descCapsule));
		so1->SetShapePose(0, shapePose);

		// Link 2
		PHSolidIf* so2 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so2->SetFramePosition(Vec3d(0,8,0));
		so2->AddShape(phSdk->CreateShape(descCapsule));
		so2->SetShapePose(0, shapePose);

		// Pointer
		PHSolidIf* so4 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(-10,0,0));
		so4->AddShape(phSdk->CreateShape(descSphere));
		so4->SetDynamical(false);

		soTarget = so4;

		// ----- ----- ----- ----- -----

		PHHingeJointDesc descJoint;
		descJoint.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descJoint.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descJoint.spring =     10.0;
		descJoint.damper =   1000.0;
		descJoint.fMax   =   1000.0;

		PH1DJointLimitDesc descLimit;
		descLimit.spring = 1000000.0;
		descLimit.damper =   10000.0;

		PHIKHingeActuatorDesc descIKA;
		descIKA.velocityGain = 10.0;

		PHIKEndEffectorDesc descIKE;

		// Base <-> Link 1
		PHHingeJointIf* jo1 = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descJoint)->Cast();
		descLimit.range  = Vec2d(Rad(-90), Rad(90));
		jo1->CreateLimit(descLimit);
		PHIKHingeActuatorIf* ika1 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika1->AddChildObject(jo1);

		// Link 1 <-> Link 2
		PHHingeJointIf* jo2  = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descJoint)->Cast();
		descLimit.range  = Vec2d(Rad(0), Rad(180));
		jo2->CreateLimit(descLimit);
		PHIKHingeActuatorIf* ika2 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika2->AddChildObject(jo2);

		// Link2 = End Effector
		PHIKEndEffectorIf* ike1 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so2);
		ika1->AddChildObject(ike1);
		ika2->AddChildObject(ike1);
		ike1->SetTargetPosition(Vec3d(-5,0,0));

		ikeTarget = ike1;

		// ----- ----- ----- ----- -----

		ike1->SetTargetPosition(soTarget->GetPose().Pos());
		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE){
			if(id == ID_UP){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.0,  0.1,  0.0));
				ikeTarget->SetTargetPosition(soTarget->GetPose().Pos());
			}

			if(id == ID_DOWN){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.0, -0.1,  0.0));
				ikeTarget->SetTargetPosition(soTarget->GetPose().Pos());
			}

			if(id == ID_LEFT){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d(-0.1,  0.0,  0.0));
				ikeTarget->SetTargetPosition(soTarget->GetPose().Pos());
			}

			if(id == ID_RIGHT){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.1,  0.0,  0.0));
				ikeTarget->SetTargetPosition(soTarget->GetPose().Pos());
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
int main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
