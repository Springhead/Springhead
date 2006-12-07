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

using namespace std;
using namespace PTM;
namespace Spr{;

//-----------------------------------------------------------------------------
IF_OBJECT_IMP_ABST(PHTreeNode, SceneObject);

PHTreeNode::PHTreeNode():joint(0){
}

bool PHTreeNode::Includes(PHTreeNode* node){
	if(node == this)return true;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		if((*it)->Includes(node))
			return true;
	return false;
}

PHTreeNode*	PHTreeNode::FindBySolid(PHSolid* solid){
	if(GetSolid() == solid)return this;
	PHTreeNode* node;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++){
		node = (*it)->FindBySolid(solid);
		if(node)return node;
	}
	return NULL;
}

/*int PHTreeNode::GetTotalDof(){
	int dof = GetDof();
	for(int i = 0; i < Children().size(); i++)
		dof += Children()[i]->GetTotalDof();
	return dof;
}*/

/*int PHTreeNode::AssignID(int _id, std::vector<PHTreeNode*>& table){
	id = _id++;
	table[id] = this;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		_id = AssignID(_id, table);
	return _id;
}*/

void PHTreeNode::CompSpatialTransform(){
	Xcp = joint->Xj[1].inv() * joint->Xjrel * joint->Xj[0];
	Xcj = joint->Xj[1].inv() * SpatialTransform(joint->Xjrel.R, Vec3d());
}

void PHTreeNode::CompCoriolisAccel(){
	PHSolid*	sp = GetParent()->GetSolid();
	PHSolid*	s = GetSolid();
	const SpatialVector		&vp = sp->v, &v = s->v, &vjrel = joint->vjrel;
	const SpatialTransform	&Xj0 = joint->Xj[0], &Xj1 = joint->Xj[1], &Xjrel = joint->Xjrel;
	SpatialVector	vj = joint->Xj[0] * vp;

	SpatialVector cpj, cjj, ccj;
	cpj.v = (vp.w * Xj0.r) * vp.w - vp.w.square() * Xj0.r;
	cpj.w.clear();

	cjj.v = (vj.w * Xjrel.r) * vj.w - vj.w.square() * Xjrel.r + 2 * vj.w % vjrel.v;
	cjj.w = vj.w % vjrel.w;

	ccj.v = (v.w * Xj1.r) * v.w - v.w.square() * Xj1.r;
	ccj.w.clear();

	CompJointCoriolisAccel();
	c = Xj1.inv() * (
		  Xjrel * SpatialTransform(Xj0.R, Vec3d()) * cpj
		+ SpatialTransform(Xjrel.R, Vec3d()) * cjj
		- SpatialTransform(Xj1.R, Vec3d()) * ccj)
		+ Xcj * cj;
	c *= scene->GetTimeStep();
}

void PHTreeNode::CompIsolatedInertia(){
	double m = GetSolid()->GetMass();
	I.vv = Matrix3d::Diag(m, m, m);
	I.vw.clear();
	I.wv.clear();
	I.ww = GetSolid()->GetInertia();
}

void PHTreeNode::CompArticulatedInertia(){
	CompIsolatedInertia();
	CompSpatialTransform();
	CompJointJacobian();

	CompCoriolisAccel();
	
	//子ノードにIaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のIaを積み上げる
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompArticulatedInertia();

	Ic = I * c;

	//親ノードにIaを積む
	AccumulateInertia();
}

void PHTreeNode::CompIsolatedBiasForce(){
	// PHSolidInfoのdvが似たような式なので共有化したほうがいい
	PHSolid* s = GetSolid();
	if(s->IsDynamical()){
		Z.v = -1.0 * s->f.v;
		Z.w = -1.0 * s->f.w + s->v.w % (s->GetInertia() * s->v.w);
		Z *= scene->GetTimeStep();
	}
	else Z.clear();
}

void PHTreeNode::CompArticulatedBiasForce(){
	CompIsolatedBiasForce();

	//子ノードにZaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のZaを積み上げる
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompArticulatedBiasForce();

	ZplusIc = Z + Ic;

	//親ノードにIaを積む
	AccumulateBiasForce();
	
}

void PHTreeNode::CompResponse(const SpatialVector& dF, bool bUpdate){
	dZ = -1.0 * dF;
	CompBiasForceDiff(bUpdate);
}

void PHTreeNode::SetupLCP(){
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->SetupLCP();
}
void PHTreeNode::IterateLCP(){
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->IterateLCP();
}

