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

	soWaist = DCAST(CRHingeHumanBodyIf, creature->GetBody())->GetSolid(CRHingeHumanBodyDesc::SO_WAIST);

	// ƒ{ƒfƒB‚Ì‹óŠÔŒÅ’è
	PHSolidDesc solidDesc;
	soFixpoint = phScene->CreateSolid(solidDesc);
	soFixpoint->SetDynamical(false);
	PHBallJointDesc ballJointDesc;
	ballJointDesc.origin = Quaternionf::Rot(Rad(0),'y');
	ballJointDesc.spring = 1000.0;
	ballJointDesc.damper =  100.0;
	joFixpoint = phScene->CreateJoint(soFixpoint, soWaist, ballJointDesc);
}
	
void CREseWalkingController::Step(){
	CRController::Step();

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
