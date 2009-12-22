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
#include "Physics.h"
#include <fstream>
#pragma hdrstop

using namespace PTM;
using namespace std;

namespace Spr{;
//----------------------------------------------------------------------------
// PHBallJointDesc

PHBallJointDesc::PHBallJointDesc(){
	spring			= 0.0;
	damper			= 0.0;
	limitSwing[0]	= FLT_MAX;
	limitSwing[1]	= FLT_MAX;
	limitTwist[0]	= FLT_MAX;
	limitTwist[1]	= FLT_MAX;	
	limitDir		= Vec3d(0.0, 0.0, 1.0);
	targetPosition			= Quaterniond(1, 0, 0, 0);
	fMax			= FLT_MAX;
	poleTwist		= Vec2d(FLT_MAX,FLT_MAX);
	offsetForce		= Vec3d();

	secondDamper	= 0.0;
	yieldStress		= 0.0;
	hardnessRate	= 1.0;
	Inertia			= Vec3d(1.0,1.0,1.0);
	type			= ELASTIC;
}

//----------------------------------------------------------------------------
// PHBallJoint
PHBallJoint::PHBallJoint(const PHBallJointDesc& desc){
	SetDesc(&desc);
	axisIndex[0] = 3;
	axisIndex[1] = 4;
	axisIndex[2] = 5;

	limit.joint = this;
	motor.joint = this;

}

void PHBallJoint::SetupLCP(){
	PHJoint::SetupLCP();
//	limit.SetupLCP(); //円形での拘束はなおりました
	motor.SetupLCP();
}

void PHBallJoint::IterateLCP(){
	PHJoint::IterateLCP();
//	limit.IterateLCP(); //円形での拘束はなおりました
	motor.IterateLCP();
}

void PHBallJoint::CompBias(){
	//	並進誤差の解消のため、速度に誤差/dtを加算, Xjrel.r: ソケットに対するプラグの位置のズレ
	db.v() = Xjrel.r * GetScene()->GetTimeStepInv();
	db.v() *= engine->velCorrectionRate;
}

void PHBallJoint::UpdateJointState(){
	// Jc.Ez() : Socketに対するPlugの向いている方向(旧currentVector)
	Jc.Ez() = Xjrel.q * Vec3d(0.0, 0.0, 1.0);
	const double eps = 1.0e-10;
	Vec3d tmp = Jc.Ez() - limitDir;
	double n = tmp.square();
	Jc.Ex() = (n > eps ? cross(Jc.Ez(), tmp).unit() : Xjrel.q * Vec3d(1.0, 0.0, 0.0));
	Jc.Ey() = cross(Jc.Ez(), Jc.Ex());
	Jcinv   = Jc.trans();
}

void PHBallJoint::CompError(){
	B.v() = Xjrel.r;
}

PHJointDesc::PHDeformationType PHBallJoint::GetDeformationMode(){
	switch(type){
	case PHBallJointDesc::ELASTIC_PLASTIC:
		if(motor.yieldFlag)return PHBallJointDesc::PLASTIC;
		else  return PHBallJointDesc::ELASTIC;
	default: return type;
	}
}


//----------------------------------------------------------------------------
// PHBallJointNode
void PHBallJointNode::CompJointJacobian(){
	PHBallJoint* j = GetJoint();
	//SwingTwist& angle = (SwingTwist&)(j->position);
	//angle.Jacobian(Jst);
	//Matrix3d test = Jst * Jcinv;
	Quaterniond q = j->Xjrel.q;
	for(int i = 0; i < 3; i++)
		J.col(i).sub_vector(PTM::TSubVectorDim<0,3>()).clear();
	/*J[0].w() = 2.0 * Vec3d(-q.x, -q.y, -q.z);
	J[1].w() = 2.0 * Vec3d( q.w,  q.z, -q.y);
    J[2].w() = 2.0 * Vec3d(-q.z,  q.w,  q.x);
    J[3].w() = 2.0 * Vec3d( q.y, -q.x,  q.w);*/
	J.col(0).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(1.0, 0.0, 0.0);
	J.col(1).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(0.0, 1.0, 0.0);
	J.col(2).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(0.0, 0.0, 1.0);
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

}
