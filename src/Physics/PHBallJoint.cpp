/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 * 
 *　SwingDirの制限は入っていません。必要な人が頑張って実装してください(by toki 2007.12.05)
 *
 */
#include <Physics/PHBallJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;
// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHBallJointNode

void PHBallJointNode::CompJointJacobian(){
	PHBallJoint* j = GetJoint();
	J.SUBMAT(0,0,3,3).clear();
	J.SUBMAT(3,0,3,3) = TMatrix3<double>::Unit();
	PHTreeNodeND<3>::CompJointJacobian();
}

void PHBallJointNode::CompJointCoriolisAccel(){
	cj.clear();		//関節座標をquaternionにとる場合コリオリ項は0
}

void PHBallJointNode::UpdateJointPosition(double dt){
	PHBallJoint* j = GetJoint();
	j->Xjrel.q += j->Xjrel.q.Derivative(j->vjrel.w()) * dt;
	j->Xjrel.q.unitize();
}

void PHBallJointNode::CompRelativePosition(){
	PHBallJoint* j = GetJoint();
	j->Xjrel.r.clear();
}

void PHBallJointNode::CompRelativeVelocity(){
	PHBallJoint* j = GetJoint();
	j->vjrel.v() = Vec3d();
	j->vjrel.w() = j->GetVelocity();
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHBallJoint

PHBallJoint::PHBallJoint(const PHBallJointDesc& desc){
	SetDesc(&desc);
	movableAxes.Enable(3);
	movableAxes.Enable(4);
	movableAxes.Enable(5);
	
	limit = 0;
	motor = 0;
	//limit = NULL;
	//motor = DBG_NEW PHBallJointMotor();
	//motor->joint = this;
}
/*
void PHBallJoint::Setup(){
	PHJoint::Setup();
	if (limit) { limit->Setup(); }
}

void PHBallJoint::IterateGS(){
	PHJoint::IterateGS();
	if (limit) { limit->IterateGS(); }
}
*/
void PHBallJoint::CompBias(){
	//	並進誤差の解消のため、速度に誤差/dtを加算, Xjrel.r: ソケットに対するプラグの位置のズレ
	dv.v() = Xjrel.r * GetScene()->GetTimeStepInv() * engine->velCorrectionRate;
}

void PHBallJoint::UpdateJointState(){
	// positionの更新：BallJointの position はSwingTwist座標系の角度値とする

	// Swing角の計算
	Vec3d  lD = Vec3d(); if(limit){ lD = limit->GetLimitDir(); }
	Vec3d  ez = Xjrel.q * Vec3d(0.0, 0.0, 1.0);
	double  c = dot(lD, ez); c = max(-1.0, min(c, 1.0));
	position[0] = acos(c);

	// Swing方位角の計算
	if (ez.x == 0) {
		position[1] = (ez.y >= 0) ? M_PI/2.0 : 3*M_PI/2.0;
	} else {
		position[1] = atan(ez.y / ez.x);
		if (ez.x < 0) {
			position[1] +=   M_PI;
		} else if (ez.x > 0 && ez.y < 0) {
			position[1] += 2*M_PI;
		}
	}

	// Twist角の計算
	Quaterniond qSwing;
	Vec3d  halfEz = 0.5*(Vec3d(0,0,1) + ez);
	double l = halfEz.norm();
	if (l > 1e-20) {
		qSwing.V() = cross(halfEz/l, Vec3d(0,0,1));
		qSwing.W() = sqrt(1 - qSwing.V().square());
	} else {
		qSwing.V() = Vec3d(1,0,0);
		qSwing.W() = 0;
	}
	Quaterniond qTwist = qSwing * Xjrel.q;
	position[2] = qTwist.Theta();
	if (qTwist.z < 0) { position[2] *= -1; } ///< Twist回転軸が反対を向くことがあるのでその対策
	if (position[2] < -M_PI) { position[2] += 2*M_PI; }
	if (position[2] >  M_PI) { position[2] -= 2*M_PI; }
}

void PHBallJoint::CompError(){
	B.v() = Xjrel.r;
}

}
