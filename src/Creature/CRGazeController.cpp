/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRGazeController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRGazeController, CRController);

void CRGazeController::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	this->pos = pos;
	this->vel = vel;
	this->attractiveness = attractiveness;

	eyeCtrl->LookAt(pos, vel);
	neckCtrl->LookAt(pos, vel, attractiveness);
}

void CRGazeController::Init(){
	CRController::Init();

	for (int i=0; i<creature->NControllers(); i++) {
		if (!eyeCtrl) {
			eyeCtrl = DCAST(CREyeControllerIf, creature->GetController(i));
		}
		if (!neckCtrl) {
			neckCtrl = DCAST(CRNeckControllerIf, creature->GetController(i));
		}
	}
}

void CRGazeController::Step(){
	CRController::Step();
}
}
