/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSLIDERJOINT_H
#define PHSLIDERJOINT_H

#include <SprPhysics.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

///	スライダ関節に対応するツリーノード
class PHSliderJointNode : public PHTreeNode1D{
public:
	SPR_OBJECTDEF(PHSliderJointNode);
	SPR_DECLMEMBEROF_PHSliderJointNodeDesc;

	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	PHSliderJointNode(const PHSliderJointNodeDesc& desc = PHSliderJointNodeDesc()){}
};

///	スライダ関節
class PHSliderJoint : public PHJoint1D{
public:
	SPR_OBJECTDEF(PHSliderJoint);
	SPR_DECLMEMBEROF_PHSliderJointDesc;

	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::SLIDERJOINT;}
	//virtual void SetConstrainedIndex(bool* con);
	virtual void CompBias();
	virtual void CompError();
	virtual void UpdateJointState();
	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHSliderJointNode();
	}
	PHSliderJoint(const PHSliderJointDesc& desc = PHSliderJointDesc());
};

}

#endif
