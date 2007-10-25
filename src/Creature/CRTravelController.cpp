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
			// walkCtrl = DCAST(CREseWalkingControllerIf, creature->GetController(i));
			walkCtrl = DCAST(CRWalkingControllerIf, creature->GetController(i));
		}
	}

	goal = Vec3f(0,-10,0);
	maxSpeed = 0.1;
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

	if (goal.Y() > -5) {
		Vec3f currPos = soWaist->GetPose().Pos();
		Vec3f goalPos = goal;
		currPos.Y()=0; goalPos.Y()=0;
		float distance = (goalPos - currPos).norm();

		/**/
		float limitStartDist = 2.0;
		float limitSatDist   = 0.7;
		float maxMax = 1.5;
		float minMax = 0.1;
		/*/
		float limitStartDist = 2.0;
		float limitSatDist   = 0.7;
		// float maxMax = 1.5;
		float maxMax = 3.0;
		float minMax = 0.4;
		/**/

		if (distance < limitSatDist) {
			maxSpeed = minMax;
		} else if (distance < limitStartDist) {
			maxSpeed = minMax + (distance-limitSatDist)/(limitStartDist-limitSatDist)*(maxMax-minMax);
		} else {
			maxSpeed = maxMax;
		}

		/*
		std::cout << "ƒS[ƒ‹ : " << goal;
		std::cout << ", ƒS[ƒ‹‚Ü‚Å‚Ì‹——£ : " << distance;
		std::cout << ", MaxSpeed : " << maxSpeed << std::endl;
		*/
	}

	if (potential.norm() > 0) {
		Vec2f dir = potential.unit();

		Vec3f currDir3 = soWaist->GetPose().Ori() * Vec3f(0,0,-1);
		Vec2f currDir = Vec2f(currDir3.X(), currDir3.Z()).unit();

		/*
		walkCtrl->SetRotationAngle(-asin(PTM::cross(currDir, dir)) * 3.0);
		walkCtrl->SetSpeed((PTM::dot(currDir, dir)+1) * 1.0);
		*/
		walkCtrl->SetRotationAngle(-asin(PTM::cross(currDir, dir)) * 0.5);
		walkCtrl->SetSpeed((PTM::dot(currDir, dir)+1) / 2.0 * maxSpeed);
	}
}

void CRTravelController::SetGoal(Vec3f goal){
	this->goal = goal;
}
}
