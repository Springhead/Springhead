#include "PHOpGroup.h"
//#include "OPApp.h"
#include "Base/Affine.h"

namespace Spr{;

void PHOpGroup::addPInds(int gPIndex)
{

	for(int i=0;i<gPInd.size();i++)
	{
		if(gPInd[i] == gPIndex)
			return;
	}
	gPInd.push_back(gPIndex);

	this->gNptcl++;
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
	gNptcl = gPInd.size();
}



}//namespace