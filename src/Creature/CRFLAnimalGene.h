#ifndef FLANIMALGENE_H
#define FLANIMALGENE_H

#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>
#include <vector>

#include "CRController.h"
#include "CRFLAnimalGeneData.h"

class CRFLAnimalGene: public CRController{

private:
	std::vector<CRFLAnimalGeneData> flAnimalGenes;
	std::vector<CRBodyIf*>		    crBody;
public:
	
	std::vector<CRFLAnimalGeneData> CreateGene(CRBodyIf* body);
	std::vector<CRFLAnimalGeneData> GetGene(CRBodyIf* body);
	
	void SetGene(std::vector<CRFLAnimalGeneData> gene, CRBodyIf* body);
	void Init();
	void MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB);

	CRFLAnimalGene();
};

#endif