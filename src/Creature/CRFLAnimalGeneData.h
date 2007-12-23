#ifndef CRFLANIMALGENEDATA_H
#define CRFLANIMALGENEDATA_H

#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>

using namespace Spr;

struct CRFLAnimalGeneData{

public:
	Quaterniond goalDir;
	
	enum {
		GEN_QUATERNIOND=0,
		GEN_DOUBLE
	} geneType;

};

#endif