/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "../../SampleApp.h"

class MyApp : public SampleApp{
public:
	enum{
		ID_LEFT,
		ID_RIGHT,
		ID_XDIR,
		ID_YDIR,
		ID_ZDIR,
		ID_INC,
		ID_DEC,
	};

	PHSolidIf *hip, *body, *chest, *head;
	PHSolidIf *upArmL, *loArmL;
	PHSolidIf *upArmR, *loArmR;
	PHSolidIf *targetL, *targetR;

	CDShapeIf *shapeUpArm, *shapeLoArm, *shapeTarget;

	CDBoxDesc			bd;
	CDSphereDesc		sd;
	PHHingeJointDesc	jd;
	PHBallJointDesc		bjd;
	PHSpringDesc		spd;

	Vec3d		targetPosL, targetPosR;
	bool		lr;
	int			axis;
	double		div;

public:
	MyApp(){
		AddAction(MENU_SCENE, ID_LEFT, "select left target");
		AddHotKey(MENU_SCENE, ID_LEFT, 'l');
		AddAction(MENU_SCENE, ID_RIGHT, "select right target");
		AddHotKey(MENU_SCENE, ID_RIGHT, 'r');
		AddAction(MENU_SCENE, ID_XDIR, "move target along x-axis");
		AddHotKey(MENU_SCENE, ID_XDIR, 'x');
		AddAction(MENU_SCENE, ID_YDIR, "move target along y-axis");
		AddHotKey(MENU_SCENE, ID_YDIR, 'y');
		AddAction(MENU_SCENE, ID_ZDIR, "move target along z-axis");
		AddHotKey(MENU_SCENE, ID_ZDIR, 'z');
		AddAction(MENU_SCENE, ID_INC, "move target in positive direction");
		AddHotKey(MENU_SCENE, ID_INC, 'm');
		AddAction(MENU_SCENE, ID_DEC, "move target in negative direction");
		AddHotKey(MENU_SCENE, ID_DEC, 'n');

		targetPosL = Vec3d(-0.2, 0.2, -0.2);
		targetPosR = Vec3d( 0.2, 0.2, -0.2);
		lr		= true;
		axis	= 0;
		div		= 0.01;
	}

	virtual void BuildScene(){
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();

		// hip
		bd.boxsize = Vec3f(0.19, 0.077, 0.137);
		hip = GetFWScene()->GetPHScene()->CreateSolid();
		hip->AddShape(phSdk->CreateShape(bd));
		hip->SetFramePosition(Vec3f(0.0, 0.0, 0.0));
		hip->SetDynamical(false);
		// body
		bd.boxsize = Vec3f(0.15, 0.08, 0.108);
		body = GetFWScene()->GetPHScene()->CreateSolid();
		body->AddShape(phSdk->CreateShape(bd));
		// chest
		bd.boxsize = Vec3f(0.18, 0.2, 0.1);
		chest = GetFWScene()->GetPHScene()->CreateSolid();
		chest->AddShape(phSdk->CreateShape(bd));
		// body joints
		jd.damper = 60.0;
		jd.spring = 160.0;
		jd.posePlug.Pos() = Vec3d(0,0.04, 0);
		jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'x');
		jd.poseSocket.Pos() = Vec3d(0, -0.04, 0);
		jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
		GetFWScene()->GetPHScene()->CreateJoint(body, hip, jd);
		//
		jd.posePlug.Pos() = Vec3d(0, 0.04, 0);
		jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.poseSocket.Pos() = Vec3d(0, -0.1, 0);
		jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
		GetFWScene()->GetPHScene()->CreateJoint(chest, body, jd);

		// head
		sd.radius = 0.06;
		head = GetFWScene()->GetPHScene()->CreateSolid();
		head->AddShape(phSdk->CreateShape(sd));
		// neck
		jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.posePlug.Pos() = Vec3d(0, 0.1, 0);
		jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.poseSocket.Pos() = Vec3d(0, -0.08, 0);
		jd.damper = 1.0;
		jd.spring = 100.0;
		jd.targetPosition = Rad(0);
		GetFWScene()->GetPHScene()->CreateJoint(head, chest, jd);
		
