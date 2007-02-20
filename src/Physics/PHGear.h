/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHGEAR_H
#define PHGEAR_H

#include <SprPhysics.h>
//#include <Physics/PHConstraint.h>

namespace Spr{;

class PHScene;
class PHConstraintEngine;
class PHJoint1D;

/**
	ÉMÉAÅFÇPé©óRìxä÷êﬂÇòAìÆÇ≥ÇπÇÈçSë©
 */
class PHGear : public SceneObject, public PHGearIfInit{
public:
	OBJECTDEF(PHGear, SceneObject);
	PHScene*			scene;
	PHConstraintEngine* engine;
	PHJoint1D*	joint[2];
	bool		bArticulated;
	double		ratio;
	double		A, Ainv, b;
	double		f;

	void	SetDesc(const PHGearDesc& desc);
	void	CompResponse(double f);
	void	SetupLCP();
	void	IterateLCP();
	PHGear(const PHGearDesc& desc = PHGearDesc());
};

class PHGears : public std::vector< UTRef<PHGear> >{
public:
	PHGear* FindByJointPair(PHJoint1D* lhs, PHJoint1D* rhs){
		for(iterator it = begin(); it != end(); it++)
			if((*it)->joint[0] == lhs && (*it)->joint[1] == rhs)
				return *it;
		return NULL;
	}
};

}

#endif
