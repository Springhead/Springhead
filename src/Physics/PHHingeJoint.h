/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_OBJECTDEF(PHHingeJointNode);
	SPR_DECLMEMBEROF_PHHingeJointNodeDesc;

	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	PHHingeJointNode(const PHHingeJointNodeDesc& desc = PHHingeJointNodeDesc()){}
};

///	ヒンジ関節
class PHHingeJoint : public PHJoint1D{


private:
	// 軌道追従制御用変数，PHBallJointとは変数の型が異なる
	double qd,		preQd;		///< ある時刻の目標位置への角度と１時刻前の目標位置への角度
	double qdDot,	preQdDot;	///< ある時刻の目標位置への角度の一階微分と１時刻前の目標位置への角度の一階微分
	double qdWDot;				///< ある時刻の目標位置への角度の二階微分
		
public:
	SPR_OBJECTDEF(PHHingeJoint);
	SPR_DECLMEMBEROF_PHHingeJointDesc;

	virtual void CompBias();
	virtual void UpdateJointState();
	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHHingeJointNode();
	}
	virtual void	CompError();
	PHHingeJoint(const PHHingeJointDesc& desc = PHHingeJointDesc());
};

}

#endif
