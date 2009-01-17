/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
Vec2d Deg(Vec2d v){
	return Vec2d(Deg(v[0]), Deg(v[1]));
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRSaccadeController::Reset(){
	angleLH = angleRH = angleV = 0.0;
}

void CRSaccadeController::Reset(double angleLH, double angleRH, double angleV) {
	Reset();
	this->angleLH = angleLH;
	this->angleRH = angleRH;
	this->angleV  = angleV;
}

void CRSaccadeController::StepHoriz(double destLH, double destRH, double angleHeadH, double dt){
	// (仮)
	angleLH = destLH;
	angleRH = destRH;
}

void CRSaccadeController::StepVert(double destV, double angleHeadV, double dt){
	// (仮)
	angleV = destV;
}

Vec2d CRSaccadeController::GetLEyeAngle(){
	return Vec2d(angleV, angleLH);
}

Vec2d CRSaccadeController::GetREyeAngle(){
	return Vec2d(angleV, angleRH);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRPursuitController::Reset(){
	intL = intR = 0.0;
	lastIL = lastIR = 0.0;
	angleLH = angleRH = angleV = 0.0;
}

void CRPursuitController::Reset(double angleLH, double angleRH, double angleV) {
	Reset();
	this->angleLH = angleLH;
	this->angleRH = angleRH;
	this->angleV  = angleV;
}

void CRPursuitController::StepHoriz(double destLH, double destRH, double angvelHeadH, double dt){
	// 誤差量と誤差変化量の計算
	double IL = destLH - angleLH;
	double IR = destRH - angleRH;
	double IVL = (IL - lastIL)/dt;
	double IVR = (IR - lastIR)/dt;
	lastIL = IL;
	lastIR = IR;

	// 積分
	intL += (dt * (-(S*IL + N*IVL + K*IR + H*IVR) + A1*angvelHeadH));
	intR += (dt * (+(S*IR + N*IVR + K*IL + H*IVL) - A1*angvelHeadH));

	// 角度指令値の計算
	angleLH += (-R1*intL + R2*intR);
	angleRH += (+R1*intR - R2*intL);
}

void CRPursuitController::StepVert(double destV, double angleHeadV, double dt){
	// (仮)
	angleV = destV;
}

Vec2d CRPursuitController::GetLEyeAngle(){
	return Vec2d(angleV, angleLH);
}

Vec2d CRPursuitController::GetREyeAngle(){
	return Vec2d(angleV, angleRH);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRPhysicalEye::Init(){
	for (int i=0; i<creature->NBodies(); ++i) {
		CRHingeHumanBodyIf* hingeBody = DCAST(CRHingeHumanBodyIf, creature->GetBody(i));
		if (hingeBody) {
			soLEye  = hingeBody->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_EYE);	
			soREye  = hingeBody->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_EYE);	
			soHead  = hingeBody->GetSolid(CRHingeHumanBodyDesc::SO_HEAD);	
			joLEyeX = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyDesc::JO_LEFT_EYE_X));
			joLEyeY = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyDesc::JO_LEFT_EYE_Y));	
			joREyeX = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyDesc::JO_RIGHT_EYE_X));	
			joREyeY = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyDesc::JO_RIGHT_EYE_Y));	
		}
		CRBallHumanBodyIf* ballBody = DCAST(CRBallHumanBodyIf, creature->GetBody(i));
		if (ballBody) {
			soLEye  = ballBody->GetSolid(CRBallHumanBodyDesc::SO_LEFT_EYE);	
			soREye  = ballBody->GetSolid(CRBallHumanBodyDesc::SO_RIGHT_EYE);	
			soHead  = ballBody->GetSolid(CRBallHumanBodyDesc::SO_HEAD);	
			joLEye = DCAST(PHBallJointIf, ballBody->GetJoint(CRBallHumanBodyDesc::JO_LEFT_EYE));
			joREye = DCAST(PHBallJointIf, ballBody->GetJoint(CRBallHumanBodyDesc::JO_RIGHT_EYE));	
		}
	}
}

void CRPhysicalEye::SetTarget(Vec3d pos, Vec3d vel){
	targetPos = pos;
	targetVel = vel;
}

