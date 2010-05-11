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
	state = ObjectStatesIf::Create();
}

void CRReachingController::Step(){
	double dt = DCAST(CRCreatureIf,GetScene())->GetPHScene()->GetTimeStep();
	if(bActive){
		if (time < period) {
			time += (float)dt;
		}

		//	³‹K‰»‚µ‚½Žž (0..1)
		float s = time/period;
		//	•R‚Ì’·‚³‚Æ‘¬“x
		double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
		if (length<0){
			length = 0;
			deltaLength = 0;
		}
		Vec3f dir = (cSolid->GetPHSolid()->GetPose() * cSolid->GetIKEndEffector()->GetTargetLocalPosition())-fP;
		if (dir.norm() != 0) { dir /= dir.norm(); }
		pos = fP + dir*initLen*length;
		cSolid->GetIKEndEffector()->SetTargetPosition(pos);
	}
}

void CRReachingController::Render(GRRenderIf* render){
	render->PushModelMatrix();

	render->SetModelMatrix(Affinef::Trn(pos[0], pos[1], pos[2]));
	render->DrawSphere(0.1, 8, 8, false);

	render->SetModelMatrix(Affinef::Trn(fP[0], fP[1], fP[2]));
	render->DrawSphere(0.1, 8, 8, false);

	/*
	PHSolidIf *soChest = creature->GetBody(0)->GetSolid(CRBallHumanBodyDesc::SO_CHEST);
	PHBallJointDesc dB; creature->GetBody(0)->GetJoint(CRBallHumanBodyDesc::JO_CHEST_NECK)->GetDesc(&dB);
	Vec3d shoulderPos = soChest->GetPose() * dB.poseSocket.Pos();
	Posed shoulderPose = soChest->GetPose(); shoulderPose.Pos() = shoulderPos;

	Vec3d goalLocal = shoulderPose.Inv() * fP;
	if (goalLocal[0] > 0) { goalLocal[0] = 1; } else { goalLocal[0] = -1; }
	if (goalLocal[1] > 0) { goalLocal[1] = 1; } else { goalLocal[1] = -1; }
	if (goalLocal[2] > 0) { goalLocal[2] = 1; } else { goalLocal[2] = -1; }

	Vec3d SetTargetPosition = shoulderPose * goalLocal;
	render->SetModelMatrix(Affinef::Trn(SetTargetPosition[0], SetTargetPosition[1], SetTargetPosition[2]));
	render->DrawSphere(0.05, 8, 8, false);

	Affinef shoulderAf; shoulderPose.ToAffine(shoulderAf);
	render->SetModelMatrix(shoulderAf);
	Vec3f vtx[] = {
		Vec3f(-10,0,0), Vec3f(10,0,0),
		Vec3f(0,-10,0), Vec3f(0,10,0),
		Vec3f(0,0,-10), Vec3f(0,0,10),
	};
	render->DrawDirect(GRRenderBaseIf::LINES, vtx, 6);
	*/

	render->PopModelMatrix();
}

void CRReachingController::Start(Vec3d pos, Vec3d v, float t){
	this->fP		= pos;

	if (!bActive) {
		this->time		= 0.0f;
		this->fV		= v;
		this->period	= t;

		Vec3f dir = (cSolid->GetPHSolid()->GetPose() * cSolid->GetIKEndEffector()->GetTargetLocalPosition())-fP;
		initLen = dir.norm();

		cSolid->GetIKEndEffector()->Enable(true);
		cSolid->GetIKEndEffector()->EnablePositionControl(true);
		bActive = true;
	}

	/*
	typedef std::pair<PHBallJointIf*,Quaterniond> BallJointGoal;
	typedef std::vector<BallJointGoal> BallJointGoals;
	BallJointGoals ballGoals;

	typedef std::pair<PHHingeJointIf*,double> HingeJointGoal;
	typedef std::vector<HingeJointGoal> HingeJointGoals;
	HingeJointGoals hingeGoals;

	//// ÅIˆÊ’u‚É‚¨‚¯‚éŠÖßŠp“x‚ð‹‚ß‚é
	DCAST(PHIKPosCtlIf,cSolid->GetIKEndEffector(0))->SetTargetPosition(pos);
	CRCreatureIf* creature = DCAST(CRCreatureIf,DCAST(SceneObject,this)->GetScene());
	PHSceneIf* phScene = creature->GetPHScene();
	state->SaveState(phScene);
	double dt = phScene->GetTimeStep();
	phScene->SetTimeStep(dt * 10);
	for (int i=0; i<10; ++i) {
		phScene->Step();
	}
	CRBodyIf* body = creature->GetBody(0);
	for (size_t i=0; i<body->NJoints(); ++i) {
		PHBallJointIf* bj = body->GetJoint(i)->GetPHJoint()->Cast();
		if (bj) {
			BallJointGoal bjg;
			bjg.first  = bj;
			bjg.second = bj->GetTargetPosition();
			ballGoals.push_back(bjg);
		}

		PHHingeJointIf* hj = body->GetJoint(i)->GetPHJoint()->Cast();
		if (hj) {
			HingeJointGoal hjg;
			hjg.first  = hj;
			hjg.second = hj->GetTargetPosition();
			hingeGoals.push_back(hjg);
		}
	}
	state->LoadState(phScene);
	cSolid->GetIKEndEffector(0)->Enable(false);

	std::cout << " --- " << std::endl;
	for (size_t i=0; i<ballGoals.size(); ++i) {
		(ballGoals[i].first)->SetTargetPosition(ballGoals[i].second);
		std::cout << "Ball  : " << ballGoals[i].second << std::endl;
	}
	for (size_t i=0; i<hingeGoals.size(); ++i) {
		(hingeGoals[i].first)->SetTargetPosition(hingeGoals[i].second);
		std::cout << "Hinge : " << hingeGoals[i].second << std::endl;
	}
	std::cout << " --- " << std::endl;
	*/
}

void CRReachingController::Stop(){
	cSolid->GetIKEndEffector()->Enable(false);
	bActive = false;
}

}
