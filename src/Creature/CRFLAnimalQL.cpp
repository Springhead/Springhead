/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRFLAnimalQL.h"
//-----------------------------------------------------------------------------------
//publicFunc:
void CRFLAnimalQL::Init(){
	for(int i=0; i<creature->NBodies(); i++){
		crBody.push_back(creature->GetBody(i));
	}
	learningRate =	  0.2;
	decreaseRate =    0.8;
	penalty		 = -100.0;
	moveRatio	 =	  0.1;

}

void CRFLAnimalQL::BoltzmannSelection(){
	
	//////////////////////////////////////////////////////////////
	//															//
	// どういう時にエネルギーが低いか考える						//
	//															//
	//////////////////////////////////////////////////////////////

}
void CRFLAnimalQL::EpsilonGreedySelection(){
	
}

void CRFLAnimalQL::SetActionNumber(std::vector<CRFLAnimalGeneData> *aGene){
	actionNumber.resize((*aGene).size());
	for(unsigned int i = 0; i < (*aGene).size(); i++){
		if((*aGene)[i].geneType == CRFLAnimalGeneData::GEN_QUATERNIOND)
			actionNumber[i] = 9;
		else if((*aGene)[i].geneType == CRFLAnimalGeneData::GEN_DOUBLE)
			actionNumber[i] = 3;
	}

}
void CRFLAnimalQL::SelectAction(std::vector<CRFLAnimalGeneData> *aGene){

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
			if((*aGene)[j].geneType == CRFLAnimalGeneData::GEN_QUATERNIOND){
				if(action[j] == 0){
					// Quaternionの計算は左から順番に考える
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad( moveRatio), 'x') * Quaterniond::Rot(Rad(-moveRatio), 'y') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 1){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad(-moveRatio), 'y') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 2){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad(-moveRatio), 'x') * Quaterniond::Rot(Rad(-moveRatio), 'y') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 3){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad( moveRatio), 'x') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 4){
//					DSTR << "The joint " << j << " is keep its position" << std::endl;
				}
				else if(action[j] == 5){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad(-moveRatio), 'x') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 6){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad( moveRatio), 'x') * Quaterniond::Rot(Rad( moveRatio), 'y') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 7){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad( moveRatio), 'y') * (*aGene)[j].goalDir;
				}
				else if(action[j] == 8){
					(*aGene)[j].goalDir = Quaterniond::Rot(Rad(-moveRatio), 'x') * Quaterniond::Rot(Rad( moveRatio), 'y') * (*aGene)[j].goalDir;
				}
				else{
				}
			}
			
			// HingeJointだった場合に取る行動
			else if((*aGene)[j].geneType == CRFLAnimalGeneData::GEN_DOUBLE){
				if(action[j] == 0){
					//if(crBody[0]->GetJoint(j)->
					(*aGene)[j].goalDir[0] += Rad(moveRatio);
				}
				else if(action[j] == 1){
//					DSTR << "The joint " << j << " is keep its position" << std::endl;
				}
				else if(action[j] == 2){
					(*aGene)[j].goalDir[0] -= Rad(moveRatio);
				}
				else{
				}
			}

		}
	}
/*
	for(unsigned int i = 0; i < actionNumber.size(); i++){
		DSTR << (*aGene)[i].goalDir << std::endl;
	}
*/
}

void CRFLAnimalQL::TakeAction(std::vector<CRFLAnimalGeneData> *aGene){

	for(unsigned int i = 0; i < crBody.size(); i++){
//		DSTR << "crBody.size() : " << crBody.size() << std::endl;
		for(unsigned int j = 0; j < (*aGene).size(); j++){
			if((*aGene)[j].geneType == CRFLAnimalGeneData::GEN_QUATERNIOND){
				PHBallJointDesc ballDesc;
				crBody[i]->GetJoint(j)->GetDesc(&ballDesc);
				ballDesc.goal = (*aGene)[j].goalDir;
				crBody[i]->GetJoint(j)->SetDesc(&ballDesc);
			}
			else if((*aGene)[j].geneType == CRFLAnimalGeneData::GEN_DOUBLE){
				PHHingeJointDesc hingeDesc;
				crBody[i]->GetJoint(j)->GetDesc(&hingeDesc);
				hingeDesc.origin = (*aGene)[j].goalDir[0];
				crBody[i]->GetJoint(j)->SetDesc(&hingeDesc);
			}
			else DSTR << "Unknown type." << std::endl;
		}
	}

}

void CRFLAnimalQL::EvaluativeFunc(Vec3d centerOfMass, 
								  std::vector<Vec3d>footPositions, 
								  std::vector<Vec3d> footForces){

	if(footPositions.size() == 2){		//< 渡されてきたデータは人型のものだ
		
		if(crBody[0]->GetSolid(CRHingeHumanBody::SO_HEAD)){
			DSTR << "human body is not up to date." << std::endl;
		}
	}

	else if(footPositions.size() == 4){		//< 渡されてきたデータは動物型のものだ

		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//[check 1]上体の向きはどうなっているの？
		if(crBody[0]->GetSolid(CRFourLegsAnimalBody::SO_HEAD) && crBody[0]->GetSolid(CRFourLegsAnimalBody::SO_CHEST)){
			Vec3d		 headPos	=	crBody[0]->GetSolid(CRFourLegsAnimalBody::SO_HEAD)->GetPoseR();
			Vec3d		 chestPos	=	crBody[0]->GetSolid(CRFourLegsAnimalBody::SO_CHEST)->GetPoseR();
			Quaterniond  chestOri	=   crBody[0]->GetSolid(CRFourLegsAnimalBody::SO_CHEST)->GetPoseQ();

			// vEvaluateHC  : 頭から胸へ伸びたベクトルの背骨方向への法線ベクトル（評価ベクトル）, localTheta  : 背骨と頭胸ベクトルのなす角
			//【問題点】	: きちんと単位ベクトルにしてから内積をとっているのに1.0を超えてしまうことがある。なぜだろう。
			double localTheta	=	acos(min (dot((chestPos - headPos).unit(), (chestOri * chestPos).unit() ), 1.0));
			vEvaluateHC			=	(chestPos - headPos) * sin(localTheta);
			//DSTR << localTheta * 180 / M_PI << std::endl;
		}
		//thetaHC : 評価ベクトルと重力方向のなす角
		double thetaHC = acos(min(dot(vEvaluateHC, Vec3d(0.0, -1.0, 0.0)), 1.0));
		//DSTR << "HC: " << vEvaluateHC << " theta: " << thetaHC *180/3.14 << " " << dot(vEvaluateHC, Vec3d(0, -1, 0)) << std::endl;

		//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// [check 2]重心は支持脚多角形の中にあるの？
		// footSolid[0] : rightFront, footSolid[1] : rightRear, footSolid[2] : leftFront, footSolid[3] : leftRear
		// <<支持脚多角形その1>>
		Vec3d normalLine = cross((footPositions[0] - footPositions[1]), (footPositions[0] - footPositions[2]));

		//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		// [check 3]各関節がボディの出せる最大トルク内にあるかを確認する
		for(int i = 0; i < crBody[0]->NJoints(); i++){
			if(DCAST(PHBallJointIf, crBody[0]->GetJoint(i)))
				DCAST(PHBallJointIf, crBody[0]->GetJoint(i))->GetTorqueMax();
			else if(DCAST(PHHingeJointIf, crBody[0]->GetJoint(i)))
				DCAST(PHHingeJointIf, crBody[0]->GetJoint(i))->GetTorqueMax();
		}

	}

	


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

