/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHBALLJOINT_H
#define PHBALLJOINT_H

#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHJointLimit.h>
#include <Physics/PHJointMotor.h>

namespace Spr{;

class PHBallJoint;
// enum PHDeformationType;

///	ボールジョイントに対応するツリーノード
class PHBallJointNode : public PHTreeNodeND<3>{
public:
	SPR_OBJECTDEF1(PHBallJointNode, PHTreeNode);
	SPR_DECLMEMBEROF_PHBallJointNodeDesc;

	/// スイング・ツイスト角の時間変化率から角速度へのヤコビアン
	Matrix3d	Jst;

	PHBallJoint*		GetJoint(){return PHTreeNodeND<3>::GetJoint()->Cast();}
	virtual void		CompJointJacobian();
	virtual void		CompJointCoriolisAccel();
	virtual void		UpdateJointPosition(double dt);
	virtual void		CompRelativePosition();
	virtual void		CompRelativeVelocity();
	PHBallJointNode(const PHBallJointNodeDesc& desc = PHBallJointNodeDesc()){}	
};

/// ボールジョイント
class PHBallJoint : public PHNDJoint<3>{
protected:
	friend class PHBallJointConeLimit;
	friend class PHBallJointSplineLimit;
	friend class PHBallJointMotor;
	friend class PHBallJointNode;

public:
	SPR_OBJECTDEF1(PHBallJoint, PHJoint);
	SPR_DECLMEMBEROF_PHBallJointDesc;

	UTRef<PHBallJointLimit>		limit;		///< 可動範囲拘束
	UTRef<PHBallJointMotor>		motor;		///< 関節コントローラ

	/// コンストラクタ
	PHBallJoint(const PHBallJointDesc& desc = PHBallJointDesc());

	/// ABAで対応するPHTreeNodeの派生クラスを生成して返す
	virtual PHTreeNode*		CreateTreeNode(){ return DBG_NEW PHBallJointNode(); }

	// ----- PHConstraintの機能をオーバーライド

	//virtual void	Setup();
	//virtual	void	IterateGS();

	// ----- PHConstraintの派生クラスで実装する機能

	virtual void	UpdateJointState();
	//virtual void	SetupAxisIndex() { PHJoint::SetupAxisIndex(); motor.SetupAxisIndex(); }
	virtual void	CompBias();
	virtual void	CompError();

	// ----- インタフェースの実装
	
	/// ChildObject．可動域を追加できる
	virtual bool AddChildObject(ObjectIf* o) {
		if (!limit) { limit = o->Cast(); if(limit){ limit->joint=this;return true; }}
		return PHConstraint::AddChildObject(o);
	}
	virtual size_t NChildObject() const {
		return((limit?1:0)+PHConstraint::NChildObject());
	}
	virtual ObjectIf* GetChildObject(size_t i) {
		if (i==0 && limit) { return limit->Cast(); }
		return PHConstraint::GetChildObject(i - (limit ? 1 : 0));
	}

	PHBallJointLimitIf* CreateLimit(const IfInfo* ii, const PHBallJointLimitDesc& desc) {
		PHBallJointLimitIf* limit = GetScene()->CreateObject(ii, &desc)->Cast();
		if (limit) { AddChildObject(limit); }
		return limit;
	}

	PHBallJointLimitIf* GetLimit() { return limit->Cast(); }

	Vec3d		GetAngle   (){ return Vec3d(position[0], position[1], position[2]); }
	Quaterniond GetPosition(){ return Xjrel.q; }
	Vec3d		GetVelocity(){ return Vec3d(velocity[0], velocity[1], velocity[2]); }
	void        SetSpring(const double& spring) { this->spring = spring; }
	double      GetSpring() { return spring; }
	void        SetDamper(const double& damper) { this->damper = damper; }
	double      GetDamper() { return damper; }
	void        SetSecondDamper(const Vec3d& secondDamper) { this->secondDamper = secondDamper; }
	Vec3d       GetSecondDamper() { return secondDamper; }
	void        SetTargetPosition(const Quaterniond& targetPosition) { this->targetPosition = targetPosition; }
	Quaterniond GetTargetPosition() { return targetPosition; }
	void        SetTargetVelocity(const Vec3d& targetVelocity) { this->targetVelocity = targetVelocity; }
	Vec3d       GetTargetVelocity() { return targetVelocity; }
	void        SetOffsetForce(const Vec3d& offsetForce) { this->offsetForce = offsetForce; }
	Vec3d       GetOffsetForce() { return offsetForce; }
	void        SetYieldStress(const double& yieldStress) { this->yieldStress = yieldStress; }
	double      GetYieldStress() { return yieldStress; }
	void        SetHardnessRate(const double& hardnessRate) { this->hardnessRate = hardnessRate; }
	double      GetHardnessRate() { return hardnessRate; }
	void        SetSecondMoment(Vec3d sM) { secondMoment = sM; }
	Vec3d       GetSecondMoment() { return secondMoment; }
	Vec3d       GetMotorForce() {
		//if (limit && limit->IsOnLimit())
		//	return Vec3d();
		//return f.w() *  GetScene()->GetTimeStepInv();
		return Vec3d(motor->f[0], motor->f[1], motor->f[2]) * GetScene()->GetTimeStepInv();
	}
};

}

#endif
