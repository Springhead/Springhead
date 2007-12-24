#include "CRTryStandingUpController.h"

namespace Spr{;

IF_OBJECT_IMP(CRTryStandingUpController, CRController);

//------------------------------------------------------------------------------------------
// private Func:

std::vector<PHSolidIf*> CRTryStandingUpController::SetFootSolid(CRBodyIf* body){
	
	if(DCAST(CRHingeHumanBodyIf, body)!=NULL){
		foot.push_back(body->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_FOOT));	
		foot.push_back(body->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_FOOT));
	
	}
	else if(DCAST(CRFourLegsAnimalBodyIf, body)!=NULL){
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_FRONT_TOE));
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_REAR_TOE));
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_FRONT_TOE));
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_REAR_TOE));		
	}
	return foot;
}

Vec3d CRTryStandingUpController::GetFootPos(PHSolidIf* footSolid){
	Vec3d pos;
	pos = footSolid->GetPose().Pos();
	return pos;
}

void CRTryStandingUpController::TransitionPoseModel(CRFLAnimalGeneData gene){
	
}

void CRTryStandingUpController::UpdateBodyState(){
	// 各ボディの情報を更新する
	for(int i=0; i<creature->NBodies(); i++){
		if(body[i] != 0){
			centerOfMass = body[i]->GetCenterOfMass();
			//DSTR << centerOfMass << std::endl;

			SetFootSolid(body[i]);
			if(DCAST(CRHingeHumanBodyIf, body[i])!=NULL){
				rightFootPos = GetFootPos(foot[0]);
				leftFootPos	 = GetFootPos(foot[1]);
			}
			else if(DCAST(CRFourLegsAnimalBodyIf, body[i])!=NULL){
				rightFrontFootPos = GetFootPos(foot[0]);
				rightRearFootPos  = GetFootPos(foot[1]);
				leftFrontFootPos  = GetFootPos(foot[2]);
				leftRearFootPos	  = GetFootPos(foot[3]);
				//DSTR << "rightFrontFoot: " << rightFrontFootPos << "rightRearFoot: " << rightRearFootPos << "leftFrontFoot: "  << leftFrontFootPos  << "leftRearFoot: "  << leftRearFootPos << std::endl;
			}
		}
	}
}

//------------------------------------------------------------------------------------------
// public Func:
void CRTryStandingUpController::Init(){	
	CRController::Init();

//大域変数の初期化
	totalStep = 0;
	
	// body[i]:i体目のクリーチャーのボディになるように登録する
	for(int i=0; i<creature->NBodies(); i++){
		body.push_back(creature->GetBody(i));
		animalGene.push_back(DBG_NEW CRFLAnimalGene(creature));
		animalGeneData = animalGene[i]->CreateGene(body[i]);
	}

	
	
}

void CRTryStandingUpController::Step(){	
	totalStep += 1;
	CRController::Step();
	UpdateBodyState();

	for(int i=0; i<creature->NBodies(); i++){
		animalGeneData = animalGene[i]->CreateGene(body[i]);
	}
}

}