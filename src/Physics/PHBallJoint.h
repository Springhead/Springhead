/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHBALLJOINT_H
#define PHBALLJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

class PHBallJoint : public InheritConstraint<PHBallJointIf, PHJoint>{
	double	max_angle;
public:
	OBJECT_DEF(PHBallJoint);
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	virtual void SetMaxAngle(double angle){max_angle = angle;}
	virtual double GetMaxAngle(){return max_angle;}
	virtual void SetMotorTorque(const Vec3d& t){fw = t;}
	virtual Vec3d GetMotorTorque(){return fw;}
	virtual Quaterniond GetPosition(){return qjrel;}
	virtual Vec3d GetVelocity(){return wjrel;}
	//virtual void CompConstraintJacobian();
	virtual void CompBias(double dt, double correction_rate);
	//virtual void CompError(double dt);
	virtual void Projection(double& f, int k);
	//virtual void ProjectionCorrection(double& F, int k);
	PHBallJoint(){
		constr[0] = constr[1] = constr[2] = true;
		constr[3] = constr[4] = constr[5] = false;
	}
};

}

#endif
