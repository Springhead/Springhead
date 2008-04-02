/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRReachingController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRReachingController::Init(){
	CRController::Init();

	// 目標位置用剛体の作成
	{
		// 目標位置と制御対象剛体を直接結合する剛体
		PHSolidDesc solidDesc;
		solidDesc.mass = 1.0;
		soTargetDirect = phScene->CreateSolid(solidDesc);
		soTargetDirect->SetDynamical(false);
	}
	{
		// 目標位置と制御対象剛体を１軸関節を介して結合する剛体
		PHSolidDesc solidDesc;
		solidDesc.mass = 1.0;
		soTargetHinged = phScene->CreateSolid(solidDesc);
		PHHingeJointDesc jointDesc;
		{
			jointDesc.posePlug.Ori()   = fixOri;
			jointDesc.poseSocket.Ori() = fixOri;
		}
		phScene->CreateJoint(soTargetDirect, soTargetHinged, jointDesc);
	}

	// 目標位置用剛体と制御対象をバネで結合
	{
		PHSpringDesc springDesc;
		{
			springDesc.posePlug.Pos() = reachPos;
			springDesc.spring = Vec3d(1,1,1) * springPos;
			springDesc.damper = Vec3d(1,1,1) * damperPos;
			springDesc.springOri = 1000.0f;
			springDesc.damperOri =   50.0f;
			springDesc.bEnabled = false;
		}
		springDirect = DCAST(PHSpringIf, phScene->CreateJoint(soTargetDirect, solid, springDesc));
		springHinged = DCAST(PHSpringIf, phScene->CreateJoint(soTargetHinged, solid, springDesc));
	}

	Reset();
}

void CRReachingController::Step(){
	CRController::Step();

	double dt = phScene->GetTimeStep();
	if(bActive){
		if (period+(offset<0 ? 0 : offset) < time) {
			if (0 <= offset) {
				Reset();
			}
		} else {
			time += (float)dt;
		}

		Vec3f pos, vel;
		//	正規化した時刻 (0..1)
		float s = time/period;
		//	紐の長さと速度
		double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
		if (length<0){
			length = 0;
			deltaLength = 0;
		}
		Vec3f dir = solid->GetPose().Pos()-finalPos;
		pos = finalPos + dir*length;
		vel = dir*deltaLength;

		soTargetDirect->SetFramePosition(pos);
		soTargetDirect->SetVelocity(vel);
		soTargetDirect->SetOrientation(finalQuat);
	}
}

PHSolidIf* CRReachingController::GetSolid(){
	return solid;
}

void CRReachingController::SetTargetPos(Vec3f p, Vec3f v){
	finalPos = p;
	finalVel = v;
}

void CRReachingController::SetTargetOri(Quaterniond q, Vec3f av){
	finalQuat = q;
	finalAngV = av;
}

void CRReachingController::SetTargetTime(float t){
	period = t;
}

void CRReachingController::Start(CRReachingControllerIf::ConstraintMode mode, float keeptime){
	time     = 0.0f;
	offset   = keeptime;

	soTargetDirect->SetFramePosition(solid->GetPose().Pos());
	soTargetHinged->SetFramePosition(solid->GetPose().Pos());

	if (mode==CRReachingControllerIf::CM_P3R0) {
		springDirect->SetSpringOri(0);
		springDirect->SetDamperOri(0);
		springDirect->Enable(true);
		springHinged->Enable(false);

	} else if (mode==CRReachingControllerIf::CM_P3R2) {
		springDirect->Enable(false);
		springHinged->Enable(true);

	} else if (mode==CRReachingControllerIf::CM_P3R3) {
		springDirect->SetSpringOri(springOri);
		springDirect->SetDamperOri(damperOri);
		springDirect->Enable(true);
		springHinged->Enable(false);

	} else {
		return;

	}

	bActive = true;
}

float CRReachingController::GetRemainingTime(){
	float remaining = period - time;
	return((remaining > 0) ? remaining : 0);
}

CRReachingControllerIf::ReachState CRReachingController::GetReachState(){
	std::cout << ((solid->GetPose() * reachPos) - finalPos).norm() << std::endl;
	if (!bActive) {
		return CRReachingControllerIf::RS_STOP;
	} else if (time < period) {
		return CRReachingControllerIf::RS_NOTHING_REACHED;
	} else if (((solid->GetPose() * reachPos) - finalPos).norm() > 0.09) {
		return CRReachingControllerIf::RS_TARGET_REACHED;
	} else {
		return CRReachingControllerIf::RS_SOLID_REACHED;
	}
}

void CRReachingController::Reset(){
	bActive = false;
	time    = 0.0f;
	offset  = 0.0f;
	springDirect->Enable(false);
	springHinged->Enable(false);
}
}
