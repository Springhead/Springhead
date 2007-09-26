/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
IF_OBJECT_IMP(CRReachingController, CRController);

PHSolidIf* CRReachingController::GetSolid(){
	return solid;
}

void CRReachingController::Init(){
	CRController::Init();

	for (int i=0; i<creature->NBodies(); ++i) {
		CRHingeHumanBodyIf* body = DCAST(CRHingeHumanBodyIf, creature->GetBody(i));
		if (body) {
			if (solid==NULL && (0 <= solidNo && solidNo < CRHingeHumanBodyDesc::SO_NSOLIDS)) {
				solid = body->GetSolid(solidNo);
			}
		}
	}

	PHSolidDesc solidDesc;
	{
		solidDesc.mass = 1.0;
	}
	soTarget = phScene->CreateSolid(solidDesc);
	soTarget->SetDynamical(false);

	CDSphereDesc sphereDesc;
	{
		sphereDesc.radius = 0.02;
	}
	// soTarget->AddShape(phScene->GetSdk()->CreateShape(sphereDesc));
	phScene->SetContactMode(soTarget, PHSceneDesc::MODE_NONE);

	PHSpringDesc springDesc;
	{
		springDesc.posePlug.Pos() = Vec3d(0,0,0);
		springDesc.spring = Vec3d(1,1,1) * springPos;
		springDesc.damper = Vec3d(1,1,1) * damperPos;
	}
	spring = DCAST(PHSpringIf, phScene->CreateJoint(soTarget, solid, springDesc));
	spring->Enable(false);

	Reset();
}

void CRReachingController::SetTarget(Vec3f p, Vec3f v, float t, float o){
	firstPos = solid->GetPose().Pos();
	finalPos = p;
	finalVel = v;

	period   = time = t;
	offset   = o;
	spring->Enable(true);
	bActive  = true;
}

void CRReachingController::SetTarget(Vec3f p, Vec3f v, Quaterniond q, Vec3f av, float t, float o){
	finalQuat = q;
	finalAngV = av;
	bOri = true;
	SetTarget(p, v, t, o);
}

void CRReachingController::Step(){
	CRController::Step();

	double dt = phScene->GetTimeStep();
	if(bActive){
		/*
		if(bInitIfContact && (solid->GetForce().norm() > 30.0f)){	
			Reset();
		}
		*/

		if (offset < 0) {
			if (time < 0) {
				return;
			}
		} else {
			if (time <= -offset) {
				Reset();
				return;
			}
		}

		Vec3f pos, vel;
		//	³‹K‰»‚µ‚½Žž (0..1)
		float s = 1- time/period;
		//	•R‚Ì’·‚³‚Æ‘¬“x
		double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
		if (length<0){
			length = 0;
			deltaLength = 0;
		}
		Vec3f dir = solid->GetPose().Pos()-finalPos;
		//Vec3f dir = firstPos - finalPos;
		pos = finalPos + dir*length;
		vel = dir*deltaLength;
		soTarget->SetFramePosition(pos);
		soTarget->SetVelocity(vel);

		time -= dt;

		/*
		if (force.norm() >= limitForce){
			Reset();
		}
		*/

		if (/*/ false /*/ bOri /**/) {
			Quaterniond dQuat = finalQuat * solid->GetPose().Ori().Inv();
			if (dQuat.W() < 0){
				dQuat *= -1;
			}
			Vec3f dAngV = finalAngV - solid->GetAngularVelocity();
			Vec3f torque = springOri * dQuat.Rotation() + damperOri * dAngV;
			if (torque.norm() > 1e5f) {
				torque = torque.unit() * 1e5f;
			}
			solid->AddTorque(torque);
		}
	}
}

bool CRReachingController::IsMoving(){
	if (bActive) {
		if (offset < 0) {
			if (time < 0) {
				return false;
			}
		} else {
			if (time <= -offset) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool CRReachingController::IsReached(){
	return( (solid->GetPose().Pos() - finalPos).norm() < 0.2 );
}

bool CRReachingController::IsActive(){
	return bActive;
}

void CRReachingController::Reset(){
	time  = 0.0f;
	limitForce = 350.0f;
	spring->Enable(false);
	bActive = false;
	bOri = false;
}

}
