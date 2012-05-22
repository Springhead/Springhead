/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRNeckController.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRBodyPart.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHIK.h>

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
	fwHead  = NULL;
	bEnabled = false;
}
void CRNeckController::Stop()
{
	bEnabled = false;
}
void CRNeckController::Step(){
	if (!bEnabled) return;

	PHSolidIf*			soHead = fwHead->GetPHSolid();
	PHIKEndEffectorIf*	efHead = fwHead->GetPHIKEndEffector();

	// <!!> UpVector / FrontVectorをデスクリプタで指定可能にしたほうがいいと思う
	Vec3d rotLook	= PTM::cross(soHead->GetPose().Ori()*Vec3d(0,1,0), (pos-(soHead->GetPose().Pos())).unit());
	Vec3d rotUp		= PTM::cross(soHead->GetPose().Ori()*Vec3d(0,0,-1), Vec3d(0,1,0));
	// Vec3d rotLook	= PTM::cross(soHead->GetPose().Ori()*Vec3d(0,1,0), (pos-(soHead->GetPose().Pos())).unit());
	// Vec3d rotUp		= PTM::cross(soHead->GetPose().Ori()*Vec3d(0,0,-1), Vec3d(0,1,0));

	Vec3d rot		= rotLook + 0.5*rotUp;

	Quaterniond qt = Quaterniond::Rot(rot.norm(), rot.unit());
	efHead->SetTargetOrientation(qt*soHead->GetPose().Ori());

	// <!!> TBI 頭部位置制御を実装する
	// efHead->SetTargetPosition(Vec3d(0,3,0));

	efHead->EnablePositionControl(true);
	efHead->EnableOrientationControl(true);
	efHead->Enable(true);
}

}
