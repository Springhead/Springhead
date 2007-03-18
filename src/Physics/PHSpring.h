/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSPRING_H
#define PHSPRING_H

#include <SprPhysics.h>
#include <Foundation/Object.h>

namespace Spr{;

class PHSpring : public PHJoint, public PHSpringIfInit{
	Vec3d spring, damper;
public:
	OBJECTDEF(PHSpring, PHJoint);
	virtual void SetSpring(const Vec3d& s){spring = s;}
	virtual Vec3d GetSpring(){return spring;}
	virtual void SetDamper(const Vec3d& d){damper = d;}
	virtual Vec3d GetDamper(){return damper;}

	virtual void SetDesc(const void* desc);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::SPRING;}
	virtual void SetConstrainedIndex(bool* con);
	virtual void CompBias();
	virtual void Projection(double& f, int k){}
	PHSpring(const PHSpringDesc& desc = PHSpringDesc());
};

}

#endif
