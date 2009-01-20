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
	if (bEnabled) {
		std::cout << " -- " << std::endl;
		if (nodes.size() > 0 && controlpoints.size() > 0) {
			for(size_t i=0; i<nodes.size(); ++i){
				nodes[i]->PrepareSolve();
			}

			for(size_t n=0; n<numIter; n++){
				for(size_t i=0; i<nodes.size(); ++i){
					nodes[i]->ProceedSolve();
					PTM::VVector<double> diff = (DCAST(PHIKNode,nodes[i])->dTheta_prev - DCAST(PHIKNode,nodes[i])->dTheta);
					// std::cout << n << "," << i << " : " << diff.norm() << std::endl;
					/*
					std::cout << " -- -- -- " << std::endl;
					std::cout << "dT   : " << DCAST(PHIKNode,nodes[i])->dTheta << std::endl;
					std::cout << "dTp  : " << DCAST(PHIKNode,nodes[i])->dTheta_prev << std::endl;
					std::cout << "diff : " << diff << std::endl;
					std::cout << " -- -- -- " << std::endl;
					*/
				}
			}

			for(size_t i=0; i<nodes.size(); ++i){
				nodes[i]->Move();
			}
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
	if(oc){
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
	Quaterniond qS = solid->GetPose().Ori();
	Quaterniond qG = (goal * qS.Inv());

	double epsilon = Rad(10.0);
	if (qG.Theta() < epsilon) {
		return((qG.Axis() * qG.Theta()));
	} else {
		return((qG.Axis() * epsilon));
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKNode

// --- --- --- --- ---
void PHIKNode::SetNDOF(int n){
	ndof = n;
	iDx.resize(ndof); iDx.clear();
	iD.resize(ndof); iD.clear();
	F.resize(ndof, ndof); F.clear();
	for(size_t i=0; i<K.size(); ++i){ K[i].resize(ndof, ndof); K[i].clear(); }
	dTheta.resize(ndof); dTheta.clear();
	dTheta_prev.resize(ndof); dTheta_prev.clear();
	tau.resize(ndof); tau.clear();
}

void PHIKNode::ClearJacobian(){
	Mj.clear();
}

void PHIKNode::CalcAllJacobian(){
	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		int n = DCAST(PHIKControlPoint,*ctlpt)->number;
		Mj[n] = CalcJacobian(*ctlpt);
		// std::cout << number << "," << n << " : " << Mj[n] << std::endl;
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
		int n = DCAST(PHIKControlPoint,*ctlpt)->number;
		J = Mj[n] * sqrt(bias);
		JtJ += J.trans() * J;
		Jtx += J.trans() * DCAST(PHIKControlPoint,(*ctlpt))->GetTmpGoal();

		tau += J.trans() * DCAST(PHIKControlPoint,(*ctlpt))->GetForce();
	}

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
			int n = DCAST(PHIKControlPoint,*ctlpt)->number;
			K[node_i] += (Mj[n].trans() * Mj[n]);
			// std::cout << "M^T M : " << (Mj[n].trans() * Mj[n]) << std::endl;
		}
		++node_i;
	}

	/*
	std::cout << "iD  : " << iD  << std::endl;
	std::cout << "iDx : " << iDx << std::endl;
	std::cout << "F   : " << F   << std::endl;
	*/

	dTheta.clear();
	dTheta_prev.clear();
}

void PHIKNode::ProceedSolve(){
	dTheta_prev = dTheta;

	PTM::VVector<double> Kv; Kv.resize(ndof); Kv.clear();
	int node_i=0;
	for(NSetIter node=linkedNodes.begin(); node!=linkedNodes.end(); ++node){
		// std::cout << "K[" << node_i << "] : " << K[node_i] << std::endl;
		Kv += (K[node_i] * DCAST(PHIKNode,(*node))->dTheta);
		++node_i;
	}
	PTM::VVector<double> B; B.resize(ndof); B.clear();

	B = F*dTheta + Kv;
	for(int i=0; i<ndof; i++){ B[i] *= iD[i]; }
	dTheta = iDx - B;

	/*
	std::cout << "B   : " << B   << std::endl;
	std::cout << "Kv  : " << Kv  << std::endl;
	*/
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
void PHIKBallJoint::CalcAllJacobian(){
	CalcAxis();
	PHIKNode::CalcAllJacobian();
}

void PHIKBallJoint::CalcAxis(){
	e[0] = Vec3d(1,0,0);
	e[1] = Vec3d(0,1,0);
	e[2] = Vec3d(0,0,1);

	for(CSetIter ctlpt=linkedControlPoints.begin(); ctlpt!=linkedControlPoints.end(); ++ctlpt){
		PHIKPosCtlIf* cpPos;
		if (cpPos = DCAST(PHIKPosCtlIf,(*ctlpt))){
			// 関節の回転中心
			PHBallJoint* j = DCAST(PHBallJoint,joint);
			PHBallJointDesc d; j->GetDesc(&d);
			Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);

			// エンドエフェクタ位置
			PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
			Vec3d Pc = cp->solid->GetPose() * cp->pos;

			// 関節回転中心<->エンドエフェクタ 軸
			Vec3d e0 = (Pc - Pj);
			if (e0.norm() != 0){
				e0 = e0 / e0.norm();
			} else {
				e0 = Vec3d(1,0,0);
			}

			// 回転軸を求める
			if (abs(PTM::dot(e0,Vec3d(1,0,0))) > Rad(10)) {
				e[0] = Vec3d(0,1,0);
			} else {
				e[0] = Vec3d(1,0,0);
			}
			e[0] = e[0] - (PTM::dot(e[0],e0) * e0);
			e[0] = e[0] / e[0].norm();
			e[1] = PTM::cross(e0,e[0]);
			e[2] = e0;

			return;
		}
	}
}

PTM::VMatrixRow<double> PHIKBallJoint::CalcJacobian(PHIKControlPointIf* control){
	PHIKPosCtlIf* cpPos;
	if (cpPos = DCAST(PHIKPosCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,ndof);

		// 関節の回転中心
		PHBallJoint* j = DCAST(PHBallJoint,joint);
		PHBallJointDesc d; j->GetDesc(&d);
		Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);

		// エンドエフェクタ位置
		PHIKPosCtl* cp = DCAST(PHIKPosCtl,cpPos);
		Vec3d Pc = cp->solid->GetPose() * cp->pos;

		// ヤコビアンを求める
		for (int i=0; i<ndof; ++i) {
			Vec3d v = PTM::cross(e[i],(Pc-Pj));
			M[0][i]=v[0]; M[1][i]=v[1]; M[2][i]=v[2];
		}

		return M;
	}

	PHIKOriCtlIf* cpOri;
	if (cpOri = DCAST(PHIKOriCtlIf,control)){
		PTM::VMatrixRow<double> M; M.resize(3,3); M.clear();

		for (int i=0; i<3; ++i) {
			for (int j=0; j<ndof; ++j) {
				if (i==j) {
					M[i][j]=1;
				} else {
					M[i][j]=0;
				}
			}
		}

		return M;
	}

	return PTM::VMatrixRow<double>();
}

