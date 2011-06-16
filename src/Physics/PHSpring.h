/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSPRING_H
#define PHSPRING_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

class PHSpring : public PHJoint{
	//Vec3d spring, damper;
	//double springOri, damperOri;
public:
	SPR_OBJECTDEF(PHSpring);
	SPR_DECLMEMBEROF_PHSpringDesc;

	virtual void SetSpring(const Vec3d& s){spring = s;}
	virtual Vec3d GetSpring(){return spring;}
	virtual void SetDamper(const Vec3d& d){damper = d;}
	virtual Vec3d GetDamper(){return damper;}

	virtual void SetSpringOri(const double spring){springOri = spring;}
	virtual double GetSpringOri(){return springOri;}
	virtual void SetDamperOri(const double damper){damperOri = damper;}
	virtual double GetDamperOri(){return damperOri;}

	//virtual void SetDesc(const void* desc);
	virtual void SetConstrainedIndex(int* con);
	virtual void SetConstrainedIndexCorrection(bool* con);
	virtual void CompBias();
	virtual void IterateLCP();
	void ElasticDeformation();
	void PlasticDeformation();

	/// インタフェースの実装
	Vec3d	GetSecondDamper()				{return secondDamper;}
	void	SetSecondDamper(Vec3d input)	{secondDamper = input;}
	double  GetYieldStress()				{return yieldStress;}
	void	SetYieldStress(double input)	{yieldStress = input;}
	double  GetHardnessRate()				{return hardnessRate;}
	void	SetHardnessRate(double input)	{hardnessRate = input;}
	void	SetDefomationType(int t)		{type = (PHJointDesc::PHDeformationType)t;}
	int 	GetDefomationType()				{return (int)type;}
	PHJointDesc::PHDeformationType 	GetDeformationMode(){};

	bool	yieldFlag;		    // 降伏応力のフラグ

	PHSpring(const PHSpringDesc& desc = PHSpringDesc());
};

}

#endif
