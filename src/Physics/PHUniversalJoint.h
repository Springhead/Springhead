/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHUNIVERSALJOINT_H
#define PHUNIVERSALJOINT_H

#include <SprPhysics.h>
#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

///	ユニバーサルジョイントに対応するツリーノード
class PHUniversalJointNode : public PHTreeNodeND<2>{
public:
	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	//PHBallJointNode(PHBallJoint* j):PHTreeNodeND<3>(j){}
};

class PHUniversalJoint : public PHJointND<2>, public PHUniversalJointIfInit{
protected:
	Vec2d	torque;
public:
	OBJECTDEF(PHUniversalJoint, PHJointND<2>);
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::UNIVERSALJOINT;}
	virtual void SetMotorTorque(const Vec2d& t){torque = t;}
	virtual Vec2d GetMotorTorque(){return torque;}
	//virtual Vec2d GetPosition(){return qjrel;}
	//virtual Vec2d GetVelocity(){return vjrel.w;}
	//virtual void CompConstraintJacobian();
	//virtual void AddMotorTorque(){f.w = torque * scene->GetTimeStep();}
	virtual void CompBias();
	virtual void Projection(double& f, int k);
	//virtual void CompError(double dt);
	//virtual void ProjectionCorrection(double& F, int k);
	virtual void CompRelativePosition(){}
	virtual void CompRelativeVelocity(){}

	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHUniversalJointNode();
	}
	PHUniversalJoint(){
		//constr[0] = constr[1] = constr[2] = true;
		//constr[3] = constr[4] = constr[5] = false;
	}
};

}

#endif
