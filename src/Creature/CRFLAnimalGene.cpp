#include "CRFLAnimalGene.h"

using namespace Spr;

CRFLAnimalGene::CRFLAnimalGene(){
	for(int i=0; i<creature->NBodies(); i++){
		crBody.push_back(creature->GetBody(i));
	}

}
std::vector<CRFLAnimalGeneData> CRFLAnimalGene::CreateGene(CRBodyIf* body){
	DSTR << "---------make gene------------" << std::endl;
	for(int i=0; i<body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				PHBallJointDesc ballDesc;
				body->GetJoint(i)->GetDesc(&ballDesc);
				Quaterniond goalQ = ballDesc.goal;

				flAnimalGenes.resize(flAnimalGenes.size() + 1);
				flAnimalGenes.back().goalDir  = goalQ;
				flAnimalGenes.back().geneType = CRFLAnimalGeneData::GEN_QUATERNIOND;
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				PHHingeJointDesc hingeDesc;
				body->GetJoint(i)->GetDesc(&hingeDesc);
				double goalD = hingeDesc.origin;

				flAnimalGenes.resize(flAnimalGenes.size() +1);
				flAnimalGenes.back().goalDir[0] = goalD;
				flAnimalGenes.back().geneType   = CRFLAnimalGeneData::GEN_DOUBLE;

				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes.back().goalDir << std::endl;
		}		
	}	
	return flAnimalGenes;
}

std::vector<CRFLAnimalGeneData> CRFLAnimalGene::GetGene(CRBodyIf* body){
	DSTR << "---------gene info------------" << std::endl;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes[i].goalDir << std::endl;
		}		
	}
	return flAnimalGenes;
}

void CRFLAnimalGene::SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body){
	flAnimalGenes = gene;
	
	DSTR << "---------rewrite gene info------------" << std::endl;
	for(int i=0; i< body->NJoints(); i++){
		if(body->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, body->GetJoint(i))){
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, body->GetJoint(i))){
				DSTR << body->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << body->GetJoint(i)->GetName() << " " << flAnimalGenes[i].goalDir << std::endl;
		}	
	}
	
}

void CRFLAnimalGene::MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB){
	DSTR << "--------mix two genes------------" << std::endl;

}