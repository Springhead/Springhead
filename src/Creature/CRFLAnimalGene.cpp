#include "CRFLAnimalGene.h"

using namespace Spr;

void CRFLAnimalGene::Init(){
	for(int i=0; i<creature->NBodies(); i++){
		crBody.push_back(creature->GetBody(i));
	}
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::CreateGene(CRBodyIf* body){
	DSTR << "---------make gene------------" << std::endl;
	for(int i=0; i<body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			flAnimalGenes.resize(flAnimalGenes.size()+1);
			flAnimalGenes.back().resize(flAnimalGenes.back().size() +1);
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				PHBallJointDesc ballDesc;
				body->GetJoint(i)->GetDesc(&ballDesc);
				Quaterniond goalQ = ballDesc.goal;

				flAnimalGenes.back().back().goalDir  = goalQ;
				flAnimalGenes.back().back().geneType = CRFLAnimalGeneData::GEN_QUATERNIOND;
				DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				PHHingeJointDesc hingeDesc;
				body->GetJoint(i)->GetDesc(&hingeDesc);
				double goalD = hingeDesc.origin;

				flAnimalGenes.back().back().goalDir[0] = goalD;
				flAnimalGenes.back().back().geneType   = CRFLAnimalGeneData::GEN_DOUBLE;

				DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes.back().back().goalDir << std::endl;
		}		
	}	
	return flAnimalGenes.back();
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::GetGene(CRBodyIf* body){
	DSTR << "---------gene info------------" << std::endl;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				DSTR << "joint: " << i << " " << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes.back().back().goalDir << std::endl;
		}		
	}
	return flAnimalGenes.back();
}

void CRFLAnimalGene::SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body){
	flAnimalGenes.back() = gene;
	
	DSTR << "---------rewrite gene info------------" << std::endl;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes.back().back().goalDir << std::endl;
		}	
	}
	
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB){
	DSTR << "--------mix two genes------------" << std::endl;
	std::vector<CRFLAnimalGeneData> mixedGene;
	srand((unsigned) time(NULL));	
	unsigned int changePoint = abs(rand()%geneA.size() + 1);
	for(unsigned int i=0; i<geneA.size(); i++){
		if(i<changePoint){
			mixedGene[i] = geneA[i];
		} else{
			mixedGene[i] = geneB[i];
		}
	}
	
	return mixedGene;
}