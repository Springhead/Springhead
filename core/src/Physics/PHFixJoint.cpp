/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFixJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHFixJointNode

void PHFixJointNode::CompJointCoriolisAccel() {
	cj.clear();
}

void PHFixJointNode::CompRelativeVelocity(){
	joint->vjrel.clear();
}

void PHFixJointNode::CompRelativePosition(){
	joint->Xjrel.r.clear();
	joint->Xjrel.q = Quaterniond();
}

void PHFixJointNode::AccumulateInertia(){
	(Matrix6d&)XIX = Xcp_mat.trans() * I * Xcp_mat;
	parent->I += XIX;
}
void PHFixJointNode::AccumulateBiasForce(){
	(Vec6d&)XtrZplusIc = Xcp_mat.trans() * ZplusIc;
	parent->Z += XtrZplusIc;
}
void PHFixJointNode::CompAccel(){
	solid->dv = Xcp_mat * parent->solid->dv + c;
	
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccel();
}

void PHFixJointNode::CompResponseMatrix(){
	Tcp = Xcp_mat;
	PHTreeNode::CompResponseMatrix();
}

void PHFixJointNode::CompResponseMap(){
	int n = (int)root->nodes.size();

	PHTreeNode* node;	
	for(int i = 0; i < n; i++){
		node = root->nodes[i];
		
		if(!solid->IsDynamical()){
			dZdv_map[i].clear();
		}
		else if(i == id){
			(Matrix6d&)dZdv_map[i] = Tcp * parent->dZdv_map[parent->id] * Tcp.trans();
		}
		else if(depth < node->depth){
			(Matrix6d&)dZdv_map[i] = dZdv_map[node->GetParent()->id] * node->Tcp.trans();
		}
		else{
			(Matrix6d&)dZdv_map[i] = Tcp * parent->dZdv_map[node->id];
		}
	}

	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompResponseMap();
}

void PHFixJointNode::CompResponse(PHTreeNode* src, const SpatialVector& df){
	solid->dv += dZdv_map[src->id] * (-df);
}

void PHFixJointNode::CompResponseCorrection(PHTreeNode* src, const SpatialVector& dF){
	solid->dV += dZdv_map[src->id] * (-dF);
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHFixJoint

PHFixJoint::PHFixJoint(const PHFixJointDesc& desc) {
	SetDesc(&desc);
	
	movableAxes.Clear();
}

void PHFixJoint::CompBias(){
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	db.v() = Xjrel.r * dtinv;
	db.w() = Xjrel.q.RotationHalf() * dtinv;
	db *= engine->velCorrectionRate;
}

}
