/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	PHSpringDesc springDesc;
	{
		springDesc.spring    = Vec3d(1,1,1) * 500.0;
		springDesc.damper    = Vec3d(1,1,1) *  50.0;
		springDesc.springOri = 500.0;
		springDesc.damperOri =  50.0;
	}
	joFixpoint = phScene->CreateJoint(soFixpoint, soWaist, springDesc);
}
	
void CREseWalkingController::Step(){
	CRController::Step();

	Vec3d p = soFixpoint->GetPose().Pos();
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
