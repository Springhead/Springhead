/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHHINGEJOINT_H
#define PHHINGEJOINT_H

#include <SprPhysics.h>
#include <Foundation/Object.h>

namespace Spr{;

class PHHingeJoint : public InheritJoint1D<PHHingeJointIf, PHJoint1D>{
public:
	OBJECT_DEF(PHHingeJoint);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::HINGEJOINT;}
	virtual double GetPosition();
	virtual double GetVelocity();
	virtual void CompBias(double dt, double correction_rate);
	virtual void Projection(double& f, int k);
	PHHingeJoint(){
		axis_index = 5;
	}
};

}

#endif
