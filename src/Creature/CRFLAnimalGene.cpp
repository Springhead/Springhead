/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRFLAnimalGene.h"

using namespace Spr;

void CRFLAnimalGene::Init(){
	for(int i=0; i<creature->NBodies(); i++){
		crBody.push_back(creature->GetBody(i));
	}
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::CreateGene(CRBodyIf* body){
//	DSTR << "---------make gene------------" << std::endl;
	
	std::vector<CRFLAnimalGeneData> posture;
	//DSTR << body->GetIfInfo()->ClassName() << "の遺伝子の関節総数 : " << body->NJoints() << std::endl;
	for(int i=0; i<body->NJoints(); i++){
		if(body->GetJoint(i)){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				PHBallJointDesc ballDesc;
				body->GetJoint(i)->GetDesc(&ballDesc);
				Quaterniond goalQ = ballDesc.goal;

				CRFLAnimalGeneData gene;
				gene.goalDir  = goalQ;
				gene.geneType = CRFLAnimalGeneData::GEN_QUATERNIOND;
				posture.push_back(gene);
				//DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				PHHingeJointDesc hingeDesc;
				body->GetJoint(i)->GetDesc(&hingeDesc);
				double goalD = hingeDesc.origin;

				CRFLAnimalGeneData gene;
				gene.goalDir[0] = goalD;
				gene.geneType   = CRFLAnimalGeneData::GEN_DOUBLE;
				posture.push_back(gene);
				//DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		//各試行でできたゴール方向の要素
		//DSTR << " " << body->GetJoint(i)->GetName() << " " << posture.back().goalDir << std::endl;
		}
	}
	flAnimalGenes.push_back(posture);
	
	// 出来上がった遺伝子をチェックする
	/*
	typedef std::vector< std::vector< CRFLAnimalGeneData > > TVV;
	typedef std::vector< CRFLAnimalGeneData > TV;
	for(TVV::iterator it = flAnimalGenes.begin(); it!=flAnimalGenes.end(); ++it){
		for(TV::iterator it2 = it->begin(); it2!=it->end(); ++it2){
			DSTR << it2->goalDir << std::endl;
		}
	}
	*/
	
	return flAnimalGenes.back();
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::GetGene(CRBodyIf* body){
	//DSTR << "---------gene info------------" << std::endl;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				//DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				//DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		//DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes.back().back().goalDir << std::endl;
		}		
	}
	return flAnimalGenes.back();
}

void CRFLAnimalGene::SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body){
	flAnimalGenes.back() = gene;
	
	//DSTR << "---------rewrite gene info------------" << std::endl;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				//DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				//DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		//DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes.back().back().goalDir << std::endl;
		}	
	}
	
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB){
	//DSTR << "--------mix two genes------------" << std::endl;
	std::vector<CRFLAnimalGeneData> mixedGene;
	srand((unsigned) time(NULL));	
	if(geneA.size() == geneB.size()){
		unsigned int changePoint = (unsigned int)(rand()%geneA.size() + 1);
		//DSTR << "changePoint : " << changePoint << std::endl;
		for(unsigned int i=0; i<geneA.size(); i++){
			if(i<changePoint){
				//DSTR << "a" << std::endl;
				mixedGene.push_back(geneA[i]);
			} else{
				//DSTR << "b" << std::endl;
				mixedGene.push_back(geneB[i]);
			}
		}
	}
	else {
		DSTR << "Two genes are different type." << std::endl;
		mixedGene = CreateGene(crBody[0]);
	}
	
	//出来上がった遺伝子をチェックする
	/*
	typedef std::vector< std::vector< CRFLAnimalGeneData > > TVV;
	typedef std::vector< CRFLAnimalGeneData > TV;
	for(TVV::iterator it = flAnimalGenes.begin(); it!=flAnimalGenes.end(); ++it){
		for(TV::iterator it2 = it->begin(); it2!=it->end(); ++it2){
			DSTR << it2->goalDir << std::endl;
		}
	}
	*/
	return mixedGene;
}