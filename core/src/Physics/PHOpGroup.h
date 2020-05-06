#ifndef PHOpGroup_H
#define PHOpGroup_H

#include <Physics/SprPHOpObj.h>
#include <Physics/PHOpParticle.h>

namespace Spr{;

class PHOpGroup :public SceneObject, public PHOpGroupDesc
{
	
public:
	SPR_OBJECTDEF(PHOpGroup);
	ACCESS_DESC(PHOpGroup);
	
	//このグループに含む粒子のindex
	std::vector<int> gPInd;
	int GetGrpInPtcl(int gi)
	{
		return gPInd[gi];
	}
	int GetGrpInPtclNum()
	{
		return (int) gPInd.size();
	}
	PHOpGroupDesc* GetGroupDesc()
	{
		return this;
	}
	PHOpGroup(const PHOpGroupDesc& desc = PHOpGroupDesc()) :PHOpGroupDesc(desc)
	{
		gCurrCenter = gOrigCenter = Vec3f(0,0,0);
		gAgroup = gAgroup.Zero();
		gtotalMass = 0;
	
		gNptcl = 0;
		gEigen = gEigen.Unit();
	}
	bool addPInds(int gPIndex);
	void delPInds(int gPIndex);
	void preCalculation();

	void groupTimeStep();
	
	void projectPos(PHOpParticle *objPArr);
	void gravityStep();
	void SwapPtclInGroup();
	

	int getParticleGlbIndex(int pindex)
	{
		return this->gPInd[pindex];
	}

	
};


}//namespace






#endif