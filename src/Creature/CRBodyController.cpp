/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyController.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCRCreature.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSdk.h>
#include <Collision/SprCDShape.h>
#include <Graphics/SprGRRender.h>

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRBodyController::Init(){
	// state = ObjectStatesIf::Create();
}

void CRBodyController::Step(){
	if (!soPoint) {
		PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
		PHSdkIf*   phSdk   = phScene->GetSdk();

		PHSolidDesc descSolid; descSolid.dynamical = false;
		soPoint = phScene->CreateSolid(descSolid);

		CDSphereDesc descSphere; descSphere.radius = 0.1;
		soPoint->AddShape(phSdk->CreateShape(descSphere));

		phScene->SetContactMode(soPoint, PHSceneDesc::MODE_NONE);
	}

	double dt = DCAST(CRCreatureIf,GetScene())->GetPHScene()->GetTimeStep();

	for (size_t i=0; i<targetValues.size(); ++i) {
		/*
		CRBCTargetValue* tv = targetValues[i];
		if (tv->bCtlPos) {
			tv->solid->GetIKEndEffector()->SetTargetPosition(tv->finalPos);
			soPoint->SetFramePosition(tv->finalPos);
		}
		*/
		CRBCTargetValue* tv = targetValues[i];
		// ˆÊ’u§Œä
		if (tv->bCtlPos && tv->bEnable) {
			tv->time += (float)dt;

			//	³‹K‰»‚µ‚½Žž (0..1)
			float s = std::min( tv->time / tv->timeLimit , 1.0f );

			//	•R‚Ì’·‚³‚Æ‘¬“x
			double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
			double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
			if (length<0) {
				length = 0;
				deltaLength = 0;
			}
			CRIKSolidIf* so = tv->solid;
			Vec3f dir = (so->GetPHSolid()->GetPose() * so->GetIKEndEffector()->GetTargetLocalPosition())-tv->finalPos;
			if (dir.norm() != 0) { dir /= dir.norm(); }
			Vec3d currPos = tv->finalPos + dir*(tv->finalPos - tv->initPos).norm()*length;
			so->GetIKEndEffector()->SetTargetPosition(currPos);
			soPoint->SetFramePosition(currPos);

			if (tv->time > tv->timeLimit) {
				tv->bCtlPos = false;
			}
		}

		// Žp¨§Œä
		if (tv->bCtlOri && tv->bEnable) {
			tv->bCtlOri = false; // –¢ŽÀ‘•
		}
	}
}

void CRBodyController::SetTargetPos(UTString effector, Vec3d pos) {
	CRBCTargetValue* tv = FindTargetValue(effector);
	if (tv) {
		tv->initPos  = tv->solid->GetPHSolid()->GetPose().Pos();
		tv->finalPos = pos;
		tv->bCtlPos  = true;
		tv->solid->GetIKEndEffector()->Enable(true);
		tv->solid->GetIKEndEffector()->EnablePositionControl(true);
	}
}

void CRBodyController::SetTargetOri(UTString effector, Quaterniond ori) {
	CRBCTargetValue* tv = FindTargetValue(effector);
	if (tv) {
		tv->finalOri = ori;
		tv->bCtlOri  = true;
		tv->solid->GetIKEndEffector()->Enable(true);
		tv->solid->GetIKEndEffector()->EnableOrientationControl(true);
	}
}

void CRBodyController::SetTargetPose(UTString effector, Posed pose) {
	CRBCTargetValue* tv = FindTargetValue(effector);
	if (tv) {
		tv->finalPos = pose.Pos();
		tv->bCtlPos  = true;
		tv->finalOri = pose.Ori();
		tv->bCtlOri  = true;
	}
}

void CRBodyController::SetTimeLimit(UTString effector, float timeLimit) {
	CRBCTargetValue* tv = FindTargetValue(effector);
	if (tv) {
		tv->timeLimit = timeLimit;
		tv->time = 0.0f;
	}
}

void CRBodyController::Plan() {
	// –¢ŽÀ‘•
}

void CRBodyController::Restart(UTString effector) {
	CRBCTargetValue* tv = FindTargetValue(effector);
	if (tv) {
		tv->time = 0.0f;
		tv->bEnable = true;
	}
}

CRBCTargetValue* CRBodyController::FindTargetValue(UTString label) {
	for (size_t i=0; i<targetValues.size(); ++i) {
		if (label == targetValues[i]->solid->GetLabel()) {
			return targetValues[i];
		}
	}

	CRIKSolidIf* so = body->FindByLabel(label)->Cast();
	if (so) {
		targetValues.push_back(new CRBCTargetValue());
		targetValues.back()->solid = so;
		return targetValues.back();
	}

	DSTR << "CRBodyController.cpp : FindTargetValue : Label [" << label.c_str() << "] was not found." << std::endl;;
	return NULL;
}

}