void PHIKBallJoint::Move(){
	if (linkedControlPoints.size() == 0) {
		return;
	}
	std::cout << dTheta << std::endl;

	/*
	for (int i=0; i<ndof; ++i) {
		if (dTheta[i] > Rad(50)) {
			dTheta[i] =  Rad(50);
		} else if (dTheta[i] < Rad(-50)) {
			dTheta[i] = Rad(-50);
		}
	}
	*/

	// 回転軸ベクトルにする
	Vec3d dT = Vec3d();
	for (int i=0; i<ndof; ++i) {
		dT += dTheta[i] * e[i];
	}

	// Axis-Angle表現にする
	double angle = dT.norm();
	Vec3d axis = dT;
	if (angle != 0) {
		axis = axis / angle;
	} else {
		axis = Vec3d(1,0,0);
	}

	// Quaternionにする
	Quaterniond dQ = Quaterniond::Rot(angle, axis);

	// 関節のローカル座標をもとめる
	PHBallJoint* j = DCAST(PHBallJoint,joint);
	PHBallJointDesc d; j->GetDesc(&d);
	Vec3d Pj = j->solid[0]->GetPose() * d.poseSocket * Vec3d(0,0,0);
	Quaterniond Qj = (j->solid[0]->GetPose() * d.poseSocket).Ori();

	// 関節のローカル座標系にする
	Quaterniond pos = Qj.Inv() * dQ * Qj * joint->GetPosition();

	// 関節を動かす
	joint->SetGoal(pos);

	// デバッグ表示（要改善）
	/*
	PHSceneIf* phscene = this->GetScene()->Cast();
	phscene->GetSolids()[5*number+0]->SetFramePosition(dT*0*2+Pj);
	phscene->GetSolids()[5*number+1]->SetFramePosition(dT*1*2+Pj);
	phscene->GetSolids()[5*number+2]->SetFramePosition(dT*2*2+Pj);
	phscene->GetSolids()[5*number+3]->SetFramePosition(dT*3*2+Pj);
	phscene->GetSolids()[5*number+4]->SetFramePosition(dT*4*2+Pj);
	*/

	return;
}

void PHIKBallJoint::AddControlPoint(PHIKControlPointIf* control){
	if (DCAST(PHIKOriCtlIf,control)) {
		// 姿勢制御に荷担するボールジョイントは自由度を３に引き上げる
		// （デフォルトではエンドエフェクタの位置を変えない回転軸を除いた
		// 　２軸（ユニバーサルジョイント）になっている）
		SetNDOF(3);
	}
	PHIKNode::AddControlPoint(control);
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
	if (linkedControlPoints.size() == 0) {
		return;
	}

	// std::cout << dTheta << std::endl;

	static const double Pi = 3.141592653589;

	// 新しい回転角度
	double angle  = joint->GetPosition() + (dTheta[0]);

	// トルクを実現するためのオフセットの追加
	double torque = tau[0];
	angle += torque * Rad(16) / joint->GetSpring();

	// 関節を動かす
	joint->SetSpringOrigin(angle);
}

}
