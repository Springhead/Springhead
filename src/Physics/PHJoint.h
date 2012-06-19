/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHJOINT_H
#define PHJOINT_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHJointLimit.h>
#include <Physics/PHJointMotor.h>
#include <Physics/PhysicsDecl.hpp>

namespace Spr{;

class PHJoint : public PHConstraint {
public:
	SPR_OBJECTDEF_ABST(PHJoint);
	SPR_DECLMEMBEROF_PHJointDesc;

	/// コンストラクタ
	PHJoint() {}

	/// ABAで対応するPHTreeNodeの派生クラスを生成して返す
	virtual PHTreeNode* CreateTreeNode(){ return NULL; }

	// ----- インタフェースの実装

	void	SetMaxForce(double max){ fMax = fabs(max); }
	double	GetMaxForce(){ return fMax; }
};

template<int NDOF> class PHTreeNodeND;

template<int NDOF>
class PHNDJoint : public PHJoint {
protected:
	friend class PHTreeNodeND<NDOF>;

public:
	/// 関節の位置・速度
	PTM::TVector<NDOF,double> position, velocity;

	/// コンストラクタ
	PHNDJoint(){
		position.clear();
		velocity.clear();
		nMovableAxes = NDOF;
	}
};

class PH1DJoint : public PHNDJoint<1> {
protected:
	friend class PHTreeNode1D;
	friend class PH1DJointLimit;
	friend class PH1DJointMotor;

	PH1DJointLimit* limit;			///< 可動範囲拘束
	PH1DJointMotor  motor;			///< モータ

public:
	SPR_OBJECTDEF_ABST1(PH1DJoint, PHJoint);
	SPR_DECLMEMBEROF_PH1DJointDesc;

	/// コンストラクタ
	PH1DJoint() {
		motor.joint = this;
		limit = NULL;
	}

	// ----- PHConstraintの派生クラスで実装する機能

	/// どの自由度を速度拘束するかを設定
	virtual void SetupAxisIndex() {
		PHJoint::SetupAxisIndex();
		motor.SetupAxisIndex();
		if (limit) { limit->SetupAxisIndex(); }
	}
	
	/// LCPの補正値の計算．誤差修正用
	virtual void CompBias() {
		PHJoint::CompBias();
		motor.CompBias();
		if (limit) { limit->CompBias(); }
	}

	// ----- このクラスと，このクラスから派生するクラスの機能

	/// バネ中点（目標角度）からの偏差を返す．回転関節がオーバライドする
	virtual double	GetDeviation(){
		return GetPosition() - GetTargetPosition();
	}

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

	PH1DJointLimitIf* CreateLimit(const PH1DJointLimitDesc& desc) {
		PH1DJointLimitIf* limit = GetScene()->CreateObject(PH1DJointLimitIf::GetIfInfoStatic(), &desc)->Cast();
		if (limit) { AddChildObject(limit); }
		return limit;
	}

	double	GetPosition() { UpdateState(); return position[0]; }
	double	GetVelocity() { UpdateState(); return velocity[0]; }

	PH1DJointLimitIf* GetLimit() { return limit->Cast(); }

	virtual void SetSpring(const double& spring) { this->spring = spring; }
	virtual double GetSpring() { return spring; }
	virtual void SetDamper(const double& damper) { this->damper = damper; }
	virtual double GetDamper() { return damper; }
	virtual void SetSecondDamper(const double& secondDamper) { this->secondDamper = secondDamper; }
	virtual double GetSecondDamper() { return secondDamper; }
	virtual void SetTargetPosition(const double& targetPosition) { this->targetPosition = targetPosition; }
	virtual double GetTargetPosition() { return targetPosition; }
	virtual void SetTargetVelocity(const double& targetVelocity) { this->targetVelocity = targetVelocity; }
	virtual double GetTargetVelocity() { return targetVelocity; }
	virtual void SetOffsetForce(const double& offsetForce) { this->offsetForce = offsetForce; }
	virtual double GetOffsetForce() { return offsetForce; }
	virtual void SetYieldStress(const double& yieldStress) { this->yieldStress = yieldStress; }
	virtual double GetYieldStress() { return yieldStress; }
	virtual void SetHardnessRate(const double& hardnessRate) { this->hardnessRate = hardnessRate; }
	virtual double GetHardnessRate() { return hardnessRate; }
	virtual void SetSecondMoment(double sM) { secondMoment = sM; }
	virtual double GetSecondMoment() { return secondMoment; }
	virtual double GetMotorForce() { if (limit) { if (limit->IsOnLimit()) return 0; } return(f[movableAxes[0]] / GetScene()->GetTimeStep()); }
};

}

#endif
