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
	// tazzさんのメモの(11)式、軸[cos(psi), sin(psi), 0]^Tまわりにthetaだけ回転した後Z軸まわりにphi回転するquaternion
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
	// tazzさんのメモの(12)式、item[0]:psi, item[1]:theta, item[2]:phi
	item(0) = atan2(q.w * q.y + q.x * q.z, q.w * q.x - q.y * q.z);
	item(1) = 2 * atan2(sqrt(q.x * q.x + q.y * q.y), sqrt(q.w * q.w + q.z * q.z));
	item(2) = 2 * atan2(q.z, q.w);
}
void SwingTwist::Jacobian(Matrix3d& J){
	// tazzさんのメモの(13)式、st=[psi, theta, phi]^Tの時間微分から角速度ωを与えるヤコビアンJ (ω = J * (d/dt)st)
	double psi = SwingDir();
	double the = max(Rad(1.0), Swing());	// スイング角0でランク落ちするので無理やり回避
	double cos_psi = cos(psi), sin_psi = sin(psi);
	double cos_the = cos(the), sin_the = sin(the);
	J[0][0] = -sin_the * sin_psi;
	J[0][1] =  cos_psi;
	J[0][2] =  sin_the * sin_psi;
	J[1][0] =  sin_the * cos_psi;
	J[1][1] =  sin_psi;
	J[1][2] = -sin_the * cos_psi;
	J[2][0] =  1.0 - cos_the;
	J[2][1] =  0.0;
	J[2][2] =  cos_the;
}
void SwingTwist::Coriolis(Vec3d& c, const Vec3d& sd){
	double cos_psi = cos(SwingDir()), sin_psi = sin(SwingDir());
	double cos_the = cos(Swing()), sin_the = sin(Swing());
	double tmp1 = sd[1] * (sd[0] - sd[2]);
	double tmp2 = sd[0] * (sd[0] - sd[2]);
	double tmp3 = sd[0] * sd[1];
	c[0] = -cos_the * sin_psi * tmp1 - sin_the * cos_psi * tmp2 - sin_psi * tmp3;
	c[1] =  cos_the * cos_psi * tmp1 - sin_the * sin_psi * tmp2 + cos_psi * tmp3;
	c[2] =  sin_the * tmp1;	
}
void SwingTwist::JacobianInverse(Matrix3d& J, const Quaterniond& q){
	// tazzさんのメモの(14)式、角速度ωからst=[psi, theta, phi]^Tの時間微分を求めるヤコビアンJInv ((d/dt)st = JInv * ω)
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

PHBallJoint::PHBallJoint(const PHBallJointDesc& desc){
	SetDesc(&desc);
	axisIndex[0] = 3;
	axisIndex[1] = 4;
	axisIndex[2] = 5;
}
void PHBallJoint::SetDesc(const void* desc){
	PHConstraint::SetDesc(desc);
	const PHBallJointDesc& descBall = *(const PHBallJointDesc*)desc;
	swingUpper  = descBall.swingUpper;
	twistLower  = descBall.twistLower;
	twistUpper  = descBall.twistUpper;
	spring      = descBall.spring;
	origin      = descBall.origin;
	damper      = descBall.damper;
	SetMotorTorque(descBall.torque);
}

void PHBallJoint::UpdateJointState(){
	// 相対quaternionからスイング・ツイスト角を計算
	position = Xjrel.q.V();								/// Xjrel:ソケットに対するプラグの位置と向きのクォータニオン
	angle.FromQuaternion(Xjrel.q);
	angle.JacobianInverse(Jstinv, Xjrel.q);
	velocity = Xjrel.q.Derivative(vjrel.w()).V();
}

void PHBallJoint::SetConstrainedIndex(bool* con){
//	DSTR << "ST=" << angle << std::endl;
//	DSTR << "JI=" << Jstinv << std::endl;
	con[0] = con[1] = con[2] = true;
	// 可動範囲をチェック
	swingOnUpper = (swingUpper > 0 && angle.Swing() >= swingUpper);
	twistOnLower = (twistLower <= twistUpper && angle.Twist() <= twistLower);
	twistOnUpper = (twistLower <= twistUpper && angle.Twist() >= twistUpper);
	// 以下3 -> swing方位，4 -> swing角, 5 -> twist角
	con[3] = false || (spring != 0.0 || damper != 0.0);
	con[4] = swingOnUpper || (spring != 0.0 || damper != 0.0);
	con[5] = twistOnLower || twistOnUpper || (spring != 0.0 || damper != 0.0);
}

// ヤコビアンの角速度部分を座標変換してSwingTwist角の時間変化率へのヤコビアンにする
void PHBallJoint::ModifyJacobian(){
	J[0].wv() = Jstinv * J[0].wv();
	J[0].ww() = Jstinv * J[0].ww();
	J[1].wv() = Jstinv * J[1].wv();
	J[1].ww() = Jstinv * J[1].ww();
}

void PHBallJoint::CompBias(){
	double dtinv = 1.0 / scene->GetTimeStep();
	db.v() = Xjrel.r * dtinv;		//	並進誤差の解消のため、速度に誤差/dtを加算
	db.w()[0] = 0.0;
	db.w()[1] = (swingOnUpper ? (angle.Swing() - swingUpper) * dtinv : 0.0);
	db.w()[2] = (twistOnLower ? (angle.Twist() - twistLower) * dtinv :
			     twistOnUpper ? (angle.Twist() - twistUpper) * dtinv : 0.0);
	db *= engine->velCorrectionRate;
	if(spring != 0.0 || damper != 0.0){	//	バネダンパはSwingTwist座標系で働く
		Quaterniond diff =  Xjrel.q * origin.Inv();	//	originからみた q
		Vec3d prop = origin * diff.RotationHalf();	//	外部座標系から見た 回転ベクトル
		prop = Jstinv * prop;						//	回転軸ベクトルをSwingTwistに変換
		double tmp = 1.0 / (damper + spring * scene->GetTimeStep());
		dA.w()[0] = tmp * dtinv;
		db.w()[0] = spring * prop[0] * tmp;
		if (!swingOnUpper){
			dA.w()[1] = tmp * dtinv;
			db.w()[1] = spring * prop[1] * tmp;
		}
		if (!twistOnLower && !twistOnUpper ){
			dA.w()[2] = tmp * dtinv;
			db.w()[2] = spring * prop[1] * tmp;
		}
	}
}

void PHBallJoint::CompError(){
	B.v() = Xjrel.r;
	B.w()[0] = 0.0;
	B.w()[1] = (swingOnUpper ? (angle.Swing() - swingUpper) : 0.0);
	B.w()[2] = (twistOnLower ? (angle.Twist() - twistLower) :
			    twistOnUpper ? (angle.Twist() - twistUpper) : 0.0);
}

void PHBallJoint::Projection(double& f, int k){
	if(k == 4 && swingOnUpper)
		f = min(0.0, f);
	if(k == 5 && twistLower < twistUpper){
		if(twistOnLower)
			f = max(0.0, f);
		if(twistOnUpper)
			f = min(0.0, f);
	}
}

//----------------------------------------------------------------------------
// PHBallJointNode
IF_OBJECT_IMP(PHBallJointNode, PHTreeNode)

void PHBallJointNode::CompJointJacobian(){
	PHBallJoint* j = GetJoint();
	//SwingTwist& angle = (SwingTwist&)(j->position);
	//angle.Jacobian(Jst);
	//Matrix3d test = Jst * Jstinv;
	Quaterniond q = j->Xjrel.q;
	for(int i = 0; i < 3; i++)
		J.col(i).SUBVEC(0, 3).clear();
	/*J[0].w() = 2.0 * Vec3d(-q.x, -q.y, -q.z);
	J[1].w() = 2.0 * Vec3d( q.w,  q.z, -q.y);
    J[2].w() = 2.0 * Vec3d(-q.z,  q.w,  q.x);
    J[3].w() = 2.0 * Vec3d( q.y, -q.x,  q.w);*/
	J.col(0).SUBVEC(3, 3) = Vec3d(1.0, 0.0, 0.0);
	J.col(1).SUBVEC(3, 3) = Vec3d(0.0, 1.0, 0.0);
	J.col(2).SUBVEC(3, 3) = Vec3d(0.0, 0.0, 1.0);
	PHTreeNodeND<3>::CompJointJacobian();
}
void PHBallJointNode::CompJointCoriolisAccel(){
	//PHBallJoint* j = GetJoint();
	//cj.v().clear();
	//((SwingTwist&)(j->position)).Coriolis(cj.w(), j->velocity);
	//cj.w.clear();
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
	//j->Xjrel.qはUpdateJointPositionで更新済み
}
void PHBallJointNode::CompRelativeVelocity(){
	PHBallJoint* j = GetJoint();
	j->vjrel.v().clear();
	//j->vjrel.w() = ((Quaterniond&)j->position).AngularVelocity((Quaterniond&)j->velocity);
	j->vjrel.w() = j->velocity;
}

void PHBallJointNode::ModifyJacobian(){
	PHBallJoint* j = GetJoint();
	Jq = j->Jstinv;
}

void PHBallJointNode::CompBias(){
	dA.clear();
	db.clear();
}
void PHBallJointNode::Projection(double& f, int k){
	PHBallJoint* j = GetJoint();
	if(k == 1 && j->swingOnUpper)
		f = min(0.0, f);
	if(k == 2 && j->twistLower < j->twistUpper){
		if(j->twistOnLower)
			f = max(0.0, f);
		if(j->twistOnUpper)
			f = min(0.0, f);
	}
}

}
