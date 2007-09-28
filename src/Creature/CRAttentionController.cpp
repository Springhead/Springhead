/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRAttentionController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRAttentionController, CRController);

void CRAttentionController::Init(){
	CRController::Init();

	internalScene = creature->GetInternalScene();
	gazeCtrl = NULL;
	for (int i=0; i<creature->NControllers(); i++) {
		if (!gazeCtrl) {
			gazeCtrl = DCAST(CRGazeControllerIf, creature->GetController(i));
		}
	}
}
	
void CRAttentionController::Step(){
	CRController::Step();

	// creature->GetInternalScene()->SortByAttractiveness();
	// CRInternalSceneObjectIf* obj = creature->GetInternalScene()->GetISObject(0);

	float maxattr = 0;
	CRISAttractiveObjectIf* maxattrObj = NULL;
	for (int i=0; i<creature->GetInternalScene()->NObjects(); ++i) {
		CRISAttractiveObjectIf* att = DCAST(CRISAttractiveObjectIf, creature->GetInternalScene()->GetISObject(i));
		if (att && att->GetTotalAttractiveness() > maxattr) {
			maxattr = att->GetTotalAttractiveness();
			maxattrObj = att;
		}
	}
	if (maxattrObj) {
		gazeCtrl->LookAt(maxattrObj->GetSolid()->GetPose() * maxattrObj->GetPos(), maxattrObj->GetSolid()->GetVelocity(), maxattrObj->GetTotalAttractiveness());
	}
}
}
