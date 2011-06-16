/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRNeckController.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHIK.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRNeckController::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	this->bEnabled = true;
	this->pos = pos;
	this->vel = vel;
	this->attractiveness = attractiveness;
}

void CRNeckController::Init(){
	orig = Quaterniond();
	csHead  = NULL;
	bEnabled = false;
}
void CRNeckController::Stop()
{
	bEnabled = false;
}
void CRNeckController::Step(){
	if (!bEnabled) return;
	if (csHead) {
		PHSolidIf*			soHead = csHead->GetPHSolid();
		PHIKEndEffectorIf*	efHead = csHead->GetIKEndEffector();

		Vec3d rotLook	= PTM::cross(soHead->GetPose().Ori()*Vec3d(0,1,0), (pos-(soHead->GetPose().Pos())).unit());
		Vec3d rotUp		= PTM::cross(soHead->GetPose().Ori()*Vec3d(0,0,-1), Vec3d(0,1,0));
		Vec3d rot		= rotLook + 0.3*rotUp;

		if (!(attractiveness <= lowerAttractiveness)) {
			if (attractiveness < upperAttractiveness) {
				rot = rot * ((attractiveness - lowerAttractiveness) / (upperAttractiveness - lowerAttractiveness));
			}

			Quaterniond qt = Quaterniond::Rot(rot.norm(), rot.unit());
			efHead->SetTargetOrientation(qt*soHead->GetPose().Ori());

			efHead->EnableOrientationControl(true);
			efHead->Enable(true);
		}
	} else {
		// ‚¢‚¸‚êAddChildObject‚ÅŽw’è‚·‚é‚æ‚¤‚É‚·‚×‚« (mitake, 09/07/19)
		CRBodyIf* body = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetBody(0);
		PHSceneIf* phScene = DCAST(CRCreatureIf,DCAST(SceneObjectIf,this)->GetScene())->GetPHScene();

		for (size_t i=0; i<body->NChildObject(); ++i) {
			CRIKSolidIf* cso = body->GetChildObject(i)->Cast();
			if (cso) {
				if (UTString("Head")  == cso->GetLabel()) { csHead  = cso; }
			}
		}
	}
}

}
