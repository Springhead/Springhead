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
		actions[i] = rand()%actionNumber[i];
	}
}

void CRFLAnimalQL::TakeAction(){
	
}

void CRFLAnimalQL::EvaluativeFunc(){
	
}

void CRFLAnimalQL::UpdateQValues(){

}

//-----------------------------------------------------------------------------------
//publicFunc:

void CRFLAnimalQL::Init(){
	for(int i=0; i<creature->NBodies(); i++){
		crBody.push_back(creature->GetBody(i));
	}
	learningRate = 0.2;
	decreaseRate = 0.8;
	
}

void CRFLAnimalQL::Step(){
	SelectAction();
	TakeAction();
	UpdateQValues();
}


