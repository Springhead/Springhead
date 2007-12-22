#include "CRTryStandingUpController.h"

namespace Spr{;

IF_OBJECT_IMP(CRTryStandingUpController, CRController);

CRBodyIf* CRTryStandingUpController::GetBody(CRCreatureIf* creature){
	CRBodyIf* body;
	body = creature->GetBody(0);
	return body;
}

std::vector<PHSolidIf*> CRTryStandingUpController::SetFootSolid(CRBodyIf* body){
	std::vector<PHSolidIf*> foot;
	
	if(DCAST(CRHingeHumanBodyIf, body)!=NULL){
		foot.resize(sizeof(PHSolidIf*) +1);
		foot.back() = body->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_FOOT);
		foot.resize(sizeof(PHSolidIf*) +1);
		foot.back() = body->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_FOOT);
	
	}
	else if(DCAST(CRFourLegsAnimalBodyIf, body)!=NULL){
		foot.resize(sizeof(PHSolidIf*) +1);
		foot.back() = body->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_FRONT_TOE);
		foot.resize(sizeof(PHSolidIf*) +1);
		foot.back() = body->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_REAR_TOE);
		foot.resize(sizeof(PHSolidIf*) +1);
		foot.back() = body->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_FRONT_TOE);
		foot.resize(sizeof(PHSolidIf*) +1);
		foot.back() = body->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_REAR_TOE);
	}
	return foot;
}

Vec3d CRTryStandingUpController::GetFootPos(PHSolidIf* footSolid){
	Vec3d pos;
	pos = footSolid->GetPose();
	return pos;
}

void CRTryStandingUpController::TransitionPoseModel(CRBodyIf* crBody){
	
	;
}

void CRTryStandingUpController::Init(){	
	CRController::Init();

	//大域変数の初期化
	totalStep = 0;
}

void CRTryStandingUpController::Step(){	
	totalStep += 1;
	CRController::Step();

	// 各ボディの重心を出す
	for(int i=0; i<creature->NBodies(); i++){
	centerOfMass = creature->GetBody(i)->GetCenterOfMass();
	DSTR << centerOfMass << std::endl;

	rightFrontFootPos = GetFootPos(creature->GetBody(0)->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_FRONT_TOE));
	rightRearFootPos  = GetFootPos(creature->GetBody(0)->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_REAR_TOE));
	leftFrontFootPos  = GetFootPos(creature->GetBody(0)->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_FRONT_TOE));
	leftRearFootPos	  = GetFootPos(creature->GetBody(0)->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_REAR_TOE));

	}
}

}