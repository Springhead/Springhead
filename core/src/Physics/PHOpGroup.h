/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
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
	
	int getParticleGlbIndex(int pindex)
	{
		return this->gPInd[pindex];
	}

	
};


}//namespace






#endif