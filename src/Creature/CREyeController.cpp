/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CREyeController.h"

#define CREYECTRL_DEBUG true

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

IF_OBJECT_IMP(CREyeController, SceneObject);

void CREyeController::LookAt(Vec3f pos, Vec3f vel){
	nextLookatPos = pos;
	nextLookatVel = vel;
}

void CREyeController::Step(){
	/*/ こっちはとりあえず動くコード
	currLookatPos = nextLookatPos;
	
	Vec3f targetDirL  = (currLookatPos - soLEye->GetPose().Pos()).unit();
	Vec3f targetDirR  = (currLookatPos - soREye->GetPose().Pos()).unit();

	ControlEyeToTargetDir(soLEye, targetDirL);
	ControlEyeToTargetDir(soREye, targetDirR);
	/*/
	// 制御状態の遷移
	controlState = GetNextState(controlState);

	// 実際の眼球運動制御の実行
	Vec3f targetDirL, targetDirR;
	switch(controlState){
	case CS_SACCADE:
		if(CREYECTRL_DEBUG){std::cout << "Saccade" << std::endl;}
		SaccadeControl();
		break;
	case CS_PURSUIT:
		if(CREYECTRL_DEBUG){std::cout << "Pursuit" << std::endl;}
		PursuitControl();
		break;
	default:
		break;
	}
	/**/
}

CREyeControllerState::ControlState CREyeController::GetNextState(ControlState currentCS){
	// 条件判定に必要な数値の計算
	Vec3f apHoriz, apVert;
	apHoriz = apVert = soHead->GetPose().Ori().Inv() * currLookatPos;

	apHoriz[1] = 0.0f;
	apHoriz = soHead->GetPose().Ori() * apHoriz;
	apVert[0]  = 0.0f;
	apVert  = soHead->GetPose().Ori() * apVert;

	Vec3f eyeDirRelL = soHead->GetPose().Ori().Inv() * soLEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, -1.0f);
	Vec3f eyeDirRelR = soHead->GetPose().Ori().Inv() * soREye->GetPose().Ori() * Vec3f(0.0f, 0.0f, -1.0f);
	float t1 =  atan2(eyeDirRelL.Z(), -eyeDirRelL.X());
	float t2 =  atan2(eyeDirRelR.Z(), -eyeDirRelR.X());
	float t3 = -atan2(eyeDirRelL.Y(),  eyeDirRelL.Z());
	float t4 = -atan2(eyeDirRelR.Y(),  eyeDirRelR.Z());

	Vec3f vecL = apHoriz - soLEye->GetCenterPosition();
	vecL = soHead->GetPose().Ori().Inv() * vecL;
	float t1_a = atan2(-vecL.Z(),vecL.X());
	float eL   = t1_a - t1;

	Vec3f vecR = apHoriz - soREye->GetCenterPosition();
	vecR = soHead->GetPose().Ori().Inv() * vecR;
	float t2_a = atan2(-vecR.Z(),vecR.X());
	float eR   = t2_a - t2;

	Vec3f vecLV = apVert - soLEye->GetCenterPosition();
	vecLV = soHead->GetPose().Ori().Inv() * vecLV;
	float t3_a = atan2(vecLV.Y(),-vecLV.Z());
	float eLV  = t3_a - t3;

	Vec3f vecRV = apVert - soREye->GetCenterPosition();
	vecRV = soHead->GetPose().Ori().Inv() * vecRV;
	float t4_a = atan2(vecRV.Y(),-vecRV.Z());
	float eRV  = t4_a - t4;

	// 条件判定と次の制御状態の決定
	switch(controlState){
	case CS_SACCADE:
		if ((abs(eL) < Rad(0.1f)) && (abs(eR)  < Rad(0.1f)) && (abs(eLV) < Rad(0.1f)) && (abs(eRV) < Rad(0.1f))) {
			// Saccade終了
			return CS_PURSUIT;
		}else{
			// Saccade継続
			return CS_SACCADE;
		}
		break;
	case CS_PURSUIT:
		if ((abs(eL) > Rad(5.0f)) || (abs(eR)  > Rad(5.0f)) || (abs(eLV) > Rad(5.0f)) || (abs(eRV) > Rad(5.0f))) {
			// Saccadeへ移行
			currLookatPos = nextLookatPos;
			currLookatVel = nextLookatVel;
			saccadeTimer = 0.0f;
			saccadeFromR = soLEye->GetPose().Ori() * Vec3f(0.0f,0.0f,1.0f);
			saccadeFromL = soREye->GetPose().Ori() * Vec3f(0.0f,0.0f,1.0f);
			return CS_SACCADE;
		}else{
			// Pursuit継続
			return CS_PURSUIT;
		}
		break;
	default:
		assert(false && "到達しないはずの文");
		break;
	}

	// ここにはこないはずだけど
	return CS_SACCADE;
}

