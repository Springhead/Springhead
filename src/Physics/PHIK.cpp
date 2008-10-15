/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "PHIK.h"
#include "Physics/PHJoint.h"

using namespace std;
namespace Spr{

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKEngine

void PHIKEngine::Step(){
	if (nodes.size() > 0 && controlpoints.size() > 0) {
		for(size_t i=0; i<nodes.size(); ++i){
			nodes[i]->PrepareSolve();
		}

		for(size_t n=0; n<numIter; n++){
			for(size_t i=0; i<nodes.size(); ++i){
				nodes[i]->ProceedSolve();
			}
		}

		for(size_t i=0; i<nodes.size(); ++i){
			nodes[i]->Move();
		}
	}
}

void PHIKEngine::Clear(){
	for(size_t i=0; i<nodes.size(); ++i){
		PHIKNode* nd = nodes[i]->Cast();
		delete nd;
	}
	nodes.clear();

	for(size_t i=0; i<controlpoints.size(); ++i){
		PHIKControlPoint* ct = controlpoints[i]->Cast();
		delete ct;
	}
	controlpoints.clear();
}

PHIKNode* PHIKEngine::CreateIKNode(const IfInfo* ii, const PHIKNodeDesc& desc){
	PHIKNode* iknode = NULL;
	if (ii == PHIKSolidIf::GetIfInfoStatic()) {
		iknode = DBG_NEW PHIKSolid();
		DCAST(PHIKSolidIf,iknode)->SetDesc(&desc);
	} else if (ii == PHIKBallJointIf::GetIfInfoStatic()) {
		iknode = DBG_NEW PHIKBallJoint();
		DCAST(PHIKBallJointIf,iknode)->SetDesc(&desc);
	} else if (ii == PHIKHingeJointIf::GetIfInfoStatic()) {
		iknode = DBG_NEW PHIKHingeJoint();
		DCAST(PHIKHingeJointIf,iknode)->SetDesc(&desc);
	}
	AddChildObject(iknode->Cast());
	return iknode;
}

PHIKControlPoint* PHIKEngine::CreateIKControlPoint(const IfInfo* ii, const PHIKControlPointDesc& desc){
	PHIKControlPoint* ikcontrolpoint = NULL;
	if (ii == PHIKPosCtlIf::GetIfInfoStatic()) {
		ikcontrolpoint = DBG_NEW PHIKPosCtl();
		DCAST(PHIKPosCtlIf,ikcontrolpoint)->SetDesc(&desc);
	} else if (ii == PHIKOriCtlIf::GetIfInfoStatic()) {
		ikcontrolpoint = DBG_NEW PHIKOriCtl();
		DCAST(PHIKOriCtlIf,ikcontrolpoint)->SetDesc(&desc);
	}
	AddChildObject(ikcontrolpoint->Cast());
	return ikcontrolpoint;
}

bool PHIKEngine::AddChildObject(ObjectIf* o){
	// --- --- --- --- --- --- ---
	// ノードの場合
	PHIKSolidIf* so = o->Cast();
	if(so){
		nodes.push_back(so);
		so->SetNumber(nodes.size()-1);
		return true;
	}

	PHIKBallJointIf* bj = o->Cast();
	if(bj){
		nodes.push_back(bj);
		bj->SetNumber(nodes.size()-1);
		return true;
	}

	PHIKHingeJointIf* hj = o->Cast();
	if(hj){
		nodes.push_back(hj);
		hj->SetNumber(nodes.size()-1);
		return true;
	}

	// --- --- --- --- --- --- ---
	// 制御点の場合
	PHIKPosCtlIf* pc = o->Cast();
	if(pc){
		controlpoints.push_back(pc);
		pc->SetNumber(controlpoints.size()-1);
		return true;
	}

	PHIKOriCtlIf* oc = o->Cast();
	if(pc){
		controlpoints.push_back(oc);
		oc->SetNumber(controlpoints.size()-1);
		return true;
	}

	return false;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKControlPoint

// --- --- --- --- ---
Vec3d PHIKPosCtl::GetTmpGoal(){
	Vec3d spos = solid->GetPose()*pos;
	Vec3d dir = goal - spos;
	double epsilon = 0.5;
	if (dir.norm() < epsilon) {
		return(dir);
	} else {
		return(dir/dir.norm()*epsilon);
	}
}

// --- --- --- --- ---
Vec3d PHIKOriCtl::GetTmpGoal(){
	Vec3d sorieul; solid->GetPose().Ori().ToEuler(sorieul);
	Vec3d dir = goal - sorieul;
	double epsilon = 0.5;
	if (dir.norm() < epsilon) {
		return(dir);
	} else {
		return(dir/dir.norm()*epsilon);
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKNode

// --- --- --- --- ---
Matrix3d CrossMatrix(Vec3d v){
	Matrix3d c = Matrix3d();
						c[0][1] = -v[2];	c[0][2] =  v[1];
	c[1][0] =  v[2];						c[1][2] = -v[0];
	c[2][0] = -v[1];	c[2][1] =  v[0];
	return c;
}

void PHIKNode::SetNDOF(int n){
	ndof = n;
	iDx.resize(ndof);
	iD.resize(ndof);
	F.resize(ndof, ndof);
	for(size_t i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
	dTheta.resize(ndof);
}

void PHIKNode::PrepareSolve(){
	PTM::VMatrixRow<double>	JtJ; JtJ.resize(ndof,ndof);	JtJ.clear();
	PTM::VVector<double>	Jtx; Jtx.resize(ndof);		Jtx.clear();

	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		PTM::VMatrixRow<double> J; J.resize(ndof,ndof);
		J = CalcJacobian(*ctlpt) * sqrt(bias);
		JtJ += J.trans() * J;
		Jtx += J.trans() * DCAST(PHIKControlPoint,(*ctlpt))->GetTmpGoal();
	}

	for(int i=0; i<ndof; i++){
		iD[i]  = 1.0 / JtJ[i][i];
		iDx[i] = iD[i] * Jtx[i];
	}

	int node_i=0;
	for(NSetIter node=linkedNodes.begin(); node!=linkedNodes.end(); ++node){
		K[node_i].clear();
		for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
			K[node_i] += (CalcJacobian(*ctlpt).trans() * DCAST(PHIKNode,(*node))->CalcJacobian(*ctlpt));
		}
		++node_i;
	}

	dTheta.clear();
}

void PHIKNode::ProceedSolve(){
	PTM::VVector<double> Kv; Kv.resize(ndof); Kv.clear();
	int node_i=0;
	for(NSetIter node=linkedNodes.begin(); node!=linkedNodes.end(); ++node){
		Kv += (K[node_i] * DCAST(PHIKNode,(*node))->dTheta);
		++node_i;
	}
	PTM::VVector<double> B; B.resize(ndof); B.clear();
	B = F*dTheta + Kv;
	for(int i=0; i<ndof; i++){ B[i] *= iD[i]; }
	dTheta = iDx - B;
}

void PHIKNode::AddControlPoint(PHIKControlPointIf* control){
	NSet* ln = &(DCAST(PHIKControlPoint,control)->linkedNodes);
	for(NSetIter node=ln->begin(); node!=ln->end(); ++node){
		linkedNodes.insert(*node);
	}
	DCAST(PHIKControlPoint,control)->linkedNodes.insert(this->Cast());
	linkedControlPoints.insert(control);
	K.resize(linkedNodes.size());
	for(unsigned int i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
}

PTM::VMatrixRow<double> PHIKNode::CalcJacobian(PHIKControlPointIf* control){
	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> PHIKSolid::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();
		M[0][0]=1; M[1][1]=1; M[2][2]=1;
		return M;
	}
 
	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3);
		Vec3d Pm = solid->GetPose().Pos();
		PHIKOriCtl* cp = DCAST(PHIKOriCtl,cpOri);
		Vec3d Pc = cp->solid->GetPose().Pos();
		double norm = (Pm-Pc).norm();
		M = CrossMatrix((Pm-Pc)/(norm*norm));
		return M;
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> PHIKBallJoint::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3);
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pm = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
		PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;
		M = CrossMatrix(Pm-Pc);
		return M;	 
	}

	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();
		M[0][0]=1; M[1][1]=1; M[2][2]=1;
		return M;
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> PHIKHingeJoint::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,1);
		PHHingeJoint* j = DCAST(PHHingeJoint,joint);
		Vec3d Pm = j->solid[0]->GetPose() * j->Xj[0].r;
		Vec3d Rm = j->solid[0]->GetPose().Ori() * j->Xj[0].q * Vec3d(0,0,1);
		PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;
		Vec3d M3 = PTM::cross((Pm-Pc), Rm);
		M[0][0]=M3[0]; M[0][1]=M3[1]; M[0][2]=M3[2];
		return M;
	}
 
	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,1); return M; /// 適切なのを返す
	}

	return PTM::VMatrixRow<double>();
}

void PHIKHingeJoint::Move(){
	static const double Pi = 3.141592653589;
	double angle = joint->GetPosition() + (dTheta[0]);
	while (angle > +2*Pi) { angle -= 2*Pi; }
	while (angle < -2*Pi) { angle += 2*Pi; }
	joint->SetSpringOrigin(angle);
}

}
