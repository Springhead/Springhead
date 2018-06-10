/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSPRING_H
#define PHSPRING_H

#include <Physics/SprPHJoint.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHJointMotor.h>

namespace Spr{;

class PHSpring : public PHJoint{
	friend class PHSpringMotor;

public:
	SPR_OBJECTDEF(PHSpring);
	SPR_DECLMEMBEROF_PHSpringDesc;

	/// 関節コントローラ．PHSpringは6自由度関節（＝拘束しない）であり，実質的なコントロールはMotorが担当．
	UTRef<PHSpringMotor>  motor;

	/// コンストラクタ
	PHSpring(const PHSpringDesc& desc = PHSpringDesc()) {
		motor = DBG_NEW PHSpringMotor();
		motor->joint = this;

		SetDesc(&desc);

		for (int i=0; i<6; ++i)
			movableAxes.Enable(i);
	}

	// ----- PHConstraintの派生クラスで実装する機能

	/// どの自由度を速度拘束するかを設定
	//virtual void SetupAxisIndex() {
	//	PHJoint::SetupAxisIndex();
	//	motor.SetupAxisIndex();
	//}

	/// LCPの補正値の計算．誤差修正用
	//virtual void CompBias() {
	//	PHJoint::CompBias();
	//	motor.CompBias();
	//}

	// ----- インタフェースの実装

	virtual void SetTargetPosition(const Vec3d& targetPosition) { this->targetPosition = targetPosition; }
	virtual Vec3d GetTargetPosition() { return targetPosition; }
	virtual void SetTargetOrientation(const Quaterniond& targetOrientation) { this->targetOrientation = targetOrientation; }
	virtual Quaterniond GetTargetOrientation() { return targetOrientation; }
	virtual void SetSpring(const Vec3d& spring) { this->spring = spring; }
	virtual Vec3d GetSpring() { return spring; }
	virtual void SetDamper(const Vec3d& damper) { this->damper = damper; }
	virtual Vec3d GetDamper() { return damper; }
	virtual void SetSecondDamper(const Vec3d& secondDamper) { this->secondDamper = secondDamper; }
	virtual Vec3d GetSecondDamper() { return secondDamper; }
	virtual void SetSpringOri(const double& springOri) { this->springOri = springOri; }
	virtual double GetSpringOri() { return springOri; }
	virtual void SetDamperOri(const double& damperOri) { this->damperOri = damperOri; }
	virtual double GetDamperOri() { return damperOri; }
	virtual void SetSecondDamperOri(const double& secondDamperOri) { this->secondDamperOri = secondDamperOri; }
	virtual double GetSecondDamperOri() { return secondDamperOri; }
	virtual void SetYieldStress(const double& yieldStress) { this->yieldStress = yieldStress; }
	virtual double GetYieldStress() { return yieldStress; }
	virtual void SetHardnessRate(const double& hardnessRate) { this->hardnessRate = hardnessRate; }
	virtual double GetHardnessRate() { return hardnessRate; }
	virtual void SetSecondMoment(Vec3d sM) { secondMoment = sM; }
	virtual Vec3d GetSecondMoment() { return secondMoment; }

	// <!!>
	virtual Vec6d GetMotorForce() {
		Vec3d _f = motor->f.v() * GetScene()->GetTimeStepInv();
		Vec3d _t = motor->f.w() * GetScene()->GetTimeStepInv();
		return Vec6d(_f.x, _f.y, _f.z, _t.x, _t.y, _t.z);
	}
};

}

#endif
