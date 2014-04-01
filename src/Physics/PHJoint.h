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

public:
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
	typedef PTM::TVector<NDOF, double> VecNd;

	/// 関節の位置・速度
	VecNd position, velocity;

	/// コンストラクタ
	PHNDJoint(){
		position.clear();
		velocity.clear();
	}
};

class PH1DJoint : public PHNDJoint<1> {
protected:
	friend class PHTreeNode1D;
	friend class PH1DJointLimit;
	friend class PH1DJointMotor;

public:
	SPR_OBJECTDEF_ABST1(PH1DJoint, PHJoint);
	SPR_DECLMEMBEROF_PH1DJointDesc;

	UTRef<PH1DJointLimit> limit;			///< 可動範囲拘束
	UTRef<PH1DJointMotor> motor;			///< モータ

	/// コンストラクタ
	PH1DJoint(){
		limit = DBG_NEW PH1DJointLimit();
		motor = DBG_NEW PH1DJointMotor();
		limit->joint = this;
		motor->joint = this;
	}

	// ----- PHConstraintの派生クラスで実装する機能

	/// どの自由度を速度拘束するかを設定
	//virtual void SetupAxisIndex() {
	//	PHJoint::SetupAxisIndex();
	//	motor.SetupAxisIndex();
	//	if (limit) { limit->SetupAxisIndex(); }
	//}
	
	/// LCPの補正値の計算．誤差修正用
	//virtual void CompBias() {
	//	PHJoint::CompBias();
	//	motor.CompBias();
	//	if (limit) { limit->CompBias(); }
	//}

	// ----- このクラスと，このクラスから派生するクラスの機能

	/// バネ中点（目標角度）からの偏差を返す．回転関節がオーバライドする
	virtual double	GetDeviation(){
		return GetPosition() - GetTargetPosition();
	}

	/// ChildObject．可動域を追加できる
	virtual bool      AddChildObject(ObjectIf* o);
	virtual size_t    NChildObject  () const;
	virtual ObjectIf* GetChildObject(size_t i);
	PH1DJointLimitIf* CreateLimit   (const PH1DJointLimitDesc& desc);

	double	GetPosition() { UpdateState(); return position[0]; }
	double	GetVelocity() { UpdateState(); return velocity[0]; }

	PH1DJointLimitIf* GetLimit() { return limit->Cast(); }

	// ----- インタフェースの実装
	bool   IsCyclic         (){ return cyclic; }
	void   SetCyclic        (bool on){ cyclic = on; }
	void   SetSpring        (const double& spring) { this->spring = spring; }
	double GetSpring        () { return spring; }
	void   SetDamper        (const double& damper) { this->damper = damper; }
	double GetDamper        () { return damper; }
	void   SetSecondDamper  (const double& secondDamper) { this->secondDamper = secondDamper; }
	double GetSecondDamper  () { return secondDamper; }
	void   SetTargetPosition(const double& targetPosition) { this->targetPosition = targetPosition; }
	double GetTargetPosition() { return targetPosition; }
	void   SetTargetVelocity(const double& targetVelocity) { this->targetVelocity = targetVelocity; }
	double GetTargetVelocity() { return targetVelocity; }
	void   SetOffsetForce   (const double& offsetForce) { this->offsetForce = offsetForce; }
	double GetOffsetForce   () { return offsetForce; }
	void   SetYieldStress   (const double& yieldStress) { this->yieldStress = yieldStress; }
	double GetYieldStress   () { return yieldStress; }
	void   SetHardnessRate  (const double& hardnessRate) { this->hardnessRate = hardnessRate; }
	double GetHardnessRate  () { return hardnessRate; }
	void   SetSecondMoment  (double sM) { secondMoment = sM; }
	double GetSecondMoment  () { return secondMoment; }
	double GetMotorForce    () {
		return motor->f[0] * GetScene()->GetTimeStepInv();
	//	if (limit) { if (limit->IsOnLimit()) return 0; } return(f[movableAxes[0]] / GetScene()->GetTimeStep());
	}
};

class PHMate : public PHJoint{
public:
	SPR_OBJECTDEF_ABST1(PHMate, PHJoint);
};

class PHPointToPointMate : public PHMate{
public:
	SPR_OBJECTDEF(PHPointToPointMate);
	virtual void CompBias();
	PHPointToPointMate(const PHPointToPointMateDesc& desc = PHPointToPointMateDesc());
};
class PHPointToLineMate : public PHMate{
public:
	SPR_OBJECTDEF(PHPointToLineMate);
	virtual void CompBias();
	PHPointToLineMate(const PHPointToLineMateDesc& desc = PHPointToLineMateDesc());
};
class PHPointToPlaneMate : public PHMate{
public:
	SPR_OBJECTDEF(PHPointToPlaneMate);
	Vec2d	range;
	bool	onLower;
	bool	onUpper;
	double	diff;
public:
	void	SetRange(Vec2d  r){ range = r; }
	void	GetRange(Vec2d& r){ r = range; }

	virtual void SetupAxisIndex();
	virtual void CompBias      ();

	PHPointToPlaneMate(const PHPointToPlaneMateDesc& desc = PHPointToPlaneMateDesc());
};
class PHLineToLineMate : public PHMate{
public:
	SPR_OBJECTDEF(PHLineToLineMate);
	virtual void CompBias();
	PHLineToLineMate(const PHLineToLineMateDesc& desc = PHLineToLineMateDesc());
};
class PHPlaneToPlaneMate : public PHMate{
public:
	SPR_OBJECTDEF(PHPlaneToPlaneMate);
	virtual void CompBias();
	PHPlaneToPlaneMate(const PHPlaneToPlaneMateDesc& desc = PHPlaneToPlaneMateDesc());
};

}

#endif
