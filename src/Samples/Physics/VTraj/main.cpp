/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageVTrajSample
 Springhead2/src/Samples/VTraj/main.cpp

\ref 柔らかい軌道追従制御のデモプログラム．

\secntion secSpecVTrajSample 仕様
アームの先端剛体の中心位置ががポインタ（球体）の位置になるよう到達運動する．
- スペースキーでシミュレーション開始．
- 'v'で柔らかい追従制御のON/OFFをトグルする．
- 'b'で関節のバネダンパを固くする・柔らかくする．（初期状態では柔らかい）
- 'i'をタイプするとポインタ位置が上に動く．
- 'k'で、下へ
- 'j'で、左へ
- 'l'で、右へ
*/

#include <vector>
#include "../../SampleApp.h"

#pragma  hdrstop

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class VTrajSampleApp : public SampleApp{
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
		ID_PLIANT,
		ID_SOFT,
		ID_STIFF,
	};

	int argc;
	char** argv;

	bool bVT, bHard;
	int  time;

	VTrajSampleApp(){
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

		AddAction(MENU_SCENE, ID_PLIANT, "Pliant Tracking");
		AddHotKey(MENU_SCENE, ID_PLIANT, 'v');

		AddAction(MENU_SCENE, ID_SOFT, "Soft Tracking");
		AddHotKey(MENU_SCENE, ID_SOFT, 'c');

		AddAction(MENU_SCENE, ID_STIFF, "Stiff Tracking");
		AddHotKey(MENU_SCENE, ID_STIFF, 'b');

		bVT   = false;
		bHard = false;
		time = 0;
	}
	~VTrajSampleApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		curScene = 0;

		fwScene->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
		GetCurrentWin()->GetTrackball()->SetPosition(Vec3f(6.5,6,20));

		GetCurrentWin()->GetTrackball()->SetPosition(Vec3d(0,0,50));
	}

	PHSolidIf*         soTarget;
	PHIKEndEffectorIf* ikeTarget;

	// シーン構築
	virtual void BuildScene() {
		PHSdkIf* phSdk = phScene->GetSdk();

		PHSolidDesc descSolid;

		CDRoundConeDesc descCapsule;
		descCapsule.radius = Vec2f(0.3, 0.3);
		descCapsule.length = 4;

		CDSphereDesc descSphere;
		descSphere.radius  = 0.5;

		CDBoxDesc descBox;
		descBox.boxsize = Vec3d(1.0, 1.0, 1.0);

		Posed shapePose; shapePose.Ori() = Quaterniond::Rot(Rad(90), 'x');

		// Base Link
		PHSolidIf* so0 = phScene->CreateSolid(descSolid);
		so0->SetDynamical(false);
		so0->AddShape(phSdk->CreateShape(descCapsule));
		so0->SetShapePose(0, shapePose);

		// Link 1
		PHSolidIf* so1 = phScene->CreateSolid(descSolid);
		so1->SetFramePosition(Vec3d(0,4,0));
		so1->AddShape(phSdk->CreateShape(descCapsule));
		so1->SetShapePose(0, shapePose);

		// Link 2
		PHSolidIf* so2 = phScene->CreateSolid(descSolid);
		so2->SetFramePosition(Vec3d(0,8,0));
		so2->AddShape(phSdk->CreateShape(descCapsule));
		so2->SetShapePose(0, shapePose);

		// Pointer
		PHSolidIf* so4 = phScene->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(6,0,0));
		so4->AddShape(phSdk->CreateShape(descSphere));
		so4->SetDynamical(false);

		// Obstacle
		PHSolidIf* so5 = phScene->CreateSolid(descSolid);
		so5->SetFramePosition(Vec3d(4,0,0));
		so5->AddShape(phSdk->CreateShape(descBox));

		// Obstacle Base
		PHSolidIf* so6 = phScene->CreateSolid(descSolid);
		so6->SetFramePosition(Vec3d(4,-2,0));
		so6->AddShape(phSdk->CreateShape(descBox));
		so6->SetDynamical(false);

		soTarget = so4;

		// ----- ----- ----- ----- -----

		PHHingeJointDesc descJoint;
		descJoint.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descJoint.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descJoint.spring =      1.0;
		descJoint.damper =    100.0;
		descJoint.fMax   =   1000.0;

		PHIKHingeActuatorDesc descIKA;
		descIKA.velocityGain = 10.0;

		PHIKEndEffectorDesc descIKE;

		// Base <-> Link 1
		PHHingeJointIf* jo1 = phScene->CreateJoint(so0, so1, descJoint)->Cast();
		PHIKHingeActuatorIf* ika1 = phScene->CreateIKActuator(descIKA)->Cast();
		ika1->AddChildObject(jo1);

		// Link 1 <-> Link 2
		PHHingeJointIf* jo2  = phScene->CreateJoint(so1, so2, descJoint)->Cast();
		PHIKHingeActuatorIf* ika2 = phScene->CreateIKActuator(descIKA)->Cast();
		ika2->AddChildObject(jo2);

		// Link2 = End Effector
		descIKE.targetLocalPosition = Vec3d(0,2,0);
		PHIKEndEffectorIf* ike1 = phScene->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so2);
		ika1->AddChildObject(ike1);
		ika2->AddChildObject(ike1);
		ike1->SetTargetPosition(Vec3d(-5,0,0));

		ikeTarget = ike1;

		// Obstacle <-> Obstacle Base
		descJoint.poseSocket = Posed(1,0,0,0, 0,-1,0);
		descJoint.posePlug   = Posed(1,0,0,0, 0, 1,0);
		PHHingeJointIf* jo3  = phScene->CreateJoint(so5, so6, descJoint)->Cast();
		jo3->SetSpring(500.0);
		jo3->SetDamper(  5.0);

		// ----- ----- ----- ----- -----

		ike1->SetTargetPosition(soTarget->GetPose().Pos());
		phScene->GetIKEngine()->Enable(true);

		phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		phScene->SetContactMode(so2, so5, PHSceneDesc::MODE_LCP);
	}

	virtual void OnStep(){
		double s = 0;
		if (0<=time && time < 100) {
			s   =       ((double)(time % 100)) / 100.0;
		} else if (100 <= time && time < 200) {
			s   = 1.0 - ((double)(time % 100)) / 100.0;
		} else {
			s = 0;
			time = 0;
		}
		time++;
		double length = (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		ikeTarget->SetTargetPosition(length * (soTarget->GetPose().Pos() - Vec3d(0,0,0)));

		// ----- ----- ----- ----- -----
		if (bHard) {
			for (size_t i=0; i<2; ++i) {
				PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
				jo->SetSpring(  100.0);
				jo->SetDamper(10000.0);
			}
		} else {
			for (size_t i=0; i<2; ++i) {
				PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
				jo->SetSpring(  1.0);
				jo->SetDamper(100.0);
			}
		}

		// ----- ----- ----- ----- -----
		if (bVT) {
			StepVTT();
		} else {
			phScene->GetIKEngine()->Enable(true);
			for (size_t i=0; i<phScene->NJoints(); ++i) {
				PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
				if (jo) { jo->SetOffsetForce(0); }
			}
			phScene->Step();
		}
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

			if(id == ID_PLIANT){
				bVT   = true;
				bHard = false;
			}

			if(id == ID_SOFT){
				bVT   = false;
				bHard = false;
			}

			if(id == ID_STIFF){
				bVT   = false;
				bHard = true;
			}
		}

		SampleApp::OnAction(menu, id);
	}

	void StepVTT() {
		double offsets[2];
		double targets[2];
		double velocities[2];
		double springs[2];
		double dampers[2];

		states->SaveState(phScene);
		phScene->GetConstraintEngine()->EnableContactDetection(false);
		phScene->GetIKEngine()->Enable(true);
		for (size_t i=0; i<2; ++i) {
			PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
			springs[i]    = jo->GetSpring();
			dampers[i]    = jo->GetDamper();
			jo->SetSpring(0);
			jo->SetDamper(FLT_MAX);
		}

		phScene->Step();
		for (size_t i=0; i<2; ++i) {
			PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
			offsets[i] = jo->GetMotorForce();
			double limit = 200;
			if (offsets[i] < -limit) { offsets[i] = -limit; }
			if (limit  < offsets[i]) { offsets[i] =  limit; }

			targets[i]    = jo->GetTargetPosition();
			velocities[i] = jo->GetTargetVelocity();
		}

		states->LoadState(phScene);
		phScene->GetConstraintEngine()->EnableContactDetection(true);
		phScene->GetIKEngine()->Enable(false);

		for (size_t i=0; i<2; ++i) {
			PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
			jo->SetOffsetForce(offsets[i]);
			jo->SetSpring(springs[i]);
			jo->SetDamper(dampers[i]);
			jo->SetTargetPosition(targets[i]);
			jo->SetTargetVelocity(velocities[i]);
		}

		phScene->Step();
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
