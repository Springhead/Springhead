/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRReachingMovement.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRReachingMovement, SceneObject);

void CRReachingMovement::Init(){
	PHSceneIf* phScene = DCAST(PHSceneIf, GetScene());

	PHSolidDesc solidDesc;
	solidDesc.mass = 0.0001;
	soTarget = phScene->CreateSolid(solidDesc);
	soTarget->SetDynamical(false);

	PHSpringDesc springDesc;
	springDesc.posePlug.Pos() = Vec3d(0,-0.1,0);
	springDesc.spring = Vec3d(1,1,1) * springPos;
	springDesc.damper = Vec3d(1,1,1) * damperPos;
	spring = DCAST(PHSpringIf, phScene->CreateJoint(soTarget, solid, springDesc));

	spring->Enable(false);
}

void CRReachingMovement::Reset(){
	time  = 0.0f;
	limitForce = 350.0f;
	spring->Enable(false);
	bActive = false;
	bOri = false;
}

void CRReachingMovement::SetTarget(Vec3f p, Vec3f v, float t, float o){
	finalPos = p;
	finalVel = v;

	period   = time = t;
	offset   = o;
	spring->Enable(true);
	bActive  = true;
}

void CRReachingMovement::SetTarget(Vec3f p, Vec3f v, Quaterniond q, Vec3f av, float t, float o){
	finalQuat = q;
	finalAngV = av;
	bOri = true;
	SetTarget(p, v, t, o);
}

void CRReachingMovement::Step(){
	double dt = DCAST(PHSceneIf, GetScene())->GetTimeStep();
	if(bActive){
		/*
		if(bInitIfContact && (solid->GetForce().norm() > 30.0f)){	
			Reset();
		}
		*/

		if(time <= -offset){
			Reset();
			return;
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

		if (bOri) {
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

}
