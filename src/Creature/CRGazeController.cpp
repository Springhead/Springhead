/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRGazeController.h>

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRGazeController::Init(){
	orig = Quaterniond();
	head = NULL;
}

void CRGazeController::Step(){
	if (!enabled) return;

	PHSolidIf*			soHead = head->GetPHSolid();
	PHIKEndEffectorIf*	efHead = head->GetIKEndEffector();

	Vec3d rotLook	= PTM::cross(soHead->GetPose().Ori()*front, (pos-(soHead->GetPose().Pos())).unit());
	Vec3d rotUp		= PTM::cross(soHead->GetPose().Ori()*up, front);
	Vec3d rot		= rotLook + 0.5*rotUp;

	Quaterniond qt = Quaterniond::Rot(rot.norm(), rot.unit());

	if (efHead) {
		efHead->SetTargetOrientation(qt*soHead->GetPose().Ori());

		// <!!> TBI 頭部位置制御を実装する
		// efHead->SetTargetPosition(Vec3d(0,3,0));

		efHead->EnablePositionControl(true);
		efHead->EnableOrientationControl(true);
		efHead->Enable(true);
	}
}

void CRGazeController::Reset(){
	// not implemented now
}

int CRGazeController::GetStatus(){
	// not implemented now
	return CRControllerDesc::CS_WAITING;
}

void CRGazeController::SetTargetPosition(Vec3d pos){
	enabled = true;
	this->pos = pos;
}

Vec3d CRGazeController::GetTargetPosition(){
	return this->pos;
}
}
