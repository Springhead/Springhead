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
// PHBallJoint
IF_OBJECT_IMP(PHBallJoint, PHJoint)

PHBallJoint::PHBallJoint(const PHBallJointDesc& desc){
	SetDesc(&desc);
	axisIndex[0] = 3;
	axisIndex[1] = 4;
	axisIndex[2] = 5;
	
	// limit上に居るかどうかのフラグの初期化
	for(int i=0; i<3; ++i){
		onLimit[i].onLower = 0;
		onLimit[i].onUpper = 0;
	}
}

bool PHBallJoint::GetDesc(void* desc){
	PHConstraint::GetDesc(desc);
	((PHBallJointDesc*)desc)->spring		 = spring;
	((PHBallJointDesc*)desc)->damper		 = damper;
	((PHBallJointDesc*)desc)->limit			 = limit;
	((PHBallJointDesc*)desc)->goalVector	 = goalVector;
	((PHBallJointDesc*)desc)->torque		 = GetMotorTorque();
	return true;
}

void PHBallJoint::SetDesc(const void* desc){
	PHConstraint::SetDesc(desc);
	const PHBallJointDesc& descBall = *(const PHBallJointDesc*)desc;
	limit = descBall.limit;
	spring      = descBall.spring;
	//goal	    = descBall.goal;		//(SwingTwist)
	goalVector  = descBall.goalVector; 
	damper      = descBall.damper;
	SetMotorTorque(descBall.torque);
}

void PHBallJoint::UpdateJointState(){

	currentVector = Xjrel.q.Conjugated() * Quaterniond(0.0, 0.0, 0.0, 1.0) * Xjrel.q;

}

void PHBallJoint::SetConstrainedIndex(bool* con){
	con[0] = con[1] = con[2] = true;
	// 可動範囲をチェック
	float nowTheta;
	goalVector  = Xj[0].q * Vec3f(0.0, 0.0, 1.0);
	goalVector  = Xj[0].q.Conjugated() * goalVector;
	
	//DSTR << "Xj[0].q.Conjugated : " << Xj[0].q.Conjugated() << endl;
	//DSTR << "Xj[0].q : " << Xj[0].q << endl;
	//DSTR << "goalVector : " << goalVector << endl;

	nowTheta = acos(dot(goalVector.unit(), currentVector.unit()));			//"vector".unit():<Vector>の単位ベクトル
	
	//cout << "nowTheta" << nowTheta << endl;
	DSTR << "nowTheta" << nowTheta << endl;
	
	for(int i=0; i<3; ++i){
		if(nowTheta > limit.upper[i]){
			onLimit[i].onUpper = 1;
		}
		else if(nowTheta < limit.lower[i]){
			onLimit[i].onLower  = 1;
		}
		else{
			onLimit[i].onUpper = 0;
			onLimit[i].onLower = 0;
		}

		DSTR << "onLimit[" << i << "].onUpper : " << onLimit[i].onUpper << endl;
		DSTR << "onLimit[" << i << "].onLower : " << onLimit[i].onLower << endl;
	
	}

	for(int i=0; i<3;++i){
		con[i+3] = onLimit[i].onUpper || onLimit[i].onLower || spring[i]!=0.0 || damper[i] != 0.0;
	}

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
	db.v() = Xjrel.r * dtinv;		//	並進誤差の解消のため、速度に誤差/dtを加算, Xjrel.r: ソケットに対するプラグの位置
	
	for(int i=0; i<3; ++i){
		db.w()[i] = (onLimit[i].onLower ? (nowTheta[i] - limit.lower[i]) * dtinv :
			     onLimit[i].onUpper ? (nowTheta[i] - limit.upper[i]) * dtinv : 0.0);
	}
	db *= engine->velCorrectionRate;
	Vec3d prop = nowTheta - goalVector;

	for(int i=0; i<3; ++i){
		if (onLimit[i].onLower || onLimit[i].onUpper) continue;
		if (spring[i] != 0.0 || damper[i] != 0.0){	//	バネダンパはSwingTwist座標系で働く
			double tmp = 1.0 / (damper[i] + spring[i] * scene->GetTimeStep());
			dA.w()[i] = tmp * dtinv;
			db.w()[i] = spring[i] * prop[i] * tmp;
		}
	}

}

void PHBallJoint::CompError(){
	B.v() = Xjrel.r;
	for(int i=0; i<3; ++i){
		B.w()[i] = (onLimit[i].onLower ? (nowTheta[i] - limit.lower[i]) :
			onLimit[2].onUpper ? (nowTheta[i] - limit.upper[i]) : 0.0);
	}
}

void PHBallJoint::Projection(double& f, int k){
	if (3<=k){
		if(onLimit[k-3].onLower)
			f = max(0.0, f);
		if(onLimit[k-3].onUpper)
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
	if(j->onLimit[k].onLower)
		f = max(0.0, f);
	if(j->onLimit[k].onUpper)
		f = min(0.0, f);
}


}