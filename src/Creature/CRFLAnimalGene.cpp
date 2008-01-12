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
	std::vector<CRFLAnimalGeneData> posture;
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
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				PHHingeJointDesc hingeDesc;
				body->GetJoint(i)->GetDesc(&hingeDesc);
				double goalD = hingeDesc.origin;

				CRFLAnimalGeneData gene;
				gene.goalDir[0] = goalD;
				gene.geneType   = CRFLAnimalGeneData::GEN_DOUBLE;
				posture.push_back(gene);
			}
		}
	}
	flAnimalGenes.push_back(posture);
	return flAnimalGenes.back();
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::GetGene(CRBodyIf* body){
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
			}
		}		
	}
	return flAnimalGenes.back();
}

void CRFLAnimalGene::SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body){
	flAnimalGenes.back() = gene;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
			}
		}	
	}
	
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB){
	std::vector<CRFLAnimalGeneData> mixedGene;
	srand((unsigned) time(NULL));	
	if(geneA.size() == geneB.size()){
		unsigned int changePoint = (unsigned int)(rand()%geneA.size() + 1);
		for(unsigned int i=0; i<geneA.size(); i++){
			if(i<changePoint){
				mixedGene.push_back(geneA[i]);
			} else{
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