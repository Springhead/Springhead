/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHHINGEJOINT_H
#define PHHINGEJOINT_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

///	ヒンジ関節に対応するツリーノード
class PHHingeJointNode : public PHTreeNode1D {
public:
	SPR_OBJECTDEF(PHHingeJointNode);
	SPR_DECLMEMBEROF_PHHingeJointNodeDesc;

	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void UpdateJointPosition(double dt);
	PHHingeJointNode(const PHHingeJointNodeDesc& desc = PHHingeJointNodeDesc()){}
};

///	ヒンジ関節
class PHHingeJoint : public PH1DJoint {
public:
	SPR_OBJECTDEF(PHHingeJoint);
	SPR_DECLMEMBEROF_PHHingeJointDesc;

	/// コンストラクタ
	PHHingeJoint(const PHHingeJointDesc& desc = PHHingeJointDesc());

	/// ABAで対応するPHTreeNodeの派生クラスを生成して返す
	virtual PHTreeNode* CreateTreeNode(){ return DBG_NEW PHHingeJointNode(); }

	// ----- PHConstraintの派生クラスで実装する機能
	virtual void UpdateJointState();
	virtual void CompBias();
	virtual void CompError();

	// ----- PH1DJointの派生クラスで実装する機能
	virtual double GetDeviation();	
};

}

#endif
