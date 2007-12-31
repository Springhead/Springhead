/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	ギア：１自由度関節を連動させる拘束
 */
class PHGear : public SceneObject, public PHGearIfInit{
public:
	OBJECTDEF(PHGear, SceneObject);
	PHScene*			scene;
	PHConstraintEngine* engine;
	PHJoint1D*		joint[2];		///< 連動させる関節
	bool			bArticulated;
	double			ratio;
	double			A, Ainv, b;
	double			f;

	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual size_t NChildObject()const{ return 2; }
	virtual ObjectIf* GetChildObject(size_t pos);
	void	SetDesc(const void* desc);
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
