#ifndef FLANIMALGENE_H
#define FLANIMALGENE_H

#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>
#include <vector>

#include "GeneData.h"

class CRFLAnimalGene{

private:
	std::vector<CRFLAnimalGeneData> flAnimalGenes;
public:
	void				  Init();
	void				  CreateGene();
	void				  SetGene(std::vector<CRFLAnimalGeneData> gene);
	std::vector<CRFLAnimalGeneData> GetGene();
	void				  MixGenes(std::vector<CRFLAnimalGeneData> geneA, std::vector<CRFLAnimalGeneData> geneB);

	CRFLAnimalGene();
};

#endif