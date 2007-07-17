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
	lookatPos = pos;
	lookatVel = vel;
}

void CREyeController::Step(){
	// 視覚制御に用いる状態量の計算
	CalcEyeStatusValue();

	// 制御状態の遷移
	controlState = GetNextState(controlState);
	//controlState = CS_PURSUIT;
	//controlState = CS_SACCADE;

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
	double dt = DCAST(PHSceneIf, GetScene())->GetTimeStep() / 10.0;

	// 眼球の座標系として頭の前方向をZ軸、上方向をY軸とする右手系ローカル座標に変換
	qToLoc = soHead->GetPose().Ori().Inv(); // グローバルからローカル座標に変換
	qToGlo = soHead->GetPose().Ori();       // ローカルからグローバル座標に変換

	// -- 視軸（角度表示）
	locLEyeAxisH = Vec3ToAngH(qToLoc * soLEye->GetPose().Ori() * Vec3d(0,0,1));
	locLEyeAxisV = Vec3ToAngV(qToLoc * soLEye->GetPose().Ori() * Vec3d(0,0,1));
	locREyeAxisH = Vec3ToAngH(qToLoc * soREye->GetPose().Ori() * Vec3d(0,0,1));
	locREyeAxisV = Vec3ToAngV(qToLoc * soREye->GetPose().Ori() * Vec3d(0,0,1));
	// -- 各眼球から見た視標の方向（角度表示）
	Vec3d locLLookat = qToLoc * (lookatPos - soLEye->GetPose().Pos());
	Vec3d locRLookat = qToLoc * (lookatPos - soREye->GetPose().Pos());
	double locLLookatH = Vec3ToAngH(locLLookat), locLLookatV = Vec3ToAngV(locLLookat);
	double locRLookatH = Vec3ToAngH(locRLookat), locRLookatV = Vec3ToAngV(locRLookat);
	// -- 視標視軸誤差
	locErrLH = Vec3ToAngH(qToLoc * (lookatPos - soLEye->GetPose().Pos())) - locLEyeAxisH;
	locErrLV = Vec3ToAngV(qToLoc * (lookatPos - soLEye->GetPose().Pos())) - locLEyeAxisV;
	locErrRH = Vec3ToAngH(qToLoc * (lookatPos - soREye->GetPose().Pos())) - locREyeAxisH;
	locErrRV = Vec3ToAngV(qToLoc * (lookatPos - soREye->GetPose().Pos())) - locREyeAxisV;
	// -- 視標視軸誤差変化量
	if (!bContLocErr) {
		locLastErrLH = 0; locLastErrLV = 0; locLastErrRH = 0; locLastErrRV = 0;
		bContLocErr = true;
	}
	locDErrLH = (locErrLH - locLastErrLH) / dt; locLastErrLH = locErrLH;
	locDErrLV = (locErrLV - locLastErrLV) / dt; locLastErrLV = locErrLV;
	locDErrRH = (locErrRH - locLastErrRH) / dt; locLastErrRH = locErrRH;
	locDErrRV = (locErrRV - locLastErrRV) / dt; locLastErrRV = locErrRV;
	// -- 頭部回転角速度
	locHeadAngvelH = Vec3ToAngH(qToLoc * soHead->GetAngularVelocity());
	locHeadAngvelV = Vec3ToAngV(qToLoc * soHead->GetAngularVelocity());

	if (CREYECTRL_DEBUG) {
		std::cout << "lookatPos : " << lookatPos << std::endl;

		std::cout << "locLEyeAxisH : " << Deg(locLEyeAxisH) << std::endl;
		std::cout << "locLEyeAxisV : " << Deg(locLEyeAxisV) << std::endl;
		std::cout << "locREyeAxisH : " << Deg(locREyeAxisH) << std::endl;
		std::cout << "locREyeAxisV : " << Deg(locREyeAxisV) << std::endl;

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
		//if ((abs(locErrLH) < Rad(10.0f)) && (abs(locErrRH)  < Rad(10.0f))) {
		if ((abs(locErrLH) < Rad(10.0f)) && (abs(locErrRH)  < Rad(10.0f)) && (abs(locErrLV) < Rad(10.0f)) && (abs(locErrRV) < Rad(10.0f))) {
			//&& (abs(locDErrLH) < Rad(1.0f)) && (abs(locDErrRH)  < Rad(1.0f)) && (abs(locDErrLV) < Rad(1.0f)) && (abs(locDErrRV) < Rad(1.0f))) {
			// Saccade終了, Pursuitへ移行
			locLastErrLH = 0; locLastErrLV = 0; locLastErrRH = 0; locLastErrRV = 0;
			integrator_L = integrator_R = integrator_Lv = integrator_Rv = 0.0f;
			locDErrLH = locDErrLV = locDErrRH = locDErrRV = 0.0;
			return CS_PURSUIT;
		}else{
			// Saccade継続
			return CS_SACCADE;
		}
		break;
	case CS_PURSUIT:
		if ((abs(locErrLH) > Rad(15.0f)) || (abs(locErrRH)  > Rad(15.0f)) || (abs(locErrLV) > Rad(15.0f)) || (abs(locErrRV) > Rad(15.0f))) {
			// Saccadeへ移行
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
	ControlEyeToTargetDir(soLEye, lookatPos - soLEye->GetPose().Pos());
	ControlEyeToTargetDir(soREye, lookatPos - soREye->GetPose().Pos());
}

void CREyeController::PursuitControl(){
	// Head Rotation
	const double alpha1 = 0.0f;
	// Output Ratio
	const double rho1 = 1.5f;
	const double rho2 = 0.5f;
	// Error Input
	const double sigma = 1.0f;
	const double kappa = 0.5f;
	// Velocity Input
	const double nu =  0.5f;
	const double eta = 0.2f;

	double dt  = (double)(DCAST(PHSceneIf, GetScene())->GetTimeStep()) / 10.0;

	// Horizontal
	integrator_L += dt * ((rho1*nu+rho2*eta)*locDErrLH + (rho1*sigma+rho2*kappa)*locErrLH + (rho1*eta+rho2*nu)*locDErrRH + (rho1*kappa+rho2*sigma)*locErrRH);
	integrator_R += dt * ((rho1*nu+rho2*eta)*locDErrRH + (rho1*sigma+rho2*kappa)*locErrRH + (rho1*eta+rho2*nu)*locDErrLH + (rho1*kappa+rho2*sigma)*locErrLH);
	double out_t1 = integrator_L + locLEyeAxisH;
	double out_t2 = integrator_R + locREyeAxisH;

	// Vertical
	integrator_Lv += dt * ((rho1*nu+rho2*eta)*locDErrLV + (rho1*sigma+rho2*kappa)*locErrLV);
	integrator_Rv += dt * ((rho1*nu+rho2*eta)*locDErrRH + (rho1*sigma+rho2*kappa)*locErrRH);
	double out_t3 = integrator_Lv + locLEyeAxisH;
	double out_t4 = integrator_Rv + locREyeAxisH;

	std::cout << "deg_integrator_L  : " << Deg(integrator_L ) << std::endl;
	std::cout << "deg_integrator_R  : " << Deg(integrator_R ) << std::endl;
	std::cout << "deg_integrator_Lv : " << Deg(integrator_Lv) << std::endl;
	std::cout << "deg_integrator_Rv : " << Deg(integrator_Rv) << std::endl;

	out_t3 = Vec3ToAngV(qToLoc*(lookatPos - soLEye->GetPose().Pos()));
	out_t4 = Vec3ToAngV(qToLoc*(lookatPos - soREye->GetPose().Pos()));

	// Output
	ControlEyeToTargetDir(soLEye, qToGlo*Quaterniond::Rot(-out_t3,'x')*Quaterniond::Rot(out_t1,'y')*Vec3f(0,0,1));
	ControlEyeToTargetDir(soREye, qToGlo*Quaterniond::Rot(-out_t4,'x')*Quaterniond::Rot(out_t2,'y')*Vec3f(0,0,1));
}

void CREyeController::ControlEyeToTargetDir(PHSolidIf* soEye, Vec3d target){
	/*/ //普通のPD制御
	Vec3f currentDir = (soEye->GetPose().Ori() * Vec3f(0.0f, 0.0f, 1.0f)).unit();
	Vec3f errorYawPitch = PTM::cross(currentDir, target);
	Vec3f derror = soEye->GetAngularVelocity();
	Vec3f torque = (Kp * (errorYawPitch)) - (Kd * derror);
 	soEye->AddTorque(torque);
	/**/

	/**/ //制御しないで直接姿勢設定（理想的なもっとも硬い制御）
	double angH = Vec3ToAngH(target);
	double angV = Vec3ToAngV(target);
	std::cout << "(angH, angV) = (" << Deg(angH) << ", " << Deg(angV) << ")" << std::endl;
	Quaterniond ori = Quaterniond::Rot(-angV,'x')*Quaterniond::Rot(angH,'y');
	Posed pose = soEye->GetPose();
	pose.Ori() = ori;
	soEye->SetPose(pose);
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
