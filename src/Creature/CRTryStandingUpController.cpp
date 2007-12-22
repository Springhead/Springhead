#include "CRTryStandingUpController.h"

namespace Spr{;

IF_OBJECT_IMP(CRTryStandingUpController, CRController);
/*
Vec3d GetCenterOfMass(CRBodyIf* crBody){
	Vec3d center;

	return center;
}
*/

Vec3d GetFootPos(PHSolidIf* footSolid){
	Vec3d pos;
	//pos = standUpBody->GetCenterOfMass();
	return pos;
}

void TransitionPoseModel(CRBodyIf* crBody){
	
	;
}

void CRTryStandingUpController::Init(){	
	CRController::Init();

	//‘åˆæ•Ï”‚Ì‰Šú‰»
	totalStep = 0;
}

void CRTryStandingUpController::Step(){
	totalStep += 1;
	CRController::Step();
/*
	centerOfMass = GetCenterOfMass(crBody);
	rightFrontFootPos = GetFootPos(footSolid);
	rightRearFootPos  = GetFootPos(footSolid);
	leftFrontFootPos  = GetFootPos(footSolid);
	lefrRearFootPos	  = GetFootPos(footSolid);
*/
}

}