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
		ID_FORE,
		ID_BACK,
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

		AddAction(MENU_SCENE, ID_FORE, "Target move Forward");
		AddHotKey(MENU_SCENE, ID_FORE, 'u');

		AddAction(MENU_SCENE, ID_BACK, "Target move Backward");
		AddHotKey(MENU_SCENE, ID_BACK, 'o');
	}
	~IKSampleApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		ToggleAction(MENU_ALWAYS, ID_DRAW_WIREFRAME);
		curScene = 0;

		GetFWScene()->EnableRenderIK();

		GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
		GetCurrentWin()->GetTrackball()->SetPosition(Vec3f(6.5,6,20));

		GetCurrentWin()->GetTrackball()->SetPosition(Vec3d(0,0,50));
	}

	PHSolidIf*         soTarget1;
	PHSolidIf*         soTarget2;
	PHIKEndEffectorIf* ikeTarget1;
	PHIKEndEffectorIf* ikeTarget2;

	// シーン構築
	virtual void BuildScene() {
		// BuildSceneBallBranch();
		BuildSceneHingeBranch();
	}

	virtual void BuildSceneHingeBranch() {
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

		// Link 3
		PHSolidIf* so3 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so3->SetFramePosition(Vec3d(0,12,0));
		so3->AddShape(phSdk->CreateShape(descCapsule));
		so3->SetShapePose(0, shapePose);

		// Link 4
		PHSolidIf* so4 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(0,16,0));
		so4->AddShape(phSdk->CreateShape(descCapsule));
		so4->SetShapePose(0, shapePose);

		// Link 5
		PHSolidIf* so5 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so5->SetFramePosition(Vec3d(0,12,0));
		so5->AddShape(phSdk->CreateShape(descCapsule));
		so5->SetShapePose(0, shapePose);

		// Link 6
		PHSolidIf* so6 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so6->SetFramePosition(Vec3d(0,16,0));
		so6->AddShape(phSdk->CreateShape(descCapsule));
		so6->SetShapePose(0, shapePose);

		// Pointer 1
		PHSolidIf* soP1 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		soP1->SetFramePosition(Vec3d(  5,3,0));
		soP1->AddShape(phSdk->CreateShape(descSphere));
		soP1->SetDynamical(false);
		soTarget1 = soP1;

		// Pointer 2
		PHSolidIf* soP2 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		soP2->SetFramePosition(Vec3d( -5,3,0));
		soP2->AddShape(phSdk->CreateShape(descSphere));
		soP2->SetDynamical(false);
		soTarget2 = soP2;

		// ----- ----- ----- ----- -----

		PHHingeJointDesc descHinge;
		descHinge.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descHinge.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descHinge.spring     =  10000.0;
		descHinge.damper     =   1000.0;
		// descHinge.fMax    =   1000.0;
		descHinge.cyclic     =     true;

		PHBallJointDesc descBall;
		descBall.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descBall.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descBall.spring     =  10000.0;
		descBall.damper     =   1000.0;
		// descBall.fMax    =   1000.0;

		PHIKHingeActuatorDesc descIKHinge;
		PHIKBallActuatorDesc  descIKBall;

		PHIKEndEffectorDesc   descIKE;

		// Base <-> Link 1
		PHHingeJointIf* jo1 = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descHinge)->Cast();
		PHIKHingeActuatorIf* ika1 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKHinge)->Cast();
		ika1->AddChildObject(jo1);

		// Link 1 <-> Link 2
		PHHingeJointIf* jo2  = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descHinge)->Cast();
		PHIKHingeActuatorIf* ika2 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKHinge)->Cast();
		ika2->AddChildObject(jo2);

		// Link 2 <-> Link 3
		PHHingeJointIf* jo3  = GetFWScene()->GetPHScene()->CreateJoint(so2, so3, descHinge)->Cast();
		PHIKHingeActuatorIf* ika3 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKHinge)->Cast();
		ika3->AddChildObject(jo3);

		// Link 3 <-> Link 4
		PHHingeJointIf* jo4  = GetFWScene()->GetPHScene()->CreateJoint(so3, so4, descHinge)->Cast();
		PHIKHingeActuatorIf* ika4 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKHinge)->Cast();
		ika4->AddChildObject(jo4);

		// Link 2 <-> Link 5
		PHHingeJointIf* jo5  = GetFWScene()->GetPHScene()->CreateJoint(so2, so5, descHinge)->Cast();
		PHIKHingeActuatorIf* ika5 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKHinge)->Cast();
		ika5->AddChildObject(jo5);

		// Link 5 <-> Link 6
		PHHingeJointIf* jo6  = GetFWScene()->GetPHScene()->CreateJoint(so5, so6, descHinge)->Cast();
		PHIKHingeActuatorIf* ika6 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKHinge)->Cast();
		ika6->AddChildObject(jo6);


		ika1->AddChildObject(ika2);
		ika2->AddChildObject(ika3);
		ika3->AddChildObject(ika4);
		ika2->AddChildObject(ika5);
		ika5->AddChildObject(ika6);


		// Link4 = End Effector
		PHIKEndEffectorIf* ike1 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so4);
		ika4->AddChildObject(ike1);
		ike1->SetTargetLocalPosition(Vec3d(0,2,0));

		ike1->EnablePositionControl(true);
		ike1->EnableOrientationControl(false);

		// Link6 = End Effector
		PHIKEndEffectorIf* ike2 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so6);
		ika6->AddChildObject(ike2);
		ike2->SetTargetLocalPosition(Vec3d(0,2,0));

		ike2->EnablePositionControl(true);
		ike2->EnableOrientationControl(false);

		ikeTarget1 = ike1;
		ikeTarget2 = ike2;

		// ----- ----- ----- ----- -----

		ike1->SetTargetPosition(soTarget1->GetPose().Pos());
		ike2->SetTargetPosition(soTarget2->GetPose().Pos());

		double pbr = 1.0;
		ika1->SetPullbackRate(pbr);
		ika2->SetPullbackRate(pbr);
		ika3->SetPullbackRate(pbr);
		ika4->SetPullbackRate(pbr);
		ika5->SetPullbackRate(pbr);
		ika6->SetPullbackRate(pbr);

		ika1->SetBias( 8.0);
		ika2->SetBias( 4.0);
		ika3->SetBias( 2.0);
		ika4->SetBias( 1.0);
		ika5->SetBias( 2.0);
		ika6->SetBias( 1.0);

		ike1->SetPositionPriority(1.0);
		ike2->SetPositionPriority(0.2);

		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxVelocity(20);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxAngularVelocity(Rad(500));

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
	}

	virtual void BuildSceneBallBranch() {
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

		// Link 3
		PHSolidIf* so3 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so3->SetFramePosition(Vec3d(0,12,0));
		so3->AddShape(phSdk->CreateShape(descCapsule));
		so3->SetShapePose(0, shapePose);

		// Link 4
		PHSolidIf* so4 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(0,16,0));
		so4->AddShape(phSdk->CreateShape(descCapsule));
		so4->SetShapePose(0, shapePose);

		// Link 5
		PHSolidIf* so5 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so5->SetFramePosition(Vec3d(0,12,0));
		so5->AddShape(phSdk->CreateShape(descCapsule));
		so5->SetShapePose(0, shapePose);

		// Link 6
		PHSolidIf* so6 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so6->SetFramePosition(Vec3d(0,16,0));
		so6->AddShape(phSdk->CreateShape(descCapsule));
		so6->SetShapePose(0, shapePose);

		// Pointer 1
		PHSolidIf* soP1 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		soP1->SetFramePosition(Vec3d(  5,3,0));
		soP1->AddShape(phSdk->CreateShape(descSphere));
		soP1->SetDynamical(false);
		soTarget1 = soP1;

		// Pointer 2
		PHSolidIf* soP2 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		soP2->SetFramePosition(Vec3d( -5,3,0));
		soP2->AddShape(phSdk->CreateShape(descSphere));
		soP2->SetDynamical(false);
		soTarget2 = soP2;

		// ----- ----- ----- ----- -----

		PHHingeJointDesc descHinge;
		descHinge.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descHinge.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descHinge.spring     =  10000.0;
		descHinge.damper     =   1000.0;
		// descHinge.fMax    =   1000.0;
		descHinge.cyclic     =     true;

		PHBallJointDesc descBall;
		descBall.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descBall.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descBall.spring     =  10000.0;
		descBall.damper     =   1000.0;
		// descBall.fMax    =   1000.0;

		PHIKHingeActuatorDesc descIKHinge;
		PHIKBallActuatorDesc  descIKBall;

		PHIKEndEffectorDesc   descIKE;

		// Base <-> Link 1
		PHBallJointIf* jo1 = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descBall)->Cast();
		PHIKBallActuatorIf* ika1 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKBall)->Cast();
		ika1->AddChildObject(jo1);

		// Link 1 <-> Link 2
		PHBallJointIf* jo2  = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descBall)->Cast();
		PHIKBallActuatorIf* ika2 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKBall)->Cast();
		ika2->AddChildObject(jo2);

		// Link 2 <-> Link 3
		PHBallJointIf* jo3  = GetFWScene()->GetPHScene()->CreateJoint(so2, so3, descBall)->Cast();
		PHIKBallActuatorIf* ika3 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKBall)->Cast();
		ika3->AddChildObject(jo3);

		// Link 3 <-> Link 4
		PHBallJointIf* jo4  = GetFWScene()->GetPHScene()->CreateJoint(so3, so4, descBall)->Cast();
		PHIKBallActuatorIf* ika4 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKBall)->Cast();
		ika4->AddChildObject(jo4);

		// Link 2 <-> Link 5
		PHBallJointIf* jo5  = GetFWScene()->GetPHScene()->CreateJoint(so2, so5, descBall)->Cast();
		PHIKBallActuatorIf* ika5 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKBall)->Cast();
		ika5->AddChildObject(jo5);

		// Link 5 <-> Link 6
		PHBallJointIf* jo6  = GetFWScene()->GetPHScene()->CreateJoint(so5, so6, descBall)->Cast();
		PHIKBallActuatorIf* ika6 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKBall)->Cast();
		ika6->AddChildObject(jo6);


		ika1->AddChildObject(ika2);
		ika2->AddChildObject(ika3);
		ika3->AddChildObject(ika4);
		ika2->AddChildObject(ika5);
		ika5->AddChildObject(ika6);


		// Link4 = End Effector
		PHIKEndEffectorIf* ike1 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so4);
		ika4->AddChildObject(ike1);
		ike1->SetTargetLocalPosition(Vec3d(0,2,0));

		ike1->EnablePositionControl(true);
		ike1->EnableOrientationControl(false);

		// Link6 = End Effector
		PHIKEndEffectorIf* ike2 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so6);
		ika6->AddChildObject(ike2);
		ike2->SetTargetLocalPosition(Vec3d(0,2,0));

		ike2->EnablePositionControl(true);
		ike2->EnableOrientationControl(false);

		ikeTarget1 = ike1;
		ikeTarget2 = ike2;

		// ----- ----- ----- ----- -----

		ike1->SetTargetPosition(soTarget1->GetPose().Pos());
		ike2->SetTargetPosition(soTarget2->GetPose().Pos());

		double pbr = 1.0;
		ika1->SetPullbackRate(pbr);
		ika2->SetPullbackRate(pbr);
		ika3->SetPullbackRate(pbr);
		ika4->SetPullbackRate(pbr);
		ika5->SetPullbackRate(pbr);
		ika6->SetPullbackRate(pbr);

		ika1->SetBias( 8.0);
		ika2->SetBias( 4.0);
		ika3->SetBias( 2.0);
		ika4->SetBias( 1.0);
		ika5->SetBias( 2.0);
		ika6->SetBias( 1.0);

		ike1->SetPositionPriority(1.0);
		ike2->SetPositionPriority(0.05);

		ike1->SetOrientationPriority(1.0);

		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxVelocity(20);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxAngularVelocity(Rad(1000));
		GetFWScene()->GetPHScene()->GetIKEngine()->SetRegularizeParam(0.3);

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE){
			if(id == ID_UP){
				Vec3d currPos = soTarget1->GetPose().Pos();
				soTarget1->SetFramePosition(currPos + Vec3d( 0.0,  1.0,  0.0));
				ikeTarget1->SetTargetPosition(soTarget1->GetPose().Pos());
				ikeTarget1->SetTargetOrientation(soTarget1->GetPose().Ori());
			}

			if(id == ID_DOWN){
				Vec3d currPos = soTarget1->GetPose().Pos();
				soTarget1->SetFramePosition(currPos + Vec3d( 0.0, -1.0,  0.0));
				ikeTarget1->SetTargetPosition(soTarget1->GetPose().Pos());
				ikeTarget1->SetTargetOrientation(soTarget1->GetPose().Ori());
			}

			if(id == ID_LEFT){
				Vec3d currPos = soTarget1->GetPose().Pos();
				soTarget1->SetFramePosition(currPos + Vec3d(-1.0,  0.0,  0.0));
				ikeTarget1->SetTargetPosition(soTarget1->GetPose().Pos());
				ikeTarget1->SetTargetOrientation(soTarget1->GetPose().Ori());
			}

			if(id == ID_RIGHT){
				Vec3d currPos = soTarget1->GetPose().Pos();
				soTarget1->SetFramePosition(currPos + Vec3d( 1.0,  0.0,  0.0));
				ikeTarget1->SetTargetPosition(soTarget1->GetPose().Pos());
				ikeTarget1->SetTargetOrientation(soTarget1->GetPose().Ori());
			}

			if(id == ID_FORE){
				Vec3d currPos = soTarget1->GetPose().Pos();
				soTarget1->SetFramePosition(currPos + Vec3d( 0.0,  0.0,  1.0));
				ikeTarget1->SetTargetPosition(soTarget1->GetPose().Pos());
				ikeTarget1->SetTargetOrientation(soTarget1->GetPose().Ori());
			}

			if(id == ID_BACK){
				Vec3d currPos = soTarget1->GetPose().Pos();
				soTarget1->SetFramePosition(currPos + Vec3d( 0.0,  0.0, -1.0));
				ikeTarget1->SetTargetPosition(soTarget1->GetPose().Pos());
				ikeTarget1->SetTargetOrientation(soTarget1->GetPose().Ori());
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
