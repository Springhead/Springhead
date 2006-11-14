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
#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

///	ボールジョイントに対応するツリーノード
class PHBallJointNode : public PHTreeNodeND<3>{
public:
	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	//PHBallJointNode(PHBallJoint* j):PHTreeNodeND<3>(j){}
};

class PHBallJoint : public InheritConstraint<PHBallJointIf, PHJointND<3> >{
protected:
	double	max_angle;
	Vec3d	torque;
public:
	OBJECT_DEF(PHBallJoint);
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	virtual void SetMaxAngle(double angle){max_angle = angle;}
	virtual double GetMaxAngle(){return max_angle;}
	virtual void SetMotorTorque(const Vec3d& t){torque = t;}
	virtual Vec3d GetMotorTorque(){return torque;}
	virtual Quaterniond GetPosition(){return qjrel;}
	virtual Vec3d GetVelocity(){return vjrel.w;}
	//virtual void CompConstraintJacobian();
	virtual void AddMotorTorque(){f.w = torque * scene->GetTimeStep();}
	virtual void CompBias();
	virtual void Projection(double& f, int k);
	//virtual void CompError(double dt);
	//virtual void ProjectionCorrection(double& F, int k);
	virtual void CompRelativePosition(){}
	virtual void CompRelativeVelocity(){}

	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHBallJointNode();
	}
	PHBallJoint(){
		constr[0] = constr[1] = constr[2] = true;
		constr[3] = constr[4] = constr[5] = false;
	}
};

}

#endif