		// upper arm
		bd.boxsize = Vec3f(0.049, 0.16, 0.048);
		shapeUpArm = phSdk->CreateShape(bd);
		upArmR = GetFWScene()->GetPHScene()->CreateSolid();
		upArmR->AddShape(shapeUpArm);
		upArmL = GetFWScene()->GetPHScene()->CreateSolid();
		upArmL->AddShape(shapeUpArm);
		// lower arm
		bd.boxsize = Vec3f(0.046, 0.12, 0.046);
		shapeLoArm = phSdk->CreateShape(bd);
		loArmR = GetFWScene()->GetPHScene()->CreateSolid();
		loArmR->AddShape(shapeLoArm);
		loArmR->SetOrientation(Quaterniond::Rot(Rad(90), 'x'));
		loArmL = GetFWScene()->GetPHScene()->CreateSolid();
		loArmL->AddShape(shapeLoArm);
		loArmL->SetOrientation(Quaterniond::Rot(Rad(90), 'x'));
		// shoulder
		bjd.posePlug.Pos() = Vec3d(0.13, 0.1, 0.0);
		bjd.poseSocket.Pos() = Vec3d(0, 0.1, 0);
		bjd.damper = 6.0;
		bjd.spring = 10.0;
		(Vec3d&)bjd.targetPosition = Vec3d(Rad(-30), Rad(30), Rad(-30));
		GetFWScene()->GetPHScene()->CreateJoint(upArmR, chest, bjd);
		
		bjd.posePlug.Pos() = Vec3d(-0.13, 0.1, 0.0);
		bjd.poseSocket.Pos() = Vec3d(0, 0.1, 0);
		(Vec3d&)bjd.targetPosition = Vec3d(Rad(30), Rad(30), Rad(30));
		GetFWScene()->GetPHScene()->CreateJoint(upArmL, chest, bjd);
		// elbow
		jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.posePlug.Pos() = Vec3d(0, -0.09, 0);
		jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.poseSocket.Pos() = Vec3d(0, 0.07, 0);
		jd.damper = 4.0;
		jd.spring = 6.0;
		jd.targetPosition = Rad(-30);
		GetFWScene()->GetPHScene()->CreateJoint(loArmR, upArmR, jd);
		
		jd.posePlug.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.posePlug.Pos() = Vec3d(0, -0.09, 0);
		jd.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
		jd.poseSocket.Pos() = Vec3d(0, 0.07, 0);
		jd.targetPosition = Rad(-30);
		GetFWScene()->GetPHScene()->CreateJoint(loArmL, upArmL, jd);

		// reaching targets
		sd.radius = 0.02;
		shapeTarget = phSdk->CreateShape(sd);
		targetR = GetFWScene()->GetPHScene()->CreateSolid();
		targetR->AddShape(shapeTarget);
		targetR->SetDynamical(false);
		targetR->SetFramePosition(targetPosR);
	
		targetL = GetFWScene()->GetPHScene()->CreateSolid();
		targetL->AddShape(shapeTarget);
		targetL->SetDynamical(false);
		targetL->SetFramePosition(targetPosL);
		
		//	バネ
		spd.damper = Vec3f(1,1,1) * 1000;
		spd.spring = Vec3f(1,1,1) * 3000;
		spd.posePlug.Pos() = Vec3d(0, -0.09, 0);
		GetFWScene()->GetPHScene()->CreateJoint(targetR, loArmR, spd);
		GetFWScene()->GetPHScene()->CreateJoint(targetL, loArmL, spd);

		// 重力を設定
		GetFWScene()->GetPHScene()->SetGravity(Vec3f(0, -9.8, 0));

		//
		GetFWScene()->SetRenderMode(true, false);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, head);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, chest);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, body);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, hip);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, upArmL);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, loArmL);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, upArmR);
		GetFWScene()->SetSolidMaterial(GRRenderIf::BLUE, loArmR);
		
		GetFWScene()->SetSolidMaterial(GRRenderIf::RED, targetL);
		GetFWScene()->SetSolidMaterial(GRRenderIf::RED, targetR);
		GetFWScene()->EnableRenderAxis(false, false, false);

		//	接触を切る
		//scene->SetContactMode(&soTarget[0], PHSceneDesc::MODE_NONE);
		//scene->SetContactMode(&soTarget[1], PHSceneDesc::MODE_NONE);
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE){
			if(id == ID_LEFT){
				lr = true;
				message = "left target selected.";
			}
			if(id == ID_RIGHT){
				lr = false;
				message = "right target selected.";
			}
			if(id == ID_XDIR){
				axis = 0;
				message = "x-axis selected.";
			}
			if(id == ID_YDIR){
				axis = 1;
				message = "y-axis selected.";
			}
			if(id == ID_ZDIR){
				axis = 2;
				message = "z-axis selected.";
			}
			if(id == ID_INC){
				if(lr){
					targetPosL[axis] += div;
					targetL->SetFramePosition(targetPosL);
				}
				else{
					targetPosR[axis] += div;
					targetR->SetFramePosition(targetPosR);
				}
				message = "target moved.";
			}
			if(id == ID_DEC){
				if(lr){
					targetPosL[axis] -= div;
					targetL->SetFramePosition(targetPosL);
				}
				else{
					targetPosR[axis] -= div;
					targetR->SetFramePosition(targetPosR);
				}
				message = "target moved.";
			}
		}
	}

} app;

int __cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