void CRPhysicalEye::Control(Vec2d angleLEye, Vec2d angleREye){
	if (joLEyeX && joLEyeY && joREyeX && joREyeY) {
		Control(joLEyeX, joLEyeY, angleLEye);
		Control(joREyeX, joREyeY, angleREye);
	} else if (joLEye && joREye) {
		Control(joLEye, angleLEye);
		Control(joREye, angleREye);
	}
}

Vec2d CRPhysicalEye::GetAxisL(){
	Quaterniond qToLocal = soHead->GetPose().Ori().Inv();
	Vec3d axisVec = qToLocal * soLEye->GetPose().Ori() * Vec3d(0,0,-1);
	Vec2d angle   = Vec3ToAngle(axisVec);
	return angle;
}

Vec2d CRPhysicalEye::GetAxisR(){
	Quaterniond qToLocal = soHead->GetPose().Ori().Inv();
	Vec3d axisVec = qToLocal * soREye->GetPose().Ori() * Vec3d(0,0,-1);
	Vec2d angle   = Vec3ToAngle(axisVec);
	return angle;
}

Vec2d CRPhysicalEye::GetTargetFromL(){
	Quaterniond qToLocal = soHead->GetPose().Ori().Inv();
	Vec3d target = qToLocal * (targetPos - soLEye->GetPose().Pos());
	Vec2d angle  = Vec3ToAngle(target);
	return angle;
}

Vec2d CRPhysicalEye::GetTargetFromR(){
	Quaterniond qToLocal = soHead->GetPose().Ori().Inv();
	Vec3d target = qToLocal * (targetPos - soREye->GetPose().Pos());
	Vec2d angle  = Vec3ToAngle(target);
	return angle;
}

Vec2d CRPhysicalEye::GetHeadAngle(){
	Vec3d axisVec = soHead->GetPose().Ori() * Vec3d(0,0,-1);
	Vec2d angle   = Vec3ToAngle(axisVec);
	return angle;
}

Vec2d CRPhysicalEye::GetHeadAngvel(){
	Vec3d angvel = soHead->GetPose().Ori().Inv() * soHead->GetAngularVelocity();
	return Vec2d(angvel[0], angvel[1]);
}

void CRPhysicalEye::Control(PHHingeJointIf* joX, PHHingeJointIf* joY, Vec2d angle){
	double lower, upper;
	if (joLEyeX->GetChildObject(1)==soLEye) {
		joX->GetRange(lower, upper);
		if (lower < upper) {
			if (angle[0] < lower) {
				joX->SetSpringOrigin(lower);
			} else if (upper < angle[0]) {
				joX->SetSpringOrigin(upper);
			} else {
				joX->SetSpringOrigin(angle[0]);
			}
		} else {
			joX->SetSpringOrigin(angle[0]);
		}

		joY->GetRange(lower, upper);
		if (lower < upper) {
			if (angle[1] < lower) {
				joY->SetSpringOrigin(lower);
			} else if (upper < angle[1]) {
				joY->SetSpringOrigin(upper);
			} else {
				joY->SetSpringOrigin(angle[1]);
			}
		} else {
			joY->SetSpringOrigin(angle[1]);
		}
	} else {
		joX->GetRange(lower, upper);
		if (lower < upper) {
			if (-angle[0] < lower) {
				joX->SetSpringOrigin(lower);
			} else if (upper < -angle[0]) {
				joX->SetSpringOrigin(upper);
			} else {
				joX->SetSpringOrigin(-angle[0]);
			}
		} else {
			joX->SetSpringOrigin(angle[0]);
		}

		joY->GetRange(lower, upper);
		if (lower < upper) {
			if (-angle[1] < lower) {
				joY->SetSpringOrigin(lower);
			} else if (upper < -angle[1]) {
				joY->SetSpringOrigin(upper);
			} else {
				joY->SetSpringOrigin(-angle[1]);
			}
		} else {
			joY->SetSpringOrigin(angle[1]);
		}
	}
}

