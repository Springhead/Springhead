/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFIXJOINT_H
#define PHFIXJOINT_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

class PHFixJointNode : public PHTreeNode{
public:
	SPR_OBJECTDEF(PHFixJointNode);

	virtual void CompResponseMatrix    ();
	virtual void CompResponseMap       ();
	virtual void CompResponse          (PHTreeNode* src, const SpatialVector& df);
	virtual void CompResponseCorrection(PHTreeNode* src, const SpatialVector& dF);
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition  ();
	virtual void CompRelativeVelocity  ();
	virtual void AccumulateInertia     ();
	virtual void AccumulateBiasForce   ();
	virtual void CompAccel             ();
	
	PHFixJointNode(const PHFixJointNodeDesc& desc = PHFixJointNodeDesc()){}
};

class PHFixJoint : public PHJoint{
public:
	SPR_OBJECTDEF(PHFixJoint);
	
	virtual PHTreeNode* CreateTreeNode(){ return DBG_NEW PHFixJointNode(); }
	virtual void CompBias();

	PHFixJoint(const PHFixJointDesc& desc = PHFixJointDesc());
};

}

#endif