void PHTreeNode::UpdateVelocity(double dt){
	PHSolid* sp = GetParent()->GetSolid();
	PHSolid* s  = GetSolid();
	UpdateJointVelocity(dt);
	CompRelativeVelocity();
	s->v = Xcp * sp->v + Xcj * joint->vjrel;
	//s->oldVel	 = s->GetVelocity();
	//s->oldAngVel = s->GetAngularVelocity();
	s->SetVelocity       (s->GetOrientation() * s->v.v);
	s->SetAngularVelocity(s->GetOrientation() * s->v.w);
	
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdateVelocity(dt);
}
void PHTreeNode::UpdatePosition(double dt){
	PHSolid* sp = GetParent()->GetSolid();
	PHSolid* s  = GetSolid();
	UpdateJointPosition(dt);
	CompRelativePosition();
	CompSpatialTransform();
	SpatialTransform Xp(sp->GetRotation(), sp->GetCenterPosition());
	SpatialTransform Xc = Xcp * Xp;
	s->SetCenterPosition(Xc.r);
	s->SetRotation(Xc.R);
	s->SetUpdated(true);
	
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdatePosition(dt);
}

//-----------------------------------------------------------------------------
IF_OBJECT_IMP(PHRootNode, PHTreeNode);

void PHRootNode::SetupABA(){
	CompArticulatedInertia();
	CompArticulatedBiasForce();
	CompAccel();
}

void PHRootNode::CompArticulatedInertia(){
	CompIsolatedInertia();

	//子ノードにIaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のIaを積み上げる
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompArticulatedInertia();

	//逆行列を計算
	Iinv = I.inv();
}

void PHRootNode::CompArticulatedBiasForce(){
	CompIsolatedBiasForce();

	//子ノードにZaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のZaを積み上げる
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompArticulatedBiasForce();
}

void PHRootNode::CompAccel(){
	if(GetSolid()->IsDynamical()){
		a = -1 * (Iinv * Z);
	}else{
		a.clear();
	}
	solid->dv = a;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccel();
}

void PHRootNode::CompBiasForceDiff(bool bUpdate){
	if(bUpdate)
		Z += dZ;
	if(GetSolid()->IsDynamical()){
		da = -1 * (Iinv * dZ);
		if(bUpdate)
			solid->dv += da;
	}
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccelDiff(bUpdate);
}
void PHRootNode::UpdateVelocity(double dt){
	solid->UpdateVelocity(dt);
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdateVelocity(dt);
}
void PHRootNode::UpdatePosition(double dt){
	solid->UpdatePosition(dt);
	solid->SetUpdated(true);
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdatePosition(dt);
}

//-----------------------------------------------------------------------------
// explicit instantiation
template class PHTreeNodeND<1>;
template class PHTreeNodeND<2>;
template class PHTreeNodeND<3>;

