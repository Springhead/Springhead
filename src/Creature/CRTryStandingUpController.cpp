/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
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
	return			foot;
}

Vec3d CRTryStandingUpController::GetFootPos(PHSolidIf*		footSolid){
	return		footSolid->GetPose().Pos();
}

Vec3d CRTryStandingUpController::CalcFootForce(PHSolidIf*	footSolid){
	Vec3d				force;
	Vec3d				torque;
	PHConstraintIf*		localPair = phScene->GetConstraintEngine()->GetContactPoints()
										   ->FindBySolidPair(DCAST(PHSolidIf, (phScene->FindObject("Floor"))), footSolid);
	if(localPair)
		localPair->GetConstraintForce(force, torque);
	
	return				force;
}

Vec3d CRTryStandingUpController::CalcFootTorque(PHSolidIf* footSolid){
	Vec3d				force;
	Vec3d				torque;
	PHConstraintIf*		localPair = phScene->GetConstraintEngine()->GetContactPoints()
										   ->FindBySolidPair(DCAST(PHSolidIf, (phScene->FindObject("Floor"))), footSolid);
	if(localPair)
		localPair->GetConstraintForce(force, torque);
	
	return				torque;
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
				//DSTR << "rightFrontFoot: " << rightFrontFootForce << "rightRearFoot: " << rightRearFootForce << "leftFrontFoot: " << leftFrontFootForce << "leftRearFoot: " << leftRearFootForce << std::endl;
			}
		}
	}
}

//------------------------------------------------------------------------------------------
// public Func:
void CRTryStandingUpController::Init(){	
	CRController::Init();

//大域変数の初期化
	totalStep		= 0;
	qLearningStep	= 0;
	animalGeneIf = DBG_NEW CRFLAnimalGene(creature);			//< animalGene型のインスタンスを生成
	animalQLIf	 = DBG_NEW CRFLAnimalQL(creature);				//< animalQL型のインスタンスを生成
	// body[i]:i体目のクリーチャーのボディになるように登録する
	for(int i = 0; i < creature->NBodies(); i++){
		body.push_back(creature->GetBody(i));							//< creatureの中にあるボディ情報を順番に格納していく
	}
}

void CRTryStandingUpController::CalcQL(){
	
	DSTR << "QL : " << qLearningStep << std::endl;
	animalQLIf->SetActionNumber(animalGenes.back());
	animalQLIf->SelectAction(animalGenes.back());
	animalQLIf->TakeAction(animalGenes.back());
	animalQLIf->UpdateQValues();	
}

void CRTryStandingUpController::CalcGA(){

	DSTR << "GA" << std::endl;
	
	//DSTR << "今までに作成された遺伝子系列の数 : "<< animalGenes.size() << std::endl;
	
	//毎ステップできてくる最新のanimalGenesを確認する
/*	
	for(unsigned int i = 0; i < animalGenes.back().size(); i++){
		DSTR << animalGenes.back()[i].goalDir << std::endl;
	}	
*/
	std::vector<CRFLAnimalGeneData> gene = animalGeneIf->MixGenes(animalGeneIf->flAnimalGenes[0], animalGenes.back());
	
/*
	for(int i=0; i<gene.size(); i++){
		DSTR << gene[i].goalDir << std::endl;
	}
*/

//	TransitionPoseModel(gene);
 
}

void CRTryStandingUpController::Step(){	
	totalStep	  += 1;
	CRController::Step();
	UpdateBodyState();	
	//最初のポーズが安定するまで待たないといきなり発振する。
	//大体totalStep == 200位まで待てばOK。
	if(totalStep <=  200){
		DSTR << "totalStep : " << totalStep << std::endl;
	}
	else if(totalStep > 200){
		qLearningStep += 1;
		for(int i = 0; i < creature->NBodies(); i++){
		animalGenes.push_back(animalGeneIf->CreateGene(body[i]));		
		}
		//QLearningのみを50回行う
		CalcQL();

		//50回QLearningを行った後、1回だけGAを行う
		if(qLearningStep == 50){
			CalcGA();
			qLearningStep = 0;
		}
	}
}

}