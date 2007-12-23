#include "CRFLAnimalGene.h"

using namespace Spr;

extern CRFourLegsAnimalBodyIf* animalBody;

CRFLAnimalGene::CRFLAnimalGene(){
}
void CRFLAnimalGene::CreateGene(){
	DSTR << "---------make gene------------" << std::endl;
	for(int i=0; i< animalBody->NJoints(); i++){
		if(animalBody->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, animalBody->GetJoint(i))){
				PHBallJointDesc ballDesc;
				animalBody->GetJoint(i)->GetDesc(&ballDesc);
				Quaterniond goalQ = ballDesc.goal;

				flAnimalGenes.resize(flAnimalGenes.size() + 1);
				flAnimalGenes.back().goalDir  = goalQ;
				flAnimalGenes.back().geneType	= GeneData::GEN_QUATERNIOND;
				DSTR << animalBody->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, animalBody->GetJoint(i))){
				PHHingeJointDesc hingeDesc;
				animalBody->GetJoint(i)->GetDesc(&hingeDesc);
				double goalD = hingeDesc.origin;

				flAnimalGenes.resize(flAnimalGenes.size() +1);
				flAnimalGenes.back().goalDir[0] = goalD;
				flAnimalGenes.back().geneType   = GeneData::GEN_DOUBLE;

				DSTR << animalBody->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << animalBody->GetJoint(i)->GetName() << " " << flAnimalGenes.back().goalDir << std::endl;
		}		
	}
}

std::vector<GeneData> CRFLAnimalGene::GetGene(){
	DSTR << "---------gene info------------" << std::endl;
	for(int i=0; i< animalBody->NJoints(); i++){
		if(animalBody->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, animalBody->GetJoint(i))){
				DSTR << animalBody->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, animalBody->GetJoint(i))){
				DSTR << animalBody->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << animalBody->GetJoint(i)->GetName() << " " << flAnimalGenes[i].goalDir << std::endl;
		}		
	}
	return flAnimalGenes;
}

void CRFLAnimalGene::SetGene(std::vector<GeneData> gene){
	flAnimalGenes = gene;
	
	DSTR << "---------rewrite gene info------------" << std::endl;
	for(int i=0; i< animalBody->NJoints(); i++){
		if(animalBody->GetJoint(i) != NULL){
			if(DCAST(PHBallJointIf, animalBody->GetJoint(i))){
				DSTR << animalBody->GetJoint(i)->GetIfInfo()->ClassName();
			}
			else if(DCAST(PHHingeJointIf, animalBody->GetJoint(i))){
				DSTR << animalBody->GetJoint(i)->GetIfInfo()->ClassName();
			}
		DSTR << " " << animalBody->GetJoint(i)->GetName() << " " << flAnimalGenes[i].goalDir << std::endl;
		}	
	}
	
}

void CRFLAnimalGene::MixGenes(std::vector<GeneData> geneA, std::vector<GeneData> geneB){
	DSTR << "--------mix two genes------------" << std::endl;

}