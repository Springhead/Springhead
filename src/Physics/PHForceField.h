/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_FORCEFIELD_H
#define PH_FORCEFIELD_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class PHGravityEngine : public PHEngine{
	OBJECT_DEF_NOIF(PHGravityEngine);
public:
	PHSolids solids;
	Vec3f	accel;

	int GetPriority() const {return SGBP_GRAVITYENGINE;}
	void Step();
	void Clear(){solids.clear();}
	virtual bool AddChildObject(ObjectIf* o);
};

}

#endif
