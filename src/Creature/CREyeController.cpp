/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CREyeController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

IF_OBJECT_IMP(CREyeController, SceneObject);

void CREyeController::SetLeftEyeSolid(PHSolidIf* eyeSolid){
	soLEye = eyeSolid;
}

void CREyeController::SetRightEyeSolid(PHSolidIf* eyeSolid){
	soREye = eyeSolid;
}

void CREyeController::SetHeadSolid(PHSolidIf* headSolid){
	soHead = headSolid;
}

void CREyeController::LookAt(Vec3f point){
	lookatPoint = point;
}

void CREyeController::Step(){
	targetDirL  = (lookatPoint - soLEye->GetPose().Pos()).unit();
	targetDirR  = (lookatPoint - soREye->GetPose().Pos()).unit();

	ControlEyeToTargetDir(soLEye, targetDirL);
	ControlEyeToTargetDir(soREye, targetDirR);
}

void CREyeController::ControlEyeToTargetDir(PHSolidIf* soEye, Vec3f target){
	Vec3f currentDir = (soEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, 1.0f)).unit();

	double targetAngH = atan2(target.X(), target.Z());
	double targetAngV = atan2(target.Y(), target.Z());

	Quaternionf targetQ = Quaternionf::Rot(Rad(targetAngV),'x') * Quaternionf::Rot(Rad(targetAngH),'y');
	
	Vec3f error  = PTM::cross(currentDir, target);
	Vec3f derror = soEye->GetAngularVelocity();
	float Kp = 0.3f;
	float Kd = 0.025f;
	Vec3f torque = (Kp * error) - (Kd * derror);

 	soEye->AddTorque(torque);
}

}
