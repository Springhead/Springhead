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
	// 視覚制御に用いる状態量の計算
	CalcEyeStatusValue();

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

	ControlEyeToTargetDir(soLEye, nextLookatPos - soLEye->GetPose().Pos());
	//ControlEyeToTargetDir(soREye, nextLookatPos - soREye->GetPose().Pos());
}

void CREyeController::CalcEyeStatusValue(){
	// 頭の前方向をZ軸、上方向をY軸とする右手系ローカル座標に変換
	Quaterniond qHeadOriInv = soHead->GetPose().Ori().Inv();
	// -- 目の回転量、視軸の角度
	Quaterniond localOriLEye, localOriREye;
	localOriLEye = qHeadOriInv * soLEye->GetPose().Ori();
	localOriREye = qHeadOriInv * soREye->GetPose().Ori();
	Vec3d localEularAngleLEye, localEularAngleREye;
	localOriLEye.ToEular(localEularAngleLEye);
	localOriREye.ToEular(localEularAngleREye);
	double localHorizAngleLEye=localEularAngleLEye[0], localHorizAngleREye=localEularAngleREye[0];
	double localVertAngleLEye=localEularAngleLEye[1], localVertAngleREye=localEularAngleREye[1];
	// -- 目の回転角速度
	Vec3d localangvelLEye, localangvelREye;
	localangvelLEye = qHeadOriInv * soLEye->GetAngularVelocity();
	localangvelREye = qHeadOriInv * soREye->GetAngularVelocity();
	// -- 頭の回転角速度
	Vec3d localangvelHead;
	localangvelHead = qHeadOriInv * soHead->GetAngularVelocity();
	// -- 視標の位置、方向
	Vec3d localLookatPosFromLEye, localLookatPosFromREye;
	localLookatPosFromLEye = qHeadOriInv * (currLookatPos - soLEye->GetPose().Pos());
	localLookatPosFromREye = qHeadOriInv * (currLookatPos - soREye->GetPose().Pos());
	Vec3d llpflh = localLookatPosFromLEye, llpflv = localLookatPosFromLEye;
	Vec3d llpfrh = localLookatPosFromREye, llpfrv = localLookatPosFromREye;
	double localLookatHorizAngleFromLEye = -atan2(llpflh.X(),llpflh.Z())
	double localLookatHorizAngleFromREye = -atan2(llpfrh.X(),llpfrh.Z())
	double localLookatVertAngleFromLEye = -atan2(llpflh.X(),llpflh.Z())
	double localLookatVertAngleFromREye = -atan2(llpfrh.X(),llpfrh.Z())

	Vec3f apHoriz, apVert;
	apHoriz = apVert = soHead->GetPose().Ori().Inv() * currLookatPos;

	apHoriz[1] = 0.0f;
	apHoriz = soHead->GetPose().Ori() * apHoriz;
	apVert[0]  = 0.0f;
	apVert  = soHead->GetPose().Ori() * apVert;

	Vec3f eyeDirRelL = soHead->GetPose().Ori().Inv() * soLEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, -1.0f);
	Vec3f eyeDirRelR = soHead->GetPose().Ori().Inv() * soREye->GetPose().Ori() * Vec3f(0.0f, 0.0f, -1.0f);
	t1 =  atan2(eyeDirRelL.Z(), -eyeDirRelL.X());
	t2 =  atan2(eyeDirRelR.Z(), -eyeDirRelR.X());
	t3 = -atan2(eyeDirRelL.Y(),  eyeDirRelL.Z());
	t4 = -atan2(eyeDirRelR.Y(),  eyeDirRelR.Z());

	float dt = (float)(DCAST(PHSceneIf, GetScene())->GetTimeStep());

	Vec3f vecL = apHoriz - soLEye->GetCenterPosition();
	vecL = soHead->GetPose().Ori().Inv() * vecL;
	float t1_a = atan2(-vecL.Z(),vecL.X());
	eL   = t1_a - t1;
	vL   = (t1_a - last_t1_a) / dt;
	last_t1_a = t1_a;

	Vec3f vecR = apHoriz - soREye->GetCenterPosition();
	vecR = soHead->GetPose().Ori().Inv() * vecR;
	float t2_a = atan2(-vecR.Z(),vecR.X());
	eR   = t2_a - t2;
	vR   = (t2_a - last_t2_a) / dt;
	last_t2_a = t2_a;

	Vec3f vecLV = apVert - soLEye->GetCenterPosition();
	vecLV = soHead->GetPose().Ori().Inv() * vecLV;
	float t3_a = atan2(vecLV.Y(),-vecLV.Z());
	eLV  = t3_a - t3;
	vLV  = (t3_a - last_t3_a) / dt;
	last_t3_a = t3_a;

	Vec3f vecRV = apVert - soREye->GetCenterPosition();
	vecRV = soHead->GetPose().Ori().Inv() * vecRV;
	float t4_a = atan2(vecRV.Y(),-vecRV.Z());
	eRV  = t4_a - t4;
	vRV  = (t4_a - last_t4_a) / dt;
	last_t4_a = t4_a;
}

