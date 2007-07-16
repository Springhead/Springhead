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
	currLookatPos = pos;
	currLookatVel = vel;
}

void CREyeController::Step(){
	// 視覚制御に用いる状態量の計算
	CalcEyeStatusValue();

	// 制御状態の遷移
	//controlState = GetNextState(controlState);
	controlState = CS_PURSUIT;

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
}

void CREyeController::CalcEyeStatusValue(){
	// 眼球の座標系として頭の前方向をZ軸、上方向をY軸とする右手系ローカル座標に変換
	qToLoc = soHead->GetPose().Ori().Inv(); // グローバルからローカル座標に変換
	qToGlo = soHead->GetPose().Ori();       // ローカルからグローバル座標に変換

	// 基本量
	// -- 目の回転量
	Quaterniond locLEyeOri, locREyeOri;
	locLEyeOri = qToLoc * soLEye->GetPose().Ori();
	locREyeOri = qToLoc * soREye->GetPose().Ori();
	// -- 目の回転角速度
	Vec3d locLEyeAngvel, locREyeAngvel;
	locLEyeAngvel = qToLoc * soLEye->GetAngularVelocity();
	locREyeAngvel = qToLoc * soREye->GetAngularVelocity();
	// -- 頭の回転角速度
	Vec3d locHeadAngvel;
	locHeadAngvel = qToLoc * soHead->GetAngularVelocity();

	// 中間量
	// -- 視軸（角度表示）
	locLEyeAxisH = Vec3ToAngH(locLEyeOri * Vec3d(0,0,1));
	locLEyeAxisV = Vec3ToAngV(locLEyeOri * Vec3d(0,0,1));
	locREyeAxisH = Vec3ToAngH(locREyeOri * Vec3d(0,0,1));
	locREyeAxisV = Vec3ToAngV(locREyeOri * Vec3d(0,0,1));
	// -- 各眼球から見た視標の方向（角度表示）
	Vec3d locLLookat = qToLoc * (currLookatPos - soLEye->GetPose().Pos());
	Vec3d locRLookat = qToLoc * (currLookatPos - soREye->GetPose().Pos());
	double locLLookatH = Vec3ToAngH(locLLookat), locLLookatV = Vec3ToAngV(locLLookat);
	double locRLookatH = Vec3ToAngH(locRLookat), locRLookatV = Vec3ToAngV(locRLookat);

	// 制御に使う量
	double dt = DCAST(PHSceneIf, GetScene())->GetTimeStep();
	// -- 視標視軸誤差
	locErrLH = locLLookatH - locLEyeAxisH;
	locErrLV = locLLookatV - locLEyeAxisV;
	locErrRH = locRLookatH - locREyeAxisH;
	locErrRV = locRLookatV - locREyeAxisV;

	if (abs(locErrLH - locLastErrLH) > 1.0) {locLastErrLH = locErrLH;}
	if (abs(locErrLV - locLastErrLV) > 1.0) {locLastErrLV = locErrLV;}
	if (abs(locErrRH - locLastErrRH) > 1.0) {locLastErrRH = locErrRH;}
	if (abs(locErrRV - locLastErrRV) > 1.0) {locLastErrRV = locErrRV;}

	// -- 視標視軸誤差変化量
	locDErrLH = (locErrLH - locLastErrLH) / dt; locLastErrLH = locErrLH;
	locDErrLV = (locErrLV - locLastErrLV) / dt; locLastErrLV = locErrLV;
	locDErrRH = (locErrRH - locLastErrRH) / dt; locLastErrRH = locErrRH;
	locDErrRV = (locErrRV - locLastErrRV) / dt; locLastErrRV = locErrRV;
	// -- 頭部回転角速度
	locHeadAngvelH = Vec3ToAngH(locHeadAngvel);
	locHeadAngvelV = Vec3ToAngV(locHeadAngvel);

	if (CREYECTRL_DEBUG) {
		std::cout << "locLEyeAxisH : " << Deg(locLEyeAxisH) << std::endl;
		std::cout << "locLEyeAxisV : " << Deg(locLEyeAxisV) << std::endl;
		std::cout << "locREyeAxisH : " << Deg(locREyeAxisH) << std::endl;
		std::cout << "locREyeAxisV : " << Deg(locREyeAxisV) << std::endl;

		std::cout << "currLookatPos : " << currLookatPos << std::endl;

		std::cout << "locLLookatH : " << Deg(locLLookatH) << std::endl;
		std::cout << "locLLookatV : " << Deg(locLLookatV) << std::endl;
		std::cout << "locRLookatH : " << Deg(locRLookatH) << std::endl;
		std::cout << "locRLookatV : " << Deg(locRLookatV) << std::endl;

		std::cout << "locErrLH : " << Deg(locErrLH) << std::endl;
		std::cout << "locErrLV : " << Deg(locErrLV) << std::endl;
		std::cout << "locErrRH : " << Deg(locErrRH) << std::endl;
		std::cout << "locErrRV : " << Deg(locErrRV) << std::endl;

		std::cout << "locDErrLH : " << Deg(locDErrLH) << std::endl;
		std::cout << "locDErrLV : " << Deg(locDErrLV) << std::endl;
		std::cout << "locDErrRH : " << Deg(locDErrRH) << std::endl;
		std::cout << "locDErrRV : " << Deg(locDErrRV) << std::endl;
	}
}