void CREyeController::SaccadeControl(){
	/*/
	currLookatPos = nextLookatPos;
	currLookatVel = nextLookatVel;

	Vec3f targetDirL  = (currLookatPos - soLEye->GetPose().Pos()).unit();
	Vec3f targetDirR  = (currLookatPos - soREye->GetPose().Pos()).unit();

	ControlEyeToTargetDir(soLEye, targetDirL);
	ControlEyeToTargetDir(soREye, targetDirR);
	/*/
	Vec3f saccadeToL  = (currLookatPos - soLEye->GetPose().Pos()).unit();
	Vec3f saccadeToR  = (currLookatPos - soREye->GetPose().Pos()).unit();

	float aL = acos(PTM::dot(saccadeToL/saccadeToL.norm(), saccadeFromL/saccadeFromL.norm()));
	float aR = acos(PTM::dot(saccadeToR/saccadeToR.norm(), saccadeFromR/saccadeFromR.norm()));
	PHSceneIf* phScene = DCAST(PHSceneIf, GetScene());
	if(CREYECTRL_DEBUG){DSTR << phScene << std::endl;}
	float t = (saccadeTimer += (float)phScene->GetTimeStep());
	float L = (float)Rad(500.0f);
	float T = 0.05f;
	float lengthL = 1.0f;
	float lengthR = 1.0f;

	#define theta(x)   (6*pow(((x)/T),5) - 15*pow(((x)/T),4) + 10*pow(((x)/T),3))
	#define dtheta(x)  (30*pow((x),4)/pow(T,5) - 60*pow((x),4)/pow(T,4) + 30*pow((x),2)/pow(T,3))

	if (aL*dtheta(T/2.0f) >= L) {
		float a_ = L/dtheta(T/2.0f);
		if (0 <= t && t < T/2.0f) {
			lengthL = (a_*theta(t))/aL;
		} else if (t < ((aL-a_)/L + T/2.0f)) {
			lengthL = (L*t + a_/2.0f -L*T/2.0f)/aL;
		} else if (t < ((aL-a_)/L + T     )) {
			lengthL = (a_*theta(t-(aL-a_)/L) + (aL-a_))/aL;
		}
	} else {
		if (0 <=t && t < T) {
			lengthL = (aL*theta(t))/aL;
		}
	}
	if (aR*dtheta(T/2.0f) >= L) {
		float a_ = L/dtheta(T/2.0f);
		if (0 <= t && t < T/2.0f) {
			lengthR = (a_*theta(t))/aR;
		} else if (t < ((aR-a_)/L + T/2.0f)) {
			lengthR = (L*t + a_/2.0f -L*T/2.0f)/aR;
		} else if (t < ((aR-a_)/L + T     )) {
			lengthR = (a_*theta(t-(aR-a_)/L) + (aR-a_))/aR;
		}
	} else {
		if (0 <=t && t < T) {
			lengthR = (aR*theta(t))/aR;
		}
	}

	Quaternionf headori     = soHead->GetPose().Ori();
	Quaternionf headori_inv = headori.Inv();
	Vec3f dirL = headori*((headori_inv*saccadeToL)*lengthL) + saccadeFromL*(1-lengthL);
	Vec3f dirR = headori*((headori_inv*saccadeToR)*lengthR) + saccadeFromR*(1-lengthR);

	ControlEyeToTargetDir(soLEye, dirL);
	ControlEyeToTargetDir(soREye, dirR);

	/**/
}

void CREyeController::PursuitControl(){
	currLookatPos = nextLookatPos;
	currLookatVel = nextLookatVel;
}

void CREyeController::ControlEyeToTargetDir(PHSolidIf* soEye, Vec3f target){
	Vec3f currentDir = (soEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, 1.0f)).unit();
	Vec3f errorYawPitch = PTM::cross(currentDir, target);
	Vec3f derror = soEye->GetAngularVelocity();
	Vec3f torque = (Kp * (errorYawPitch)) - (Kd * derror);

 	soEye->AddTorque(torque);
}

}