void CRPhysicalEye::Control(PHBallJointIf* jo, Vec2d angle){
	// std::cout << Deg(angle[0]) << ", " << Deg(angle[1]) << std::endl;
	Quaterniond q = Quaterniond::Rot(angle[1],'y') * Quaterniond::Rot(angle[0],'x');
	jo->SetGoal(q);
}

Vec2d CRPhysicalEye::Vec3ToAngle(Vec3d v){
	double D1 = sqrt(v.Y()*v.Y()+v.Z()*v.Z());
	double D2 = sqrt(v.X()*v.X()+v.Z()*v.Z());
	return(Vec2d( atan2( v.Y()/D1, -v.Z()/D1), atan2(-v.X()/D2, -v.Z()/D2) ));
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CREyeController::LookAt(Vec3f pos, Vec3f vel){
	physicalEye.SetTarget(pos, vel);
}

void CREyeController::Init(){
	CRController::Init();

	physicalEye.Init();
}

void CREyeController::Step(){
	CRController::Step();

	// 制御状態の遷移
	controlState = GetNextState(controlState);

	// 実際の眼球運動制御の実行
	Control();
}

CREyeControllerState::ControlState CREyeController::GetNextState(ControlState currentCS){
	Vec2d eL = physicalEye.GetTargetFromL() - physicalEye.GetAxisL();
	Vec2d eR = physicalEye.GetTargetFromR() - physicalEye.GetAxisR();

	// 条件判定と次の制御状態の決定
	switch(controlState){
	case CS_SACCADE:
		// サッケード完了条件
		if ((abs(eL[1]) < Rad(5.0f)) && (abs(eR[1])  < Rad(5.0f)) && (abs(eL[0]) < Rad(5.0f)) && (abs(eR[0]) < Rad(5.0f))) {
			return CS_PURSUIT;
		}else{
			return CS_SACCADE;
		}
		break;

	case CS_PURSUIT:
		// パーシュート中断条件
		if ((abs(eL[1]) > Rad(10.0f)) || (abs(eR[1])  > Rad(10.0f)) || (abs(eL[0]) > Rad(10.0f)) || (abs(eR[0]) > Rad(10.0f))) {
			return CS_SACCADE;
		}else{
			return CS_PURSUIT;
		}
		break;

	default:
		break;
	}

	// ここにはこないはずだけど
	return CS_SACCADE;
}

void CREyeController::Control(){
	double dt = DCAST(PHSceneIf, GetScene())->GetTimeStep() / 10.0;

	switch(controlState){
	case CS_SACCADE: 
		saccadeCtrl.StepHoriz(
			physicalEye.GetTargetFromL()[1], physicalEye.GetTargetFromR()[1],
			physicalEye.GetHeadAngle()[1],
			dt);
		saccadeCtrl.StepVert(
			physicalEye.GetTargetFromL()[0], physicalEye.GetHeadAngle()[0],
			dt);
		physicalEye.Control(saccadeCtrl.GetLEyeAngle(), saccadeCtrl.GetREyeAngle());
		// 次回Pursuitに切り替わったときのためリセットしておく
		pursuitCtrl.Reset(physicalEye.GetAxisL()[1], physicalEye.GetAxisR()[1], physicalEye.GetAxisL()[0]);
		break;
	
	case CS_PURSUIT:
		pursuitCtrl.StepHoriz(
			physicalEye.GetTargetFromL()[1], physicalEye.GetTargetFromR()[1],
			physicalEye.GetHeadAngvel()[1],
			dt);
		pursuitCtrl.StepVert(
			physicalEye.GetTargetFromL()[0], physicalEye.GetHeadAngle()[0],
			dt);
		physicalEye.Control(pursuitCtrl.GetLEyeAngle(), pursuitCtrl.GetREyeAngle());
		// 次回Saccadeに切り替わったときのためリセットしておく
		saccadeCtrl.Reset(physicalEye.GetAxisL()[1], physicalEye.GetAxisR()[1], physicalEye.GetAxisL()[0]);
		break;

	default:
		break;
	}
}

bool CREyeController::IsSaccading(){
	return controlState==CS_SACCADE;
}

CREyeControllerState::ControlState CREyeController::GetControlState(){
	return controlState;
}
}