CREyeControllerState::ControlState CREyeController::GetNextState(ControlState currentCS){
	// 条件判定と次の制御状態の決定
	switch(controlState){
	case CS_SACCADE:
		if ((abs(locErrLH) < Rad(1.0f)) && (abs(locErrRH)  < Rad(1.0f)) && (abs(locErrLV) < Rad(1.0f)) && (abs(locErrRV) < Rad(1.0f))
			&& (abs(locDErrLH) < Rad(0.1f)) && (abs(locDErrRH)  < Rad(0.1f)) && (abs(locDErrLV) < Rad(0.1f)) && (abs(locDErrRV) < Rad(0.1f))) {
			// Saccade終了, Pursuitへ移行
			integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
			return CS_PURSUIT;
		}else{
			// Saccade継続
			return CS_SACCADE;
		}
		break;
	case CS_PURSUIT:
		if ((abs(locErrLH) > Rad(5.0f)) || (abs(locErrRH)  > Rad(5.0f)) || (abs(locErrLV) > Rad(5.0f)) || (abs(locErrRV) > Rad(5.0f))) {
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
	ControlEyeToTargetDir(soLEye, nextLookatPos - soLEye->GetPose().Pos());
	ControlEyeToTargetDir(soREye, nextLookatPos - soREye->GetPose().Pos());
}

void CREyeController::PursuitControl(){
	/*
	const float alpha1 = 0.5f;
	const float rho1 = 1.50f;
	const float rho2 = 0.50f;
	// Error
	const float sigma = 100.0f;
	const float kappa = 50.0f;
	// Velocity
	const float nu =  0.5f;
	const float eta = 0.2f;
	*/

	// Head Rotation
	const float alpha1 = 0.0f;
	// Output Ratio
	const float rho1 = 1.5f;
	const float rho2 = 0.5f;
	// Error Input
	const float sigma = 1.0f;
	const float kappa = 0.5f;
	// Velocity Input
	const float nu =  0.5f;
	const float eta = 0.2f;

	std::cout << " IL : " << integrator_L << std::endl;

	float dt  = (float)(DCAST(PHSceneIf, GetScene())->GetTimeStep());

	//// Horizontal
	/*
	float node_L_1 = -(sigma*locErrLH + nu*locDErrLH) - (kappa*locErrRH + eta*locDErrRH) + (locHeadAngvelH*alpha1);
	float node_R_1 =  (sigma*locErrRH + nu*locDErrRH) + (kappa*locErrLH + eta*locDErrLH) - (locHeadAngvelH*alpha1);
	
	integrator_L += node_L_1 * dt;
	integrator_R += node_R_1 * dt;

	std::cout << " locErrLH : " << locErrLH << std::endl;
	std::cout << " locDErrLH : " << locDErrLH << std::endl;
	std::cout << " locErrRH : " << locErrRH << std::endl;
	std::cout << " locDErrLH : " << locDErrRH << std::endl;

	std::cout << " nodeL : " << node_L_1 << std::endl;
	std::cout << " nodeR : " << node_R_1 << std::endl;
	std::cout << " IL : " << integrator_L << std::endl;
	std::cout << std::endl;

	float out_t1 = -(integrator_L * rho1) + (integrator_R * rho2) + locLEyeAxisH;
	float out_t2 = -(integrator_L * rho2) + (integrator_R * rho1) + locREyeAxisH;
	*/

	integrator_L += dt * ((rho1*nu+rho2*eta)*locDErrLH + (rho1*sigma+rho2*kappa)*locErrLH + (rho1*eta+rho2*nu)*locDErrRH + (rho1*kappa+rho2*sigma)*locErrRH);
	integrator_R += dt * ((rho1*nu+rho2*eta)*locDErrRH + (rho1*sigma+rho2*kappa)*locErrRH + (rho1*eta+rho2*nu)*locDErrLH + (rho1*kappa+rho2*sigma)*locErrLH);

	float out_t1 = integrator_L + locLEyeAxisH;
	float out_t2 = integrator_R + locREyeAxisH;

	//// Vertical
	float node_L_2 = (sigma*locErrLV + nu*locDErrLV);// + (locHeadAngvelV*alpha1);
	float node_R_2 = (sigma*locErrRV + nu*locDErrRV);// + (locHeadAngvelV*alpha1);

	integrator_Lv += node_L_2 * dt;
	integrator_Rv += node_R_2 * dt;

	float out_t3 = (integrator_Lv * rho1) + locLEyeAxisV;
	float out_t4 = (integrator_Rv * rho1) + locREyeAxisV;

	Vec3f dirL, dirR;

	/*
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
	*/

	dirL = qToGlo*Quaternionf::Rot(-out_t3,'x')*Quaternionf::Rot(out_t1,'y')*Vec3f(0,0,1);
	dirR = qToGlo*Quaternionf::Rot(-out_t4,'x')*Quaternionf::Rot(out_t2,'y')*Vec3f(0,0,1);

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
	/**/

	/**/
	double angH = Vec3ToAngH(target);
	double angV = Vec3ToAngV(target);
	std::cout << "(angH, angV) = (" << Deg(angH) << ", " << Deg(angV) << ")" << std::endl;
	Quaterniond ori = Quaterniond::Rot(-angV,'x')*Quaterniond::Rot(angH,'y');
	Posed pose = soEye->GetPose();
	pose.Ori() = ori;
	soEye->SetPose(pose);
	/**/

	/*/
	double angH = Vec3ToAngH(target);
	double angV = Vec3ToAngV(target);
	std::cout << "(angH, angV) = (" << Deg(angH) << ", " << Deg(angV) << ")" << std::endl;

	Vec3f curr = (soEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, 1.0f)).unit();
	double angHc = Vec3ToAngH(curr);
	double angVc = Vec3ToAngV(curr);

	Vec3f tqD = soEye->GetAngularVelocity()*Kd;
	Vec3f tqH = Vec3d(0,1,0)*angHc*Kp, tqV = Vec3d(-1,0,0)*angVc*Kp;
	soEye->AddTorque(tqH+tqV-tqD);
	/**/
}

double CREyeController::Vec3ToAngH(Vec3d v){
	double D = sqrt(v.X()*v.X()+v.Z()*v.Z());
	return(atan2(v.X()/D, v.Z()/D));
}

double CREyeController::Vec3ToAngV(Vec3d v){
	double D = sqrt(v.Y()*v.Y()+v.Z()*v.Z());
	return(atan2(v.Y()/D, v.Z()/D));
}
}