template<int NDOF>
PHTreeNodeND<NDOF>::PHTreeNodeND(){
	f.clear();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::AccumulateInertia(){
	for(int i = 0; i < NDOF; i++)
		IJ[i] = I * J[i];
	for(int i = 0; i < NDOF; i++)for(int j = 0; j < NDOF; j++)
        JIJ[i][j] = J[i] * IJ[j];
	JIJinv = JIJ.inv();

	// J_JIJinv
	for(int i = 0; i < NDOF; i++){
		J_JIJinv[i].clear();
		for(int j = 0; j < NDOF; j++){
			J_JIJinv[i] += J[j] * JIJinv[j][i];
		}
	}
	// IJ_JIJinv
	for(int i = 0; i < NDOF; i++){
		IJ_JIJinv[i].clear();
		for(int j = 0; j < NDOF; j++){
			IJ_JIJinv[i] += IJ[j] * JIJinv[j][i];
		}
	}
	// IJ_JIJinv_Jtr
	IJ_JIJinv_Jtr.clear();
	for(int i = 0; i < NDOF; i++)
		IJ_JIJinv_Jtr += VVtr(IJ_JIJinv[i], J[i]);
	// IJ_JIJinv_JtrI
	IJ_JIJinv_JtrI.clear();
	for(int i = 0; i < NDOF; i++)
		IJ_JIJinv_JtrI += VVtr(IJ_JIJinv[i], IJ[i]);

	GetParent()->I += Xtr_Mat_X(Xcp, I - IJ_JIJinv_JtrI);
}

template<int NDOF>
void PHTreeNodeND<NDOF>::AccumulateBiasForce(){
	for(int i = 0; i < NDOF; i++)
		J_ZplusIc[i] = J[i] * ZplusIc;

	TVector<NDOF, double> tmp = GetJoint()->torque * scene->GetTimeStep() - J_ZplusIc;
	SpatialVector tmp2;
	for(int i = 0; i < NDOF; i++)
		tmp2 += IJ_JIJinv[i] * tmp[i];
	GetParent()->Z += Xcp.trans() * (ZplusIc + tmp2);
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompJointJacobian(){
	// 派生クラスが計算したヤコビアンに座標変換をかける
	for(int i = 0; i < NDOF; i++)
		J[i] = Xcj * J[i];
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompAccel(){
	ap = Xcp * GetParent()->a;
	//加速度を計算
	TVector<NDOF, double> IJ_ap;
	for(int i = 0; i < NDOF; i++)
		IJ_ap[i] = IJ[i] * ap;
	accel = JIJinv * (GetJoint()->torque * scene->GetTimeStep() - IJ_ap - J_ZplusIc);

	//重心周りの加速度(子ノードの積分で使用する)
	SpatialVector J_accel;
	for(int i = 0; i < NDOF; i++)
		J_accel += J[i] * accel[i];
	a = ap + c + J_accel;
	GetSolid()->dv = a;

	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccel();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompBiasForceDiff(bool bUpdate){
	if(bUpdate)
		Z += dZ;
	GetParent()->dZ = Xcp.trans() * (dZ - IJ_JIJinv_Jtr * dZ);
	GetParent()->CompBiasForceDiff(bUpdate);
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompAccelDiff(bool bUpdate){
	SpatialVector dap = Xcp * GetParent()->da;
	daccel = JIJinv * dtau;
	for(int i = 0; i < NDOF; i++)
		daccel[i] -= IJ_JIJinv[i] * dap + J_JIJinv[i] * dZ;
	dZ.clear(); // 次回の呼び出しのためにここでクリア
	dtau.clear();
    da = dap;
	for(int i = 0; i < NDOF; i++)
		da += J[i] * daccel[i];
	if(bUpdate){
		accel += daccel;
		a += da;
		GetSolid()->dv = a;
	}
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccelDiff(bUpdate);
}
template<int NDOF>
void PHTreeNodeND<NDOF>::UpdateJointVelocity(double dt){
	GetJoint()->velocity += accel;
}
template<int NDOF>
void PHTreeNodeND<NDOF>::UpdateJointPosition(double dt){
	GetJoint()->position += GetJoint()->velocity * dt;
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompResponse(const TVector<NDOF, double>& _dtau, bool bUpdate){
	dtau = _dtau;
	SpatialVector tmp;
	for(int i = 0; i < NDOF; i++)
		tmp += IJ_JIJinv[i] * dtau[i];
	GetParent()->dZ = Xcp.trans() * tmp;
	GetParent()->CompBiasForceDiff(bUpdate);
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompResponseMatrix(){
	const double eps = 1.0e-6;
	TVector<NDOF, double> tau;
	for(int i = 0; i < NDOF; i++){
		tau[i] = 1.0;
		CompResponse(tau, false);
		A[i] = max(eps, daccel[i]);
		tau[i] = 0.0;
	}
}

template<int NDOF>
void PHTreeNodeND<NDOF>::SetupLCP(){
	PHJointND<NDOF>* j = GetJoint();
	bool con, constrAtAll = false;
	for(int i = 0; i < NDOF; i++){
		con = j->constr[j->axisIndex[i]];
		if(con && constr[i])			// 継続して拘束される場合
			 f[i] *= engine->shrinkRate;
		else f[i] = 0.0;					// 新規に拘束される or 拘束されない
		constr[i] = con;
		constrAtAll |= con;
	}
	if(constrAtAll){
		// LCPのbベクトル
		b = GetJoint()->velocity;
		CompBias();	// 目標速，バネダンパによる補正項を計算
		b += db;
		
		// LCPのA行列の対角成分を計算
		CompResponseMatrix();
		for(int i = 0; i < NDOF; i++)
			Ainv[i] = 1.0 / (A[i] + dA[i]);

		// 拘束力初期値による速度変化量を計算
		CompResponse(f);
	}
	PHTreeNode::SetupLCP();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::IterateLCP(){
	TVector<NDOF, double> fnew, dfs;
	double df;
	for(int i = 0; i < NDOF; i++){
		if(!constr[i])continue;
		fnew[i] = f[i] - Ainv[i] * (dA[i] * f[i] + b[i] + accel[i]);
		Projection(fnew[i], i);
		df = fnew[i] - f[i];
		dfs.clear();
		dfs[i] = df;
		CompResponse(dfs);
		f[i] = fnew[i];
	}
	PHTreeNode::IterateLCP();
}

//-----------------------------------------------------------------------------
//IF_OBJECT_IMP(PHTreeNode1D, PHTreeNode);

/*PHTreeNode1D::PHTreeNode1D(){
	f = 0.0;
}

void PHTreeNode1D::AccumulateInertia(){
	IJ             = I * J;
	JIJ            = J * IJ;
	JIJinv         = 1.0 / JIJ;
	J_JIJinv       = J * JIJinv;
	IJ_JIJinv      = IJ * JIJinv;
	IJ_JIJinv_Jtr  = VVtr(IJ_JIJinv, J);
	IJ_JIJinv_JtrI = VVtr(IJ_JIJinv, IJ);
	GetParent()->I += Xtr_Mat_X(Xcp, I - IJ_JIJinv_JtrI);
}

void PHTreeNode1D::AccumulateBiasForce(){
  	J_ZplusIc = J * ZplusIc;
	GetParent()->Z +=
		Xcp.trans() * (ZplusIc + IJ_JIJinv * (GetJoint()->torque * scene->GetTimeStep() - J_ZplusIc));
}

void PHTreeNode1D::CompJointJacobian(){
	// 各派生クラスが設定したヤコビアンに座標変換をかける
	J = Xcj * J;
}

void PHTreeNode1D::CompAccel(){
	ap = Xcp * GetParent()->a;
	//加速度を計算
	accel = JIJinv * (GetJoint()->torque * scene->GetTimeStep() - IJ * ap - J_ZplusIc);
	
	//重心周りの加速度(子ノードの積分で使用する)
	a = ap + c + J * accel;
	GetSolid()->dv = a;
	
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccel();
}

void PHTreeNode1D::CompBiasForceDiff(bool bUpdate){
	if(bUpdate)
		Z += dZ;
	GetParent()->dZ = Xcp.trans() * (dZ - IJ_JIJinv_Jtr * dZ);
	GetParent()->CompBiasForceDiff(bUpdate);
}

void PHTreeNode1D::CompAccelDiff(bool bUpdate){
	SpatialVector dap = Xcp * GetParent()->da;
	daccel = JIJinv * dtau - IJ_JIJinv * dap - J_JIJinv * dZ;
	dZ.clear(); // 次回の呼び出しのためにここでクリア
	dtau = 0.0;
	da = dap + J * daccel;
	if(bUpdate){
		accel += daccel;
		a += da;
		GetSolid()->dv = a;
	}
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccelDiff(bUpdate);
}
void PHTreeNode1D::UpdateJointVelocity(double dt){
	GetJoint()->velocity += accel;
}
void PHTreeNode1D::UpdateJointPosition(double dt){
	GetJoint()->position += GetJoint()->velocity * dt;
}

void PHTreeNode1D::CompResponse(double _dtau, bool bUpdate){
	dtau = _dtau;
	GetParent()->dZ = Xcp.trans() * (IJ_JIJinv * dtau);
	GetParent()->CompBiasForceDiff(bUpdate);
}

void PHTreeNode1D::CompResponseMatrix(){
	const double eps = 1.0e-3;
	CompResponse(1.0, false);
	A = max(eps, daccel);
}

void PHTreeNode1D::SetupLCP(){
	PHJoint1D* j = GetJoint();
	bool con = j->constr[j->axisIndex[0]];
	if(con && constr)				// 継続して拘束される場合
		 f *= engine->shrinkRate;
	else f = 0.0;					// 新規に拘束される or 拘束されない
	constr = con;

	if(constr){
		// LCPのbベクトル
		b = GetJoint()->velocity;
		CompBias();	// 目標速，バネダンパによる補正項を計算
		b += db;
		// LCPのA行列の対角成分を計算
		CompResponseMatrix();
		Ainv = 1.0 / (A + dA);
		// 拘束力初期値による速度変化量を計算
		CompResponse(f);
	}
	PHTreeNode::SetupLCP();
}

void PHTreeNode1D::IterateLCP(){
	if(constr){
		double fnew, df;
		fnew = f - Ainv * (dA * f + b + accel);
		Projection(fnew);
		df = fnew - f;
		CompResponse(df);
		f = fnew;
	}
	PHTreeNode::IterateLCP();
}*/

void PHTreeNode1D::Projection(double& _f, int k){
	PHJoint1D* j = GetJoint();
	if(j->onLower)
		_f = max(0.0, _f);
	if(j->onUpper)
		_f = min(0.0, _f);
}

}
