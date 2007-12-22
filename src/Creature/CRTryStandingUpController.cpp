#include "CRTryStandingUpController.h"

namespace Spr{;

IF_OBJECT_IMP(CRTryStandingUpController, CRController);

Vec3d GetFootPos(PHSolidIf* footSolid){
	Vec3d pos;
	return pos;
}

void TransitionPoseModel(CRBodyIf* crBody){
	
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

/*
	rightFrontFootPos = GetFootPos(footSolid);
	rightRearFootPos  = GetFootPos(footSolid);
	leftFrontFootPos  = GetFootPos(footSolid);
	lefrRearFootPos	  = GetFootPos(footSolid);
*/
	}
}

}