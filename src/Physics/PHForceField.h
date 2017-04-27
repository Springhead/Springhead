/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_FORCEFIELD_H
#define PH_FORCEFIELD_H

#include <Physics/PHEngine.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class PHGravityEngine : public PHEngine{
	SPR_OBJECTDEF(PHGravityEngine);
public:
	PHSolids solids;		///< 重力を加える剛体の配列
	Vec3d	accel;			///< 重力加速度

	int GetPriority() const { return SGBP_GRAVITYENGINE; }
	void Step();
	
	virtual void Clear();
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
};

}

#endif
