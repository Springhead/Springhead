#ifndef SPRCDSPHASH_H
#define SPRCDSPHASH_H

#include <Foundation/SprObject.h>
#include <Physics/PHOpObj.h>

namespace Spr{;


struct CDSpatialHashTableIf : ObjectIf{
	SPR_IFDEF(CDSpatialHashTable);

	void init(float cellSize,Bounds  bounds);
	bool ReducedSelectPointsToAdd(Vec3f &pCtr, float radius, int objInd, int pInd, int timestamp);
};

struct CDSpatialHashTableDesc{
	
	
};

}
#endif
