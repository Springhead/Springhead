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
	// std::cout << " -- " << std::endl;
	if (nodes.size() > 0 && controlpoints.size() > 0) {
		for(size_t i=0; i<nodes.size(); ++i){
			nodes[i]->PrepareSolve();
		}

		for(size_t n=0; n<numIter; n++){
			for(size_t i=0; i<nodes.size(); ++i){
				nodes[i]->ProceedSolve();
			}
			double diff = 0.0f;
			for(size_t i=0; i<nodes.size(); ++i){
				diff += (DCAST(PHIKNode,nodes[i])->dTheta - DCAST(PHIKNode,nodes[i])->dTheta_prev).norm();
				// std::cout << n << "," << i << " : " << DCAST(PHIKNode,nodes[i])->dTheta << std::endl;
			}
			if (n > 0) {
				// std::cout << "diff : " << diff << std::endl;
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
	//AddChildObject(iknode->Cast());
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
	//AddChildObject(ikcontrolpoint->Cast());
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
	double epsilon = 1.0;
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
	double epsilon = 1.0;
	if (dir.norm() < epsilon) {
		return(dir);
	} else {
		return(dir/dir.norm()*epsilon);
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKNode

// --- --- --- --- ---
void PHIKNode::SetNDOF(int n){
	ndof = n;
	iDx.resize(ndof);
	iD.resize(ndof);
	F.resize(ndof, ndof);
	for(size_t i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); }
	dTheta.resize(ndof);
	dTheta_prev.resize(ndof);
	tau.resize(ndof);
}

void PHIKNode::ClearJacobian(){
	Mj.clear();
}

void PHIKNode::CalcAllJacobian(){
	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		int n = DCAST(PHIKControlPoint,*ctlpt)->number;
		Mj[n] = CalcJacobian(*ctlpt);
		// std::cout << Mj[n] << std::endl;
	}
}

void PHIKNode::PrepareSolve(){
	PTM::VMatrixRow<double>	JtJ; JtJ.resize(ndof,ndof);	JtJ.clear();
	PTM::VVector<double>	Jtx; Jtx.resize(ndof);		Jtx.clear();

	tau.clear();

	ClearJacobian();
	CalcAllJacobian();

	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		PTM::VMatrixRow<double> J; J.resize(ndof,ndof);
		// J = CalcJacobian(*ctlpt) * sqrt(bias);
		int n = DCAST(PHIKControlPoint,*ctlpt)->number;
		J = Mj[n] * sqrt(bias);
		JtJ += J.trans() * J;
		Jtx += J.trans() * DCAST(PHIKControlPoint,(*ctlpt))->GetTmpGoal();

		/*
		std::cout << " -- " << std::endl;
		std::cout << "v : " << DCAST(PHIKControlPoint,(*ctlpt))->GetTmpGoal() << std::endl;
		std::cout << " -- " << std::endl;
		*/

		tau += J.trans() * DCAST(PHIKControlPoint,(*ctlpt))->GetForce();
	}

	/*
	for(int i=0; i<ndof; i++){
		iD[i] = JtJ[i][i] + 0.1;
	}
	*/

	for(int i=0; i<ndof; i++){
		if (JtJ[i][i]!=0) {
			iD[i] = 1.0 / JtJ[i][i];
		} else {
			iD[i] = 1e+20;
		}
		iDx[i] = iD[i] * Jtx[i];
	}

	for(int i=0; i<ndof; ++i){
		for(int j=0; j<ndof; ++j){
			if (i!=j) {
				F[i][j] = JtJ[i][j];
			} else {
				F[i][j] = 0;
			}
		}
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
	dTheta_prev.clear();
}

void PHIKNode::ProceedSolve(){
	dTheta_prev = dTheta;

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

	// std::cout << "v' : " << dTheta << std::endl;
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
		M = Matrix3d::Cross((Pm-Pc)/(norm*norm));
		return M;
	}

	return PTM::VMatrixRow<double>();
}

// --- --- --- --- ---
PTM::VMatrixRow<double> PHIKBallJoint::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,2);
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
		PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;

		/*
		std::cout << "Pc : " << Pc << std::endl;
		std::cout << "Pj : " << Pj << std::endl;
		*/

		Vec3d e0 = (Pc - Pj);
		if (e0.norm() != 0){
			e0 = e0 / e0.norm();
		} else {
			e0 = Vec3d(1,0,0);
		}
		
		if (PTM::dot(e0,Vec3d(1,0,0)) > Rad(10)) {
			e1 = Vec3d(0,1,0);
		} else {
			e1 = Vec3d(1,0,0);
		}
		// std::cout << "e1: " << e1 << ", e0: " << e0 << std::endl;
		e1 = e1 - (PTM::dot(e1,e0) * e0);
		// std::cout << "e1: " << e1 << std::endl;
		e1 = e1 / e1.norm();
		e2 = PTM::cross(e0,e1);
		// std::cout << "e2: " << e2 << std::endl;

		/*
		std::cout << "Pc : " << Pc << std::endl;
		std::cout << "Pj : " << Pj << std::endl;
		std::cout << "(Pc-Pj) : " << (Pc-Pj) << std::endl;
		*/

		Vec3d v1 = PTM::cross(e1,(Pc-Pj));
		Vec3d v2 = PTM::cross(e2,(Pc-Pj));

		/*
		std::cout << "v1: " << v1 << std::endl;
		std::cout << "v2: " << v2 << std::endl;
		*/

		M[0][0]=v1[0]; M[1][0]=v1[1]; M[2][0]=v1[2];
		M[0][1]=v2[0]; M[1][1]=v2[1]; M[2][1]=v2[2];

		/*
		std::cout << " -- " << std::endl;
		std::cout << M << std::endl;
		std::cout << " -- " << std::endl;
		*/

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

