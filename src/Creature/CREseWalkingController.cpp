/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CREseWalkingController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CREseWalkingController, CRController);

void CREseWalkingController::Init(){
	CRController::Init();

	for (int i=0; i<creature->NBodies(); ++i) {
		CRHingeHumanBodyIf* body = DCAST(CRHingeHumanBodyIf, creature->GetBody(i));
		if (body) {
			soWaist = body->GetSolid(CRHingeHumanBodyDesc::SO_WAIST);
		}
	}

	// ƒ{ƒfƒB‚Ì‹óŠÔŒÅ’è
	PHSolidDesc solidDesc;
	soFixpoint = phScene->CreateSolid(solidDesc);
	soFixpoint->SetDynamical(false);
	/*
	CDBoxDesc descBox;
	descBox.boxsize = Vec3f(0.5,1.7,0.2);
	soFixpoint->AddShape(phScene->GetSdk()->CreateShape(descBox));
	*/
	PHBallJointDesc ballJointDesc;
	//ballJointDesc.origin = Quaternionf::Rot(Rad(0),'y');
	ballJointDesc.goal.SwingDir() = 1;
	ballJointDesc.spring.item(2) = 5000.0;
	ballJointDesc.damper.item(2) =  100.0;
	joFixpoint = phScene->CreateJoint(soFixpoint, soWaist, ballJointDesc);

	// phScene->SetContactMode(soFixpoint, PHSceneDesc::MODE_NONE);
}
	
void CREseWalkingController::Step(){
	// phScene->SetContactMode(soFixpoint, PHSceneDesc::MODE_NONE);

	CRController::Step();

	Vec3d p = soFixpoint->GetPose().Pos();
	// p.Y() = 0.0;
	soFixpoint->SetFramePosition(p);
	soFixpoint->SetVelocity(soWaist->GetPose().Ori() * Vec3d(0,0,-speed));
	soFixpoint->SetAngularVelocity(Vec3d(0,rotation,0));
}

void CREseWalkingController::SetSpeed(float speed){
	this->speed = speed;
}

void CREseWalkingController::SetRotationAngle(float rot){
	this->rotation = rot;
}

void CREseWalkingController::SetPos(Vec3f pos){
	soFixpoint->SetFramePosition(pos);
}
}
