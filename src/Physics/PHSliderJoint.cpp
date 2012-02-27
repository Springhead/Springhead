/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHSliderJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHSliderJointNode

void PHSliderJointNode::CompJointJacobian() {
	J.col(0).SUBVEC(0,3) = Vec3d(0.0, 0.0, 1.0);
	J.col(0).SUBVEC(3,3).clear();
	PHTreeNode1D::CompJointJacobian();
}

void PHSliderJointNode::CompJointCoriolisAccel() {
	cj.clear();
}

void PHSliderJointNode::CompRelativeVelocity() {
	PH1DJoint* j = GetJoint();
	j->vjrel.v() = Vec3d(0.0, 0.0, j->velocity[0]);
	j->vjrel.w().clear();
}

void PHSliderJointNode::CompRelativePosition() {
	PH1DJoint* j = GetJoint();
	j->Xjrel.q = Matrix3d::Unit();
	j->Xjrel.r = Vec3d(0.0, 0.0, j->position[0]);
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHSliderJoint

PHSliderJoint::PHSliderJoint(const PHSliderJointDesc& desc) {
	SetDesc(&desc);

	// 可動軸・拘束軸の設定
	nMovableAxes   = 1;
	movableAxes[0] = 2;
	InitTargetAxes();
}

// ----- エンジンから呼び出される関数

void PHSliderJoint::UpdateJointState() {
	position[0] = Xjrel.r.z;
	velocity[0] = vjrel.v().z;
}

// ----- PHConstraintの派生クラスで実装される機能

void PHSliderJoint::SetupAxisIndex() {
	if (!bArticulated) {
		// -- 0 
		axes.Enable(0);
		// -- 1
		if (bConstraintY)     { axes.Enable(1); }
		// -- 2
		//   movable
		// -- 3
		if (bConstraintRollX) { axes.Enable(3); }
		// -- 4
		axes.Enable(4);
		// -- 5
		if (bConstraintRollZ) { axes.Enable(5); }
	}

	// -- PH1DJoint::SetupAxisIndex に相当する部分
	motor.SetupAxisIndex();
	if (limit) { limit->SetupAxisIndex(); }
}

void PHSliderJoint::CompBias() {
	double dtinv = 1.0 / GetScene()->GetTimeStep();

	if (engine->numIterCorrection==0){ // Correctionを速度LCPで行う場合
		db.v()	 = Xjrel.r * dtinv;
		db.v().z = 0.0;
		if (!bConstraintY) { db.v().y = 0.0; }

		db.w() =  Xjrel.q.RotationHalf() * dtinv;
		if (!bConstraintRollX) { db.w().x = 0.0; }
		if (!bConstraintRollZ) { db.w().z = 0.0; }

		db *= engine->velCorrectionRate;
	}

	// 親クラスのCompBias．motor,limitのCompBiasが呼ばれるので最後に呼ぶ
	PH1DJoint::CompBias();
}

void PHSliderJoint::CompError() {
	/*B.v() = Xjrel.r;
	B.w() = Xjrel.q.V();
	if(onUpper)
		B.v().z = Xjrel.r.z - upper;
	else if(onLower)
		B.v().z = Xjrel.r.z - lower;
	else B.v().z = 0.0;*/
}

}

