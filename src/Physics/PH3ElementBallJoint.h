/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 *  PHBallJointはswingによる倒しこみ角度の制限と、twistによるねじりの制限ができます。
 *
 */
#ifndef PH3ELEMENTBALLJOINT_H
#define PH3ELEMENTBALLJOINT_H

#include <SprPhysics.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PH3Element.h>

namespace Spr{

class PH3ElementBallJoint;

class PH3ElementBallJointNode : public PHBallJointNode{
public:
	SPR_OBJECTDEF1(PH3ElementBallJointNode, PHTreeNode);
	SPR_DECLMEMBEROF_PH3ElementBallJointNodeDesc;

	PH3ElementBallJoint*	GetJoint(){return PHTreeNodeND<3>::GetJoint()->Cast();}

	virtual void	CompJointJacobian();
	virtual void	ModifyJacobian();
	virtual void	CompBias();
	virtual void	Projection(double& f, int k);
	virtual void    CompRelativeVelocity();
	virtual void	CompJointCoriolisAccel();
	virtual void	CompRelativePosition();
	virtual void	UpdateJointPosition(double dt);

	PH3ElementBallJointNode(const PH3ElementBallJointNodeDesc& desc = PH3ElementBallJointNodeDesc());
};

class PH3ElementBallJoint : public PHBallJoint, public PH3ElementCommonData{
public:
	SPR_OBJECTDEF1(PH3ElementBallJoint, PHJoint);
	SPR_DECLMEMBEROF_PH3ElementBallJointDesc;
	ACCESS_STATE(PH3Element);
protected:
public:
	std::vector<SpatialVector> fs;	
	/// インタフェースの実装
	double  GetSecondDamper()				{return secondDamper;}
	void	SetSecondDamper(double input)	{secondDamper = input;}
	double  GetYieldStress()				{return yieldStress;}
	void	SetYieldStress(double input)	{yieldStress = input;}
	double  GetHardnessRate()				{return hardnessRate;}
	void	SetHardnessRate(double input)	{hardnessRate = input;}
	void	SetI(const Vec3d i)				{I = i;}
	Vec3d	GetI()							{return I;}

	// このクラス内で再定義する（オーバーライドする）関数
	void	CompBias();		///< 侵入量の判定(オーバーライド)
	//TreeNode用の関数
	virtual PHTreeNode*		CreateTreeNode(){
		return DBG_NEW PH3ElementBallJointNode();
	}
	virtual Matrix3d GetJcinv(){return Jcinv;};
	virtual OnLimit* GetOnLimit(){return onLimit;};

	// コンストラクタ（引数付き）
	PH3ElementBallJoint(const PH3ElementBallJointDesc& desc = PH3ElementBallJointDesc());				/// - コンストラクタ
};

}

#endif