CREyeControllerState::ControlState CREyeController::GetNextState(ControlState currentCS){
	// 条件判定と次の制御状態の決定
	switch(controlState){
	case CS_SACCADE:
		std::cout << Deg(eL) << ", " << Deg(eR) << ", " << Deg(eLV) << ", " << Deg(eRV) << std::endl;
		if ((abs(eL) < Rad(0.1f)) && (abs(eR)  < Rad(0.1f)) && (abs(eLV) < Rad(0.1f)) && (abs(eRV) < Rad(0.1f))) {
			// Saccade終了, Pursuitへ移行
			integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
			soLEye->SetVelocity(Vec3f(0,0,0));
			soREye->SetVelocity(Vec3f(0,0,0));
			return CS_PURSUIT;
		}else{
			// Saccade継続
			return CS_SACCADE;
		}
		break;
	case CS_PURSUIT:
		//if ((abs(eL) > Rad(1.0f)) || (abs(eR)  > Rad(1.0f)) || (abs(eLV) > Rad(1.0f)) || (abs(eRV) > Rad(1.0f))) {
		//if ((abs(eL) > Rad(5.0f)) || (abs(eR)  > Rad(5.0f)) || (abs(eLV) > Rad(5.0f)) || (abs(eRV) > Rad(5.0f))) {
		if ((abs(eL) > Rad(50.0f)) || (abs(eR)  > Rad(50.0f)) || (abs(eLV) > Rad(50.0f)) || (abs(eRV) > Rad(50.0f))) {
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
	Vec3f saccadeToL  = (currLookatPos - soLEye->GetPose().Pos()).unit();
	Vec3f saccadeToR  = (currLookatPos - soREye->GetPose().Pos()).unit();

	float aL = acos(PTM::dot(saccadeToL/saccadeToL.norm(), saccadeFromL/saccadeFromL.norm()));
	float aR = acos(PTM::dot(saccadeToR/saccadeToR.norm(), saccadeFromR/saccadeFromR.norm()));
	float t = (saccadeTimer += (float)((DCAST(PHSceneIf, GetScene()))->GetTimeStep()));
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
}

void CREyeController::PursuitControl(){
	const float alpha1 = 0.5f;
	const float rho1 = 1.50f;
	const float rho2 = 0.50f;
	// Error
	const float sigma = 100.0f;
	const float kappa = 50.0f;
	// Velocity
	const float nu =  0.5f;
	const float eta = 0.2f;

	currLookatPos = nextLookatPos;
	currLookatVel = nextLookatVel;

	float dw  = (float)(soHead->GetAngularVelocity().Y());
	float dwv = (float)(soHead->GetAngularVelocity().X());

	float dt  = (float)(DCAST(PHSceneIf, GetScene())->GetTimeStep());

	//// Horizontal
	float node_L_1 = -(sigma*eL + nu*vL) - (kappa*eR + eta*vR) + (dw*alpha1);
	float node_R_1 =  (sigma*eR + nu*vR) + (kappa*eL + eta*vL) - (dw*alpha1);
	
	integrator_L += node_L_1 * dt;
	integrator_R += node_R_1 * dt;
	
	float out_t1 = -(integrator_L * rho1) + (integrator_R * rho2) + t1;
	float out_t2 = -(integrator_L * rho2) + (integrator_R * rho1) + t2;

	//// Vertical
	float node_L_2 = (sigma*eLV + nu*vLV);// + (dwv*alpha1);
	float node_R_2 = (sigma*eRV + nu*vRV);// + (dwv*alpha1);

	integrator_Lv += node_L_2 * dt;
	integrator_Rv += node_R_2 * dt;

	float out_t3 = (integrator_Lv * rho1) + t3;
	float out_t4 = (integrator_Rv * rho1) + t4;

	Vec3f dirL, dirR;

	if (cos(out_t3)!=0){
		dirL = Vec3f(-cos(out_t1), -sin(out_t1)*tan(out_t3), sin(out_t1));
	}else{
		dirL = Vec3f(-cos(out_t1), 0.0f, 0.0f);
	}

	if (cos(out_t4)!=0){
		dirR = Vec3f(-cos(out_t2), -sin(out_t2)*tan(out_t4), sin(out_t2));
	}else{
		dirR = Vec3f(-cos(out_t2), 0.0f, 0.0f);
	}

	dirL = -(soHead->GetPose().Ori() * dirL);
	dirR = -(soHead->GetPose().Ori() * dirR);
	
	ControlEyeToTargetDir(soLEye, dirL);
	ControlEyeToTargetDir(soREye, dirR);
}

void CREyeController::ControlEyeToTargetDir(PHSolidIf* soEye, Vec3f target){
	/*/
	Vec3f currentDir = (soEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, 1.0f)).unit();
	Vec3f errorYawPitch = PTM::cross(currentDir, target);
	Vec3f derror = soEye->GetAngularVelocity();
	Vec3f torque = (Kp * (errorYawPitch)) - (Kd * derror);
 	soEye->AddTorque(torque);
	/*/
	Vec3f targH, targV; targH = targV = target;
	targH[1]=0.0f; targH.unitize();
	targV[0]=0.0f; targV.unitize();
	float angH, angV;
	angH = atan2(targH[0],targH[2]);
	angV = atan2(targV[1],targV[2]);
	//std::cout << Deg(angH) << ", " << Deg(angV) << std::endl;
	Quaterniond ori = Quaternionf::Rot(angV,'x')*Quaternionf::Rot(angH,'y');
	Posed pose = soEye->GetPose();
	pose.Ori() = ori;
	soEye->SetPose(pose);
	/**/
}

}
