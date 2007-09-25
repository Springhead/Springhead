/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRTravelController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRTravelController, CRController);

void CRTravelController::Init(){
	CRController::Init();

	internalScene = creature->GetInternalScene();

	for (int i=0; i<creature->NBodies(); ++i) {
		CRHingeHumanBodyIf* body = DCAST(CRHingeHumanBodyIf, creature->GetBody(i));
		if (body) {
			soWaist = body->GetSolid(CRHingeHumanBodyDesc::SO_WAIST);
		}
	}

	for (int i=0; i<creature->NControllers(); i++) {
		if (!walkCtrl) {
			walkCtrl = DCAST(CREseWalkingControllerIf, creature->GetController(i));
		}
	}
}

void CRTravelController::Step(){
	CRController::Step();

	Vec2f potential = Vec2f(0,0);

	for (int i=0; i<internalScene->NObjects(); i++) {
		CRISTravelPotentialObjectIf* obj = DCAST(CRISTravelPotentialObjectIf, internalScene->GetISObject(i));
		if (obj) {
			Vec3f myPos = soWaist->GetPose().Pos();
			potential += obj->GetPotential(Vec2f(myPos.X(), myPos.Z()));
		}
	}

	if (potential.norm() > 0) {
		Vec2f dir = potential.unit();

		Vec3f currDir3 = soWaist->GetPose().Ori() * Vec3f(0,0,-1);
		Vec2f currDir = Vec2f(currDir3.X(), currDir3.Z()).unit();

		walkCtrl->SetRotationAngle(-asin(PTM::cross(currDir, dir)) * 3.0);
		walkCtrl->SetSpeed((PTM::dot(currDir, dir)+1) * 1.0);
	}
}

void CRTravelController::SetGoal(Vec2f goal){
	this->goal = goal;
}
}
