/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHGenericJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHGenericJointNode

void PHGenericJointNode::CompJointJacobian(){
	Vec3d Jv, Jw;
	PHGenericJoint* jnt = GetJoint();
	jnt->callback->CompJointJacobian(jnt->Cast(), Jv, Jw, jnt->position[0]);
	J.col(0)[0] = Jv[0];
	J.col(0)[1] = Jv[1];
	J.col(0)[2] = Jv[2];
	J.col(0)[3] = Jw[0];
	J.col(0)[4] = Jw[1];
	J.col(0)[5] = Jw[2];
	PHTreeNode1D::CompJointJacobian();
}	

void PHGenericJointNode::CompJointCoriolisAccel() {
	Vec3d cjv, cjw;
	PHGenericJoint* jnt = GetJoint();
	jnt->callback->CompJointCoriolisAccel(jnt->Cast(), cjv, cjw, jnt->position[0], jnt->velocity[0]);
	cj.v() = cjv;
	cj.w() = cjw;
}

void PHGenericJointNode::CompRelativeVelocity(){
	Vec3d vrel, wrel;
	PHGenericJoint* jnt = GetJoint();
	jnt->callback->CompRelativeVelocity(jnt->Cast(), vrel, wrel, jnt->position[0], jnt->velocity[0]);
	jnt->vjrel.v() = vrel;
	jnt->vjrel.w() = wrel;
}

void PHGenericJointNode::CompRelativePosition(){
	PHGenericJoint* jnt = GetJoint();
	jnt->callback->CompRelativePosition(jnt->Cast(), jnt->Xjrel.r, jnt->Xjrel.q, jnt->position[0]);
}

void PHGenericJointNode::UpdateJointPosition(double dt){
	PHTreeNode1D::UpdateJointPosition(dt);
	PHGenericJoint* jnt = GetJoint();
	if(jnt->callback->IsCyclic(jnt->Cast())){
		double p = jnt->position[0];
		while(p >  M_PI) p -= 2 * M_PI;
		while(p < -M_PI) p += 2 * M_PI;
		jnt->position[0] = p;
	}
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHGenericJoint

PHGenericJoint::PHGenericJoint(const PHGenericJointDesc& desc) {
	SetDesc(&desc);
}

void PHGenericJoint::SetCallback(PHGenericJointCallback* cb){
	callback = cb;

	// 可動自由度を取得
	int n = 1;
	int indices[6];
	indices[0] = 5;
	callback->GetMovableAxes(Cast(), n, indices);

	for(int i = 0; i < n; i++)
		movableAxes.Enable(indices[i]);
	
}

void PHGenericJoint::SetParam(const string& name, double value){
	callback->SetParam(Cast(), name, value);
}

void PHGenericJoint::UpdateJointState(){
	callback->UpdateJointState(Cast(), position[0], velocity[0], Xjrel.r, Xjrel.q, vjrel.v(), vjrel.w());
}

void PHGenericJoint::CompBias(){
	Vec3d dbv, dbw;
	callback->CompBias(Cast(), dbv, dbw, Xjrel.r, Xjrel.q, vjrel.v(), vjrel.w());
	db.v() = dbv;
	db.w() = dbw;

	//PH1DJoint::CompBias();
}

void PHGenericJoint::CompError(){
	Vec3d Bv, Bw;
	callback->CompError(Cast(), Bv, Bw, Xjrel.r, Xjrel.q);
	B.v() = Bv;
	B.w() = Bw;
}

double PHGenericJoint::GetDeviation(){
	double diff = PH1DJoint::GetDeviation();
	if(callback->IsCyclic(Cast())){
		while(diff >  M_PI) diff -= 2 * M_PI;
		while(diff < -M_PI) diff += 2 * M_PI;
	}
	return diff;
}

}
