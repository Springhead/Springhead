
#include <vector>
#include <Springhead.h>
#include "m2Real.h"
#include "m3Bounds.h"
#include "DeformUtil.h"




void DeformUtil::InitialDU()
{
	isLinkedToDp = false;
	stra_CONCENTRATED = 0;
	stra_FRIABLE = 1;
}

void DeformUtil::BuildParticles(GRMesh &meshObj, DeformParticle &dp,int strategy)
{

	mDp = dp;
	pRadius = mDp.params.radius;
	isLinkedToDp = true;
	if(strategy==stra_CONCENTRATED)
	{
		int tempIndex = 0;
		for(int i = 0;i < mDp.mPos.size();i++)
		{
			parIndex.push_back(findNeighborPs(mDp.mPos[i],tempIndex));
		}
	}
	
	isLinkedToDp = true;

}


