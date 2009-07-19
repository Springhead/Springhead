/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRNeckController.h"
#include <string>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRNeckController::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	this->pos = pos;
	this->vel = vel;
	this->attractiveness = attractiveness;
}

void CRNeckController::Init(){
	origX = 0.0;
	origZ = 0.0;

	csHead  = NULL;
	csChest = NULL;
}

void CRNeckController::Step(){
	if (csHead) {
		PHSolidIf*    soHead = csHead->GetPHSolid();
		PHIKOriCtlIf* cpHead = NULL;
		for (size_t i=0; i<csHead->NIKControlPoints(); ++i) {
			if (!cpHead) {
				cpHead = csHead->GetIKControlPoint(i)->Cast();
			} else {
				break;
			}
		}
		Vec3d rot = PTM::cross(soHead->GetPose().Ori()*Vec3d(0,1,0), (pos-(soHead->GetPose().Pos())).unit());
		if (!(attractiveness <= lowerAttractiveness)) {
			if (attractiveness < upperAttractiveness) {
				rot = rot * ((attractiveness - lowerAttractiveness) / (upperAttractiveness - lowerAttractiveness));
			}
			Quaterniond qt = Quaterniond::Rot(rot.norm(), rot.unit());
			cpHead->SetGoal(qt*soHead->GetPose().Ori());
			cpHead->Enable(true);
		}
	} else {
		// ‚¢‚¸‚êAddChildObject‚ÅŽw’è‚·‚é‚æ‚¤‚É‚·‚×‚« (mitake, 09/07/19)
		CRBodyIf* body = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetBody(0);
		PHSceneIf* phScene = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetPHScene();

		for (size_t i=0; i<body->NChildObject(); ++i) {
			CRIKSolidIf* cso = body->GetChildObject(i)->Cast();
			if (cso) {
				if (std::string("Head")  == cso->GetLabel()) { csHead  = cso; }
				if (std::string("Chest") == cso->GetLabel()) { csChest = cso; }
			}
		}
	}
}

}
