/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRFLAnimalQL.h"
//-----------------------------------------------------------------------------------
//privateFunc:
void CRFLAnimalQL::BoltzmannSelection(){
	
}
void CRFLAnimalQL::EpsilonGreedySelection(){
	
}

void CRFLAnimalQL::SelectAction(){
	////////////////////////////////////
	//								  //
	// ボルツマン選択入れるんだっけ？ //
	//								  //
	////////////////////////////////////

	srand((unsigned) time(NULL));
	for(unsigned int i = 0; i < actionNumber.size(); i++){
		action[i] = rand()%actionNumber[i];
	}
}

void CRFLAnimalQL::TakeAction(std::vector<CRFLAnimalGeneData> aGene){
	for(unsigned int i = 0; i < aGene.size(); i++){
		if(aGene.back().geneType == CRFLAnimalGeneData::GEN_QUATERNIOND){
			DSTR << "BallJoint" << std::endl;
		}
		else if(aGene.back().geneType == CRFLAnimalGeneData::GEN_DOUBLE){
			DSTR << "HingeJoint" << std::endl;
		}
		else DSTR << "Unknown type." << std::endl;
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
	SelectAction();
	//TakeAction();
	UpdateQValues();
}


