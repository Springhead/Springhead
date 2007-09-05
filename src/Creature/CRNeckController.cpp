/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRNeckController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRNeckController, CRController);

void CRNeckController::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	this->pos = pos;
	this->vel = vel;
	this->attractiveness = attractiveness;
}

void CRNeckController::Init(){
	CRController::Init();

	soHead = creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_HEAD);
	soNeck = creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_NECK);
	
	joNeckHeadX  = DCAST(PHHingeJointIf, creature->GetBody()->GetJoint(CRHingeHumanBodyDesc::JO_NECK_HEAD_X));
	joChestNeckY = DCAST(PHHingeJointIf, creature->GetBody()->GetJoint(CRHingeHumanBodyDesc::JO_CHEST_NECK_Y));
}

void CRNeckController::Step(){
	CRController::Step();

	// 首-頭をつなぐ関節の制御
	/// 頭基準座標系でのターゲットの位置
	Vec3d vecTargetFromHead = soHead->GetPose().Ori().Inv() * (pos - soHead->GetPose().Pos());
	/// Quaternionに変換
	Quaterniond qtnTargetFromHead;
	qtnTargetFromHead.RotationArc(vecTargetFromHead.unit(), Vec3d(0,0,-1));
	/// Eular角に変換
	Vec3d eulTargetFromHead;
	qtnTargetFromHead.ToEular(eulTargetFromHead);
	/// 関節の目標値をセット
	if (joNeckHeadX->GetChildObject(0)==soNeck) {
		joNeckHeadX->SetSpringOrigin(-eulTargetFromHead.Z());
		joChestNeckY->SetSpringOrigin(-eulTargetFromHead.X());
	} else {
		joNeckHeadX->SetSpringOrigin(eulTargetFromHead.Z());
		joChestNeckY->SetSpringOrigin(eulTargetFromHead.X());
	}

	/*
	Quaterniond qLook;
	qLook.RotationArc(pos.unit(), Vec3d(0,0,-1));
	qLook = qLook * soChest->GetPose().Ori();

	PHBallJointDesc ballDesc   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0, 0.1, 0);
	ballDesc.posePlug.Ori()    = Quaternionf::Rot(Rad(0), 'y');
	ballDesc.poseSocket.Pos()  = Vec3d(0,-0.08,0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'y');
	ballDesc.spring            = 100.0;
	ballDesc.origin            = qLook;
	ballDesc.damper            = 50.0;
	joNeck->SetDesc(&ballDesc);
	*/
}
}
