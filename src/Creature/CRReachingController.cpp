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
}

void CRReachingController::Step(){
	CRController::Step();

	double dt = phScene->GetTimeStep();
	if(bActive){
		if (time < period) {
			time += (float)dt;
		}

		Vec3f pos, vel;
		//	³‹K‰»‚µ‚½Žž (0..1)
		float s = time/period;
		//	•R‚Ì’·‚³‚Æ‘¬“x
		double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
		if (length<0){
			length = 0;
			deltaLength = 0;
		}
		Vec3f dir = (ikcp->GetSolid()->GetPose() * ikcp->GetPos())-fP;
		pos = fP + dir*length;
		ikcp->SetGoal(pos);
	}
}

void CRReachingController::Start(Vec3d pos, Vec3d v, float t){
	this->time		= 0.0f;
	this->ikcp		= ikcp;
	this->fP		= pos;
	this->fV		= v;
	this->period	= t;

	ikcp->Enable(true);
	bActive = true;
}

void CRReachingController::Stop(){
	ikcp->Enable(false);
	bActive = false;
}

}
