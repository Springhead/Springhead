/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// SwingTwist

void SwingTwist::ToQuaternion(Quaterniond& q){
	double psi = SwingDir(), the = Swing(), phi = Twist();
	double cos_the = cos(the / 2), sin_the = sin(the / 2);
	double cos_phi = cos(phi / 2), sin_phi = sin(phi / 2);
	double cos_psiphi = cos(psi - phi / 2), sin_psiphi = sin(psi - phi / 2);
	q.w = cos_the * cos_phi;
	q.x = sin_the * cos_psiphi;
	q.y = sin_the * sin_psiphi;
	q.z = cos_the * sin_phi;
}
void SwingTwist::FromQuaternion(const Quaterniond& q){
	item(0) = atan2(q.w * q.y + q.x * q.z, q.w * q.x - q.y * q.z);
	item(1) = 2 * atan2(sqrt(q.x * q.x + q.y * q.y), sqrt(q.w * q.w + q.z * q.z));
	item(2) = 2 * atan2(q.z, q.w);
}
void SwingTwist::Jacobian(Matrix3d& J, const Quaterniond& q){
	const double eps = 1.0e-12;
	double w2z2 = max(eps, q.w * q.w + q.z * q.z);
	double w2z2inv = 1.0 / w2z2;
	double x2y2 = max(eps, 1.0 - w2z2);
	double x2y2inv = 1.0 / x2y2;
	double tmp = sqrt(w2z2inv * x2y2inv);
	double wy_xz = q.w * q.y + q.x * q.z;
	double wx_yz = q.w * q.x - q.y * q.z;
	J[0][0] =  0.5 * (w2z2inv - x2y2inv) * wy_xz;
	J[0][1] = -0.5 * (w2z2inv - x2y2inv) * wx_yz;
	J[0][2] =  1.0;
	J[1][0] =  tmp * wx_yz;
	J[1][1] =  tmp * wy_xz;
	J[1][2] =  0.0;
	J[2][0] =  w2z2inv * wy_xz;
	J[2][1] = -w2z2inv * wx_yz;
	J[2][2] =  1.0;
}

//----------------------------------------------------------------------------
// PHBallJoint
IF_OBJECT_IMP(PHBallJoint, PHJoint)

void PHBallJoint::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHBallJointDesc& descBall = (const PHBallJointDesc&)desc;
	swingUpper  = descBall.swingUpper;
	swingSpring = descBall.swingSpring;
	swingDamper = descBall.swingDamper;
	twistLower  = descBall.twistLower;
	twistUpper  = descBall.twistUpper;
	twistSpring = descBall.twistSpring;
	twistDamper = descBall.twistDamper;
	SetMotorTorque(descBall.torque);
}

void PHBallJoint::UpdateJointState(){
	// 相対quaternionからスイング・ツイスト角を計算
	angle.FromQuaternion(qjrel);
	DSTR << angle << f.w << endl;
}

void PHBallJoint::CompDof(){
	constr[0] = constr[1] = constr[2] = true;
	// 可動範囲をチェック
	angle.FromQuaternion(qjrel);
	swingOnUpper = (swingUpper > 0 && angle.Swing() >= swingUpper);
	twistOnLower = (twistLower < twistUpper && angle.Twist() <= twistLower);
	twistOnUpper = (twistLower < twistUpper && angle.Twist() >= twistUpper);
	// 可動範囲にかかる場合は回転をSwing/Twistに座標変換する(ModifyJacobian)．
	// 以下3 -> swing方位，4 -> swing角, 5 -> twist角
	constr[3] = false;
	if(swingOnUpper){
		constr[4] = true;
	}
	else{
		constr[4] = false;
	}
	constr[5] = twistOnLower || twistOnUpper;
}

// ヤコビアンの角速度部分を座標変換してSwingTwist角の時間変化率へのヤコビアンにする
void PHBallJoint::ModifyJacobian(){
	if(swingOnUpper || twistOnLower || twistOnUpper){
		angle.Jacobian(Jst, qjrel);
		J[0].wv = Jst * J[0].wv;
		J[0].ww = Jst * J[0].ww;
		J[1].wv = Jst * J[1].wv;
		J[1].ww = Jst * J[1].ww;
	}
}

void PHBallJoint::CompBias(){
	double dtinv = 1.0 / scene->GetTimeStep();
	db.v = Xjrel.r * dtinv;
	db.w[0] = 0.0;
	db.w[1] = (swingOnUpper ? (angle.Swing() - swingUpper) * dtinv : 0.0);
	db.w[2] = (twistOnLower ? (angle.Twist() - twistLower) * dtinv :
			   twistOnUpper ? (angle.Twist() - twistUpper) * dtinv : 0.0);
	db *= engine->correctionRate;
}

/*void PHBallJoint::CompError(double dt){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}*/

void PHBallJoint::Projection(double& f, int k){
	if(k == 4 && swingOnUpper)
		f = min(0.0, f);
	if(k == 5){
		if(twistOnLower)
			f = max(0.0, f);
		if(twistOnUpper)
			f = min(0.0, f);
	}
}

//----------------------------------------------------------------------------
// PHBallJointNode

void PHBallJointNode::CompJointJacobian(){

}
void PHBallJointNode::CompJointCoriolisAccel(){

}
void PHBallJointNode::CompRelativePosition(){

}
void PHBallJointNode::CompRelativeVelocity(){

}
	
}
