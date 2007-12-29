#include "CRTryStandingUpController.h"

namespace Spr{;

IF_OBJECT_IMP(CRTryStandingUpController, CRController);

//------------------------------------------------------------------------------------------
// private Func:

std::vector<PHSolidIf*> CRTryStandingUpController::SetFootSolid(CRBodyIf*	body){
	
	if(DCAST(CRHingeHumanBodyIf, body) != NULL){
		foot.push_back(body->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_FOOT));	
		foot.push_back(body->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_FOOT));
	
	}
	else if(DCAST(CRFourLegsAnimalBodyIf, body) != NULL){
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_FRONT_TOE));
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_RIGHT_REAR_TOE));
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_FRONT_TOE));
		foot.push_back(body->GetSolid(CRFourLegsAnimalBodyDesc::SO_LEFT_REAR_TOE));		
	}
	return foot;
}

Vec3d CRTryStandingUpController::GetFootPos(PHSolidIf*		footSolid){
	return		footSolid->GetPose().Pos();
}

Vec3d CRTryStandingUpController::CalcFootForce(PHSolidIf*	footSolid){
	Vec3d		force;
	Vec3d		torque;
	PHConstraintIf* localPair;
	
	localPair = phScene->GetConstraintEngine()->GetContactPoints()
					   ->FindBySolidPair(DCAST(PHSolidIf, (phScene->FindObject("Floor"))), footSolid);
	if(localPair)
		localPair->GetConstraintForce(force, torque);
	
	return		force;
}

void CRTryStandingUpController::TransitionPoseModel(std::vector<CRFLAnimalGeneData>		gene){
	for(unsigned int i = 0; i < gene.size(); i++){
		//BDSTR << gene[i].goalDir << std::endl;
		if(i == 0){
			PHBallJointDesc		ballDesc;
			body[0]->GetJoint(CRFourLegsAnimalBodyDesc::JO_WAIST_CHEST)->GetDesc(&ballDesc);
			ballDesc.goal		= gene[i].goalDir;
			body[0]->GetJoint(CRFourLegsAnimalBodyDesc::JO_WAIST_CHEST)->SetDesc(&ballDesc);
			Quaterniond		dd  = body[0]->GetJoint(CRFourLegsAnimalBodyDesc::JO_WAIST_CHEST)->GetRelativePoseQ();	
		}	
	}
}

void CRTryStandingUpController::UpdateBodyState(){
	// 各ボディの情報を更新する
	for(int i = 0; i < creature->NBodies(); i++){
		if(body[i] != 0){
			// 重心情報
			for(int j = 0; j < body[i]->NSolids(); j++){
				if(body[i]->GetSolid(j)){
					if(body[i]->GetSolid(j)->GetMass() != 0){
						centerOfMass = body[i]->GetCenterOfMass();
					}
				}
			}
			//DSTR << centerOfMass << std::endl;

			// ボディの足位置情報
			SetFootSolid(body[i]);
			if(DCAST(CRHingeHumanBodyIf, body[i]) != NULL){
				rightFootPos		= GetFootPos(foot[0]);
				leftFootPos			= GetFootPos(foot[1]);
				rightFootForce		= CalcFootForce(foot[0]);
				leftFootForce		= CalcFootForce(foot[1]);
			}
			else if(DCAST(CRFourLegsAnimalBodyIf, body[i]) != NULL){
				rightFrontFootPos	= GetFootPos(foot[0]);
				rightRearFootPos	= GetFootPos(foot[1]);
				leftFrontFootPos	= GetFootPos(foot[2]);
				leftRearFootPos		= GetFootPos(foot[3]);
				rightFrontFootForce = CalcFootForce(foot[0]);
				rightRearFootForce	= CalcFootForce(foot[1]);
				leftFrontFootForce	= CalcFootForce(foot[2]);
				leftRearFootForce	= CalcFootForce(foot[3]);
				//DSTR << "rightFrontFoot: " << rightFrontFootPos << "rightRearFoot: " << rightRearFootPos << "leftFrontFoot: "  << leftFrontFootPos  << "leftRearFoot: "  << leftRearFootPos << std::endl;
				DSTR << "rightFrontFoot: " << rightFrontFootForce << "rightRearFoot: " << rightRearFootForce << "leftFrontFoot: " << leftFrontFootForce << "leftRearFoot: " << leftRearFootForce << std::endl;
			}
		}
	}
}




//------------------------------------------------------------------------------------------
// public Func:
void CRTryStandingUpController::Init(){	
	CRController::Init();

//大域変数の初期化
	totalStep	 = 0;
	animalGeneIf = DBG_NEW CRFLAnimalGene(creature);			//< animalGeneの一番最後にクリーチャーの分だけ貼り付ける

	// body[i]:i体目のクリーチャーのボディになるように登録する
	for(int i = 0; i < creature->NBodies(); i++){
		body.push_back(creature->GetBody(i));							//< creatureの中にあるボディ情報を順番に格納していく
	}	
}

void CRTryStandingUpController::QLearning(){
	
	
	
}

void CRTryStandingUpController::Step(){	
	totalStep += 1;
	CRController::Step();
	UpdateBodyState();

//	for(int i = 0; i < creature->NBodies(); i++){
//		animalGenes.push_back(animalGeneIf->CreateGene(body[i]));		
//	}
//	DSTR << animalGenes.size() << std::endl;
	
	//毎ステップできてくる最新のanimalGenesを確認する
//	for(unsigned int i = 0; i < animalGenes.back().size(); i++){
//		DSTR << animalGenes.back()[i].goalDir << std::endl;
//	}	
//	std::vector<CRFLAnimalGeneData> gene = animalGeneIf->MixGenes(animalGeneIf->flAnimalGenes[0], animalGenes.back());
	
	/*
	for(int i=0; i<gene.size(); i++){
		DSTR << gene[i].goalDir << std::endl;
	}
	*/
//	TransitionPoseModel(gene);
	
}

}