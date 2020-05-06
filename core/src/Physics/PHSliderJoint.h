/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSLIDERJOINT_H
#define PHSLIDERJOINT_H

#include <Physics/SprPHJoint.h>
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
class PHSliderJoint : public PH1DJoint{
public:
	SPR_OBJECTDEF(PHSliderJoint);
	SPR_DECLMEMBEROF_PHSliderJointDesc;

	/// コンストラクタ
	PHSliderJoint(const PHSliderJointDesc& desc = PHSliderJointDesc());

	/// ABAで対応するPHTreeNodeの派生クラスを生成して返す
	virtual PHTreeNode* CreateTreeNode(){ return DBG_NEW PHSliderJointNode(); }

	// ----- PHConstraintの派生クラスで実装する機能

	virtual void SetupAxisIndex();
	virtual void CompBias();
	virtual void CompError();
	virtual void UpdateJointState();
};

}

#endif
