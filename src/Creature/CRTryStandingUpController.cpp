#include "CRTryStandingUpController.h"

namespace Spr{;

IF_OBJECT_IMP(CRTryStandingUpController, CRController);

void CRTryStandingUpController::Init(){	
	CRController::Init();

	//‘åˆæ•Ï”‚Ì‰Šú‰»
	totalStep = 0;
}

void CRTryStandingUpController::Step(){
	totalStep += 1;
	CRController::Step();

	
}


}