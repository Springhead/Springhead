/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHGEAR_H
#define PHGEAR_H

#include "../Foundation/Object.h"
#include <Physics/SprPHJoint.h>
#include "PhysicsDecl.hpp"

namespace Spr{;

class PHScene;
class PHConstraintEngine;
class PH1DJoint;

/**
	ギア：１自由度関節を連動させる拘束
 */
class PHGear : public SceneObject{
public:
	SPR_OBJECTDEF(PHGear);
	SPR_DECLMEMBEROF_PHGearDesc;

	PHScene*			scene;
	PHConstraintEngine* engine;
	PH1DJoint*		joint[2];		///< 連動させる関節
	bool			bArticulated;
	double			A, Ainv, b;
	double			f;

	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual size_t NChildObject()const{ return 2; }
	virtual ObjectIf* GetChildObject(size_t pos);
	void	CompResponse(double f);
	void	SetupLCP();
	void	IterateLCP();
	double GetRatio() const { return ratio; }
	PHGear(const PHGearDesc& desc = PHGearDesc());
};

class PHGears : public std::vector< UTRef<PHGear> >{
public:
	PHGear* FindByJointPair(PH1DJoint* lhs, PH1DJoint* rhs){
		for(iterator it = begin(); it != end(); it++)
			if((*it)->joint[0] == lhs && (*it)->joint[1] == rhs)
				return *it;
		return NULL;
	}
};

}

#endif
