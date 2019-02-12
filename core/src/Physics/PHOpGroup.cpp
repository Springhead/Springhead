/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include "PHOpGroup.h"
#include "Base/Affine.h"

namespace Spr{;

bool PHOpGroup::addPInds(int gPIndex)
{

	for (unsigned int i = 0; i<gPInd.size(); i++)
	{
		if (gPInd[i] == gPIndex)
			return false;
	}
	gPInd.push_back(gPIndex);

	this->gNptcl++;

	return true;
}
	
void PHOpGroup::delPInds(int gPIndex)
{
	
	std::vector<int> newgpinds;
	if(gPIndex==this->gMyIndex)
		return;
	for(int i=0;i<this->gNptcl;i++)
	{
		int getI = this->getParticleGlbIndex(i);
		if(getI!=gPIndex)
			newgpinds.push_back(getI);

	}
	gPInd.swap(newgpinds);
	gNptcl = (int)gPInd.size();
}



}//namespace