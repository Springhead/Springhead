/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <ctime>
#include <cmath>
#include "CRFLAnimalQL.h"
//-----------------------------------------------------------------------------------
//privateFunc:
void CRFLAnimalQL::BoltzmannSelection(){
	
}
void CRFLAnimalQL::EpsilonGreedySelection(){
	
}

void CRFLAnimalQL::SetActionNumber(std::vector<CRFLAnimalGeneData> aGene){
	actionNumber.resize(aGene.size());
	for(unsigned int i = 0; i < aGene.size(); i++){
		if(aGene[i].geneType == CRFLAnimalGeneData::GEN_QUATERNIOND)
			actionNumber[i] = 9;
		else if(aGene[i].geneType == CRFLAnimalGeneData::GEN_DOUBLE)
			actionNumber[i] = 3;
	}

}
void CRFLAnimalQL::SelectAction(){
	////////////////////////////////////
	//								  //
	// ボルツマン選択入れるんだっけ？ //
	//								  //
	////////////////////////////////////

	srand((unsigned) time(NULL));
	action.resize(actionNumber.size());
	for(unsigned int i = 0; i < crBody.size(); i++){
		for(unsigned int j = 0; j < actionNumber.size(); j++){
			//適当にアクションを決めているが、ここにボルツマン選択やε-greedy選択を挟み込む必要がある
			action[j] = rand()%actionNumber[j];
//			DSTR << action[j]  << std::endl;

			//////////////////////////////////////////////////////////////////////////
			//																		//
			// 行動が可動域制限の外に出てしまった場合の処理を考えないと発振する		//
			//																		//
			//////////////////////////////////////////////////////////////////////////

			// BallJointだった場合に取る行動
			if(thisTermGene[j].geneType == CRFLAnimalGeneData::GEN_QUATERNIOND){
				if(action[j] == 0){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad( 5), 'x') * Quaterniond::Rot(Rad(-5), 'y') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 1){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad(-5), 'y') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 2){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad(-5), 'x') * Quaterniond::Rot(Rad(-5), 'y') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 3){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad( 5), 'x') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 4){
//					DSTR << "The joint " << j << " is keep its position" << std::endl;
				}
				else if(action[j] == 5){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad(-5), 'x') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 6){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad( 5), 'x') * Quaterniond::Rot(Rad( 5), 'y') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 7){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad( 5), 'y') * thisTermGene[j].goalDir;
				}
				else if(action[j] == 8){
					thisTermGene[j].goalDir = Quaterniond::Rot(Rad(-5), 'x') * Quaterniond::Rot(Rad( 5), 'y') * thisTermGene[j].goalDir;
				}
				else{
				}
			}
			
			// HingeJointだった場合に取る行動
			else if(thisTermGene[j].geneType == CRFLAnimalGeneData::GEN_DOUBLE){
				if(action[j] == 0){
					thisTermGene[j].goalDir[0] += Rad(5);
				}
				else if(action[j] == 1){
//					DSTR << "The joint " << j << " is keep its position" << std::endl;
				}
				else if(action[j] == 2){
					thisTermGene[j].goalDir[0] -= Rad(5);
				}
				else{
				}
			}

		}
	}
/*
	for(unsigned int i = 0; i < actionNumber.size(); i++){
		DSTR << thisTermGene[i].goalDir << std::endl;
	}
*/
}

void CRFLAnimalQL::TakeAction(std::vector<CRFLAnimalGeneData> aGene){

	for(unsigned int i = 0; i < crBody.size(); i++){
//		DSTR << "size of aGene : " << aGene.size() << std::endl;
		for(unsigned int j = 0; j < aGene.size(); j++){
			if(aGene[j].geneType == CRFLAnimalGeneData::GEN_QUATERNIOND){
//				DSTR << "The joint " << j << " is BallJoint class" << std::endl;
				PHBallJointDesc ballDesc;
				crBody[i]->GetJoint(j)->GetDesc(&ballDesc);	
				ballDesc.goal = aGene[i].goalDir;
				crBody[i]->GetJoint(j)->SetDesc(&ballDesc);
			}
			else if(aGene[j].geneType == CRFLAnimalGeneData::GEN_DOUBLE){
//				DSTR << "The joint " << j << " is HingeJoint class" << std::endl;
				PHHingeJointDesc hingeDesc;
				crBody[i]->GetJoint(j)->GetDesc(&hingeDesc);
				hingeDesc.origin = aGene[j].goalDir[0];
				crBody[i]->GetJoint(j)->SetDesc(&hingeDesc);
			}
			else DSTR << "Unknown type." << std::endl;
		}
	}

}

void CRFLAnimalQL::EvaluativeFunc(){
	
}

void CRFLAnimalQL::CalcQValueMax(){

}

void CRFLAnimalQL::CalcQValueMin(){

}

void CRFLAnimalQL::UpdateQValues(){

}

void CRFLAnimalQL::StateValueMax(){

}

void CRFLAnimalQL::StateValueMin(){

}

void CRFLAnimalQL::CalcQValueDash(){

}

void CRFLAnimalQL::FitnessFromQValue(){

}

void CRFLAnimalQL::FitnessFromTimesOfUse(){

}

//-----------------------------------------------------------------------------------
//publicFunc:

void CRFLAnimalQL::Init(){
	for(int i=0; i<creature->NBodies(); i++){
		crBody.push_back(creature->GetBody(i));
	}
	learningRate = 0.2;
	decreaseRate = 0.8;
	penalty		 = -100.0;

}

void CRFLAnimalQL::Step(){
//	DSTR << "size of CRBody class : " << crBody.size() << std::endl;
	for(unsigned int i = 0; i < crBody.size(); i++){
		thisTermGene = CreateGene(crBody[i]);
	}
/*
	DSTR << "----------preAction----------" << std::endl;
	for(unsigned int i = 0; i < thisTermGene.size(); i++){
		DSTR << thisTermGene[i].goalDir << "  " << thisTermGene[i].geneType << std::endl;
	}
*/
	SetActionNumber(thisTermGene);
	SelectAction();
	
/*
	DSTR << "----------postAction----------" << std::endl;
	for(unsigned int i = 0; i < thisTermGene.size(); i++){
		DSTR << thisTermGene[i].goalDir << "  " << thisTermGene[i].geneType << std::endl;
	}
*/
	TakeAction(thisTermGene);
	UpdateQValues();
}


