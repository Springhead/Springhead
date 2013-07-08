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
アームの先端剛体が，ポインタ（球体）とリンクの根元の間を往復する．
- スペースキーでシミュレーション開始．
- 'v'で柔らかい追従制御．障害物で止められる程度の力だが，アーム先端が所定の高さで運動できる．
- 'b'で固いPD制御．アーム先端は所定の高さで運動できるが，障害物を強い力で押しのけてしまう．
- 'c'で柔らかいPD制御．障害物を押しのけるような強い力はないが，アーム先端を持ち上げる力もない．
（起動時は“柔らかいPD制御”になっている．）

- 'i'をタイプするとポインタ位置が上に動く．
- 'k'で、下へ
- 'j'で、左へ
- 'l'で、右へ
*/

#include <vector>
#include "../../SampleApp.h"

#include <Physics/PHIKActuator.h>
#include <Physics/PHIKEndEffector.h>

#include "windows.h"

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
		ID_TRAJ,
	};

	int argc;
	char** argv;

	bool bPM, bHard;
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

		AddAction(MENU_SCENE, ID_TRAJ, "Toggle Trajectory");
		AddHotKey(MENU_SCENE, ID_TRAJ, 'x');

		bPM   = false;
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

		GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
		GetCurrentWin()->GetTrackball()->SetPosition(Vec3d(0,0,50));

		GetFWScene()->EnableRenderGrid(false,true,false);
		GetFWScene()->EnableRenderIK();
	}

	PHSolidIf*         soTarget;
	PHIKEndEffectorIf* ikeTarget;
	PHJointIf*         joX;

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
		descBox.boxsize = Vec3d(1.0, 1.0, 1.0);

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
		so4->SetFramePosition(Vec3d(6,0,0));
		so4->AddShape(phSdk->CreateShape(descSphere));
		so4->SetDynamical(false);

		// Obstacle
		PHSolidIf* so5 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so5->SetFramePosition(Vec3d(4,0,0));
		so5->AddShape(phSdk->CreateShape(descBox));

		// Obstacle Base
		PHSolidIf* so6 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so6->SetFramePosition(Vec3d(4,-2,0));
		so6->AddShape(phSdk->CreateShape(descBox));
		so6->SetDynamical(false);

		soTarget = so4;

		// ----- ----- ----- ----- -----

		PHHingeJointDesc descJoint;
		descJoint.poseSocket = Posed(1,0,0,0, 0, 2,0);
		descJoint.posePlug   = Posed(1,0,0,0, 0,-2,0);
		descJoint.spring =   1000.0;
		descJoint.damper =    100.0;
		descJoint.cyclic =     true;
		// descJoint.fMax   =   2000.0;

		PHIKHingeActuatorDesc descIKA;
		PHIKEndEffectorDesc   descIKE;

		// Base <-> Link 1
		PHHingeJointIf* jo1 = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descJoint)->Cast();
		PHIKHingeActuatorIf* ika1 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika1->AddChildObject(jo1);

		// Link 1 <-> Link 2
		PHHingeJointIf* jo2  = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descJoint)->Cast();
		PHIKHingeActuatorIf* ika2 = GetFWScene()->GetPHScene()->CreateIKActuator(descIKA)->Cast();
		ika2->AddChildObject(jo2);
		joX = jo2;

		ika1->AddChildObject(ika2);

		// Link2 = End Effector
		descIKE.targetLocalPosition = Vec3d(0,2,0);
		PHIKEndEffectorIf* ike1 = GetFWScene()->GetPHScene()->CreateIKEndEffector(descIKE);
		ike1->AddChildObject(so2);
		ika2->AddChildObject(ike1);
		ike1->SetTargetPosition(Vec3d(-5,0,0));

		ikeTarget = ike1;

		// Obstacle <-> Obstacle Base
		descJoint.poseSocket = Posed(1,0,0,0, 0,-1.2,0);
		descJoint.posePlug   = Posed(1,0,0,0, 0, 1.2,0);
		PHHingeJointIf* jo3  = GetFWScene()->GetPHScene()->CreateJoint(so5, so6, descJoint)->Cast();
		jo3->SetSpring(200.0);
		jo3->SetDamper( 20.0);

		// ----- ----- ----- ----- -----

		ike1->SetTargetPosition(soTarget->GetPose().Pos());
		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxVelocity(50);
		GetFWScene()->GetPHScene()->GetIKEngine()->SetMaxAngularVelocity(Rad(1000));
		GetFWScene()->GetPHScene()->GetIKEngine()->EnableTrajectoryControl(false);

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
		// GetFWScene()->GetPHScene()->SetContactMode(so2, so5, PHSceneDesc::MODE_LCP);

		GetFWScene()->GetPHScene()->SetNumIteration(50);
	}

	virtual void OnStep(){
		double s = 0;
		int  sgn = 1;
		if (0<=time && time < 100) {
			s   =       ((double)(time % 100)) / 100.0;
		} else if (100 <= time && time < 200) {
			s   = 1.0 - ((double)(time % 100)) / 100.0;
			sgn = -1;
		} else {
			s = 0;
			time = 0;
		}

		time++;
		double  length = (10*pow(s,3) - 15*pow(s,4) +  6*pow(s,5));
		double dLength = (30*pow(s,2) - 60*pow(s,3) + 30*pow(s,4));
		Vec3d      dir = soTarget->GetPose().Pos() - Vec3d(0,0,0);
		double      dt = GetFWScene()->GetPHScene()->GetTimeStep();
		ikeTarget->SetTargetPosition(dir *  length);
		ikeTarget->SetTargetVelocity(dir * dLength * sgn / (100*dt));

		/*
		// <!!>
		static Vec3d lastTP = Vec3d();
		Vec3d currTP = dir *  length;
		Vec3d vel = (currTP - lastTP) / dt;
		ikeTarget->SetTargetVelocity(currTP - lastTP);
		lastTP = currTP;

		// <!!>
		PTM::VVector<double> tempTarg; DCAST(PHIKEndEffector,ikeTarget)->GetTempTarget(tempTarg);
		Vec3d targDR  = Vec3d(tempTarg[0], tempTarg[1], tempTarg[2]);
		Vec3d currPos = DCAST(PHIKEndEffector,ikeTarget)->solidTempPose*DCAST(PHIKEndEffector,ikeTarget)->targetLocalPosition;
		*/

		// ----- ----- ----- ----- -----
		if (bHard) {
			for (size_t i=0; i<2; ++i) {
				PHHingeJointIf* jo = GetFWScene()->GetPHScene()->GetJoint(i)->Cast();
				jo->SetSpring(5000.0);
				jo->SetDamper( 500.0);
			}
		} else {
			for (size_t i=0; i<2; ++i) {
				PHHingeJointIf* jo = GetFWScene()->GetPHScene()->GetJoint(i)->Cast();
				jo->SetSpring(  20.0);
				jo->SetDamper(   0.0);
			}
		}

		// ----- ----- ----- ----- -----
		if (bPM) {
			StepPliant();
		} else {
			GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);
			for (size_t i=0; i<GetFWScene()->GetPHScene()->NJoints(); ++i) {
				PHHingeJointIf* jo = GetFWScene()->GetPHScene()->GetJoint(i)->Cast();
				if (jo) { jo->SetOffsetForce(0); }
			}
			GetFWScene()->GetPHScene()->Step();
		}

		// ----- ----- ----- ----- -----
		{
			static int cycle = 0;
			static DWORD lastCounted = timeGetTime();
			DWORD now = timeGetTime();
			if (now - lastCounted > 1000) {
				float cps = (float)(cycle) / (float)(now - lastCounted) * 1000.0f;
				lastCounted = now;
				cycle = 0;
				std::cout << "CPS : " << cps << std::endl;
			}
			cycle++;
		}

		/*
		// <!!>
		Vec3d newPos = DCAST(PHIKEndEffector,ikeTarget)->solidTempPose*DCAST(PHIKEndEffector,ikeTarget)->targetLocalPosition;
		Vec3d resultDR = newPos - currPos;
		
		std::cout << targDR << " <=> " << resultDR << "  | " << (targDR-resultDR) << std::endl;
		*/
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
				bPM   = true;
				bHard = false;
			}

			if(id == ID_SOFT){
				bPM   = false;
				bHard = false;
			}

			if(id == ID_STIFF){
				bPM   = false;
				bHard = true;
			}

			if(id == ID_TRAJ){
				bool bTraj = GetFWScene()->GetPHScene()->GetIKEngine()->IsTrajectoryControlEnabled();
				bTraj = !bTraj;
				GetFWScene()->GetPHScene()->GetIKEngine()->EnableTrajectoryControl(bTraj);
				std::cout << "bTraj : " << (bTraj ? "On" : "Off") << std::endl;
			}
		}

		SampleApp::OnAction(menu, id);
	}

	void StepPliant() {
		double offsets[2];
		double targets[2];
		double velocities[2];
		double springs[2];
		double dampers[2];
		PHIKActuatorState    stA[2];
		PHIKEndEffectorState stE;

		states->SaveState(GetFWScene()->GetPHScene());
		GetFWScene()->GetPHScene()->GetConstraintEngine()->EnableContactDetection(false);
		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(true);
		for (size_t i=0; i<2; ++i) {
			PHHingeJointIf* jo = GetFWScene()->GetPHScene()->GetJoint(i)->Cast();
			springs[i]    = jo->GetSpring();
			dampers[i]    = jo->GetDamper();
			jo->SetSpring(0);
			jo->SetDamper(FLT_MAX);
			jo->SetOffsetForce(0);
		}

		GetFWScene()->GetPHScene()->Step();
		for (size_t i=0; i<2; ++i) {
			PHHingeJointIf* jo = GetFWScene()->GetPHScene()->GetJoint(i)->Cast();
			offsets[i]    = jo->GetMotorForce();
			targets[i]    = jo->GetTargetPosition();
			velocities[i] = jo->GetTargetVelocity();
			
			PHIKHingeActuatorIf* ika = GetFWScene()->GetPHScene()->GetIKActuator(i)->Cast();
			ika->GetState(&(stA[i]));
		}
		ikeTarget->GetState(&stE);

		states->LoadState(GetFWScene()->GetPHScene());

		// GetFWScene()->GetPHScene()->GetConstraintEngine()->EnableContactDetection(true);
		GetFWScene()->GetPHScene()->GetIKEngine()->Enable(false);


		for (size_t i=0; i<2; ++i) {
			PHHingeJointIf* jo = GetFWScene()->GetPHScene()->GetJoint(i)->Cast();
			jo->SetOffsetForce(offsets[i]);
			jo->SetSpring(springs[i]);
			jo->SetDamper(dampers[i]);
			jo->SetTargetPosition(targets[i]);
			jo->SetTargetVelocity(velocities[i]);

			PHIKHingeActuatorIf* ika = GetFWScene()->GetPHScene()->GetIKActuator(i)->Cast();
			ika->SetState(&(stA[i]));
		}
		ikeTarget->SetState(&stE);

		GetFWScene()->GetPHScene()->Step();
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
