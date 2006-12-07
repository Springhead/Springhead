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
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

///	ヒンジ関節のツリーノード
class PHHingeJointNode : public PHTreeNode1D{
public:
	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompBias();
	//virtual void Projection(double&, int){}
	//PHHingeJointNode();
	//PHHingeJointNode(PHHingeJoint* j);
};

///	ヒンジ関節
class PHHingeJoint : public PHJoint1D, PHHingeJointIfInit{
public:
	OBJECT_DEF(PHHingeJoint);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::HINGEJOINT;}
	virtual void CompBias();
	virtual void UpdateJointState();
	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHHingeJointNode();
	}

	// virtual void CompConstraintJacobian();
	// virtual void CompError(double dt);
	// virtual void ProjectionCorrection(double& F, int k);
	PHHingeJoint();
};

}

#endif
