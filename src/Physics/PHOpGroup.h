#ifndef PHOpGroup_H
#define PHOpGroup_H

#include <Physics/SprPHOpObj.h>
#include <Physics/PHOpParticle.h>

namespace Spr{;

class PHOpGroup
{
public:
	//粒子グループのid
	int gMyIndex;
	//このグループに含む粒子のindex
	std::vector<int> gPInd;
	//グループのモーメント行列
	Matrix3f gAgroup;
	//グループの固有ベクトル群
	Matrix3f gEigen;
	//グループの質量
	float gtotalMass;
	//初期中心
	Vec3f gOrigCenter;
	//現在中心
	Vec3f gCurrCenter;
	//このグループに含む粒子の数
	int gNptcl;

	PHOpGroup()
	{
		gCurrCenter = gOrigCenter = Vec3f(0,0,0);
		gAgroup = gAgroup.Zero();
		gtotalMass = 0;
	
		gNptcl = 0;
		gEigen = gEigen.Unit();
	}
	void addPInds(int gPIndex);
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