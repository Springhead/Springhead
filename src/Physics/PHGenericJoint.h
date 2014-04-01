/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHGENERICJOINT_H
#define PHGENERICJOINT_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

class PHGenericJoint;

class PHGenericJointNode : public PHTreeNode1D{
public:
	SPR_OBJECTDEF(PHGenericJointNode);

	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void UpdateJointPosition(double dt);

	PHGenericJoint* GetJoint(){ return DCAST(PHGenericJoint, joint); }
	
	PHGenericJointNode(const PHGenericJointNodeDesc& desc = PHGenericJointNodeDesc()){}
};

class PHGenericJoint : public PH1DJoint{
public:
	SPR_OBJECTDEF(PHGenericJoint);
	
	PHGenericJointCallback* callback;

public:
	void SetCallback(PHGenericJointCallback* cb);
	void SetParam   (const std::string& name, double value);
	
	virtual PHTreeNode* CreateTreeNode  (){ return DBG_NEW PHGenericJointNode(); }
	virtual void        UpdateJointState();
	virtual void        CompBias        ();
	virtual void        CompError       ();
	virtual double      GetDeviation    ();

	PHGenericJoint(const PHGenericJointDesc& desc = PHGenericJointDesc());
};

}

#endif