void PHIKBallJoint::Move(){
	// std::cout << dTheta << std::endl;

	Vec3d dT = dTheta[0] * e1 + dTheta[1] * e2;
	double angle = dT.norm();
	Vec3d axis = dT;
	if (angle != 0) {
		axis = axis / angle;
	} else {
		axis = Vec3d(1,0,0);
	}

	// std::cout << number << " : " << Deg(angle) << std::endl;

	Quaterniond dQ = Quaterniond::Rot(angle, axis);

	PHBallJoint* j = DCAST(PHBallJoint,joint);
	PHBallJointDesc d; j->GetDesc(&d);
	Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
	Quaterniond Qj = (j->solid[0]->GetPose() * d.poseSocket).Ori();

	Quaterniond pos = Qj.Inv() * dQ * Qj * joint->GetPosition();
	// Quaterniond pos = dQ * joint->GetPosition();
	// pos = pos / pos.norm();
	// std::cout << joint->GetPosition() << " -> " << pos << std::endl;

	/*
	Quaterniond q1 = Quaterniond(cos(dTheta[0]/2), e1[0]*sin(dTheta[0]/2), e1[1]*sin(dTheta[0]/2), e1[2]*sin(dTheta[0]/2));
	Quaterniond q2 = Quaterniond(cos(dTheta[1]/2), e2[0]*sin(dTheta[1]/2), e2[1]*sin(dTheta[1]/2), e2[2]*sin(dTheta[1]/2));
	Quaterniond pos = q2 * q1 * joint->GetPosition();
	*/

	joint->SetGoal(pos);

	PHSceneIf* phscene = this->GetScene()->Cast();
	phscene->GetSolids()[5*number+0]->SetFramePosition(dT*0*2+Pj);
	phscene->GetSolids()[5*number+1]->SetFramePosition(dT*1*2+Pj);
	phscene->GetSolids()[5*number+2]->SetFramePosition(dT*2*2+Pj);
	phscene->GetSolids()[5*number+3]->SetFramePosition(dT*3*2+Pj);
	phscene->GetSolids()[5*number+4]->SetFramePosition(dT*4*2+Pj);

	/*
	dT = axis*0.1;
	*/

	/*
	Vec3d dT1 = e1 * 0.1;
	Vec3d dT2 = e2 * 0.1;
	*/
	/*
	phscene->GetSolids()[6*number+0]->SetFramePosition(dT1*0*2+Pj);
	phscene->GetSolids()[6*number+1]->SetFramePosition(dT1*2*2+Pj);
	phscene->GetSolids()[6*number+2]->SetFramePosition(dT1*4*2+Pj);
	phscene->GetSolids()[6*number+3]->SetFramePosition(dT2*0*2+Pj);
	phscene->GetSolids()[6*number+4]->SetFramePosition(dT2*2*2+Pj);
	phscene->GetSolids()[6*number+5]->SetFramePosition(dT2*4*2+Pj);
	*/

	return;
	
	/*
	std::cout << " -- " << std::endl;
	std::cout << "w : " << dTheta << std::endl;
	std::cout << " -- " << std::endl;
	*/

	/*
	Quaterniond dQ = Quaterniond();
	dQ = Quaterniond::Rot(dTheta[0], 'x') * dQ;
	dQ = Quaterniond::Rot(dTheta[1], 'y') * dQ;
	dQ = Quaterniond::Rot(dTheta[1], 'z') * dQ;
	Quaterniond pos = dQ * joint->GetPosition();
	joint->SetGoal(pos);
	*/

	/*
	Vec3d ex = Vec3d(1.00, 0.12, 0.21); ex = ex / ex.norm();
	Vec3d ey = Vec3d(0.30, 1.00, 0.11); ey = ey / ey.norm();
	Vec3d ez = Vec3d(0.25, 0.10, 1.00); ez = ez / ez.norm();

	Quaterniond qx = Quaterniond(cos(dTheta[0]/2), ex[0]*sin(dTheta[0]/2), ex[1]*sin(dTheta[0]/2), ex[2]*sin(dTheta[0]/2));
	Quaterniond qy = Quaterniond(cos(dTheta[1]/2), ey[0]*sin(dTheta[1]/2), ey[1]*sin(dTheta[1]/2), ey[2]*sin(dTheta[1]/2));
	Quaterniond qz = Quaterniond(cos(dTheta[2]/2), ez[0]*sin(dTheta[2]/2), ez[1]*sin(dTheta[2]/2), ez[2]*sin(dTheta[2]/2));
	Quaterniond pos = qx * qz * qy * joint->GetPosition();

	joint->SetGoal(pos);

	return;
	*/

	/*
	/////////////////////////////////////////////////////////
	Vec3d dT = Vec3d(dTheta[0],dTheta[1],dTheta[2]);
	
	double angle = dT.norm();
	Vec3d  axis;
	if (angle!=0) axis  = dT / angle;
	else axis = Vec3d(0,0,0);

	while (angle >  3.1415926) { angle -= 3.1415926; }
	while (angle < -3.1415926) { angle += 3.1415926; }
	dT = angle * axis;

	PHBallJoint* j = DCAST(PHBallJoint,joint);
	PHBallJointDesc d; j->GetDesc(&d);
	Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
	PHSceneIf* phscene = this->GetScene()->Cast();
	phscene->GetSolids()[5*number+0]->SetFramePosition(dT*0*2+Pj);
	phscene->GetSolids()[5*number+1]->SetFramePosition(dT*1*2+Pj);
	phscene->GetSolids()[5*number+2]->SetFramePosition(dT*2*2+Pj);
	phscene->GetSolids()[5*number+3]->SetFramePosition(dT*3*2+Pj);
	phscene->GetSolids()[5*number+4]->SetFramePosition(dT*4*2+Pj);

	//
	Quaterniond dQ  = Quaterniond(cos(angle/2), axis[0]*sin(angle/2), axis[1]*sin(angle/2), axis[2]*sin(angle/2));
	Quaterniond pos = dQ * joint->GetPosition();
	//
	Quaterniond dQ  = joint->GetPosition().Derivative(dT);
	Quaterniond pos = dQ + joint->GetPosition();
	//
	if (pos.norm() != 0) {
		pos = pos / pos.norm();
	}
	joint->SetGoal(pos);
	*/
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
	double angle  = joint->GetPosition() + (dTheta[0]);
	double torque = tau[0];
	angle += torque * Rad(16) / joint->GetSpring();
	joint->SetSpringOrigin(angle);
}

}
