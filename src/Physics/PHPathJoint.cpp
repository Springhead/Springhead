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
// PHPath
IF_OBJECT_IMP_ABST(PHPath, SceneObject)

PHPath::PHPath(const PHPathDesc& desc){
	resize(desc.points.size());
	for(unsigned int i = 0; i < desc.points.size(); i++){
		(*this)[i].s = desc.points[i].s;
		(*this)[i].pose = desc.points[i].pose;
	}
	bLoop = desc.bLoop;
	bReady = false;
}

void PHPath::Rollover(double& s){
	double lower = front().s;
	double upper = back().s;
	assert(lower < upper);
	double range = upper - lower;
	while(s >= upper)s -= range;
	while(s <  lower)s += range;
}

PHPath::iterator PHPath::Find(double s){
	if(size() <= 1)return begin();
	if(bLoop)
		Rollover(s);
	iterator it;
	for(it = begin(); it != end(); it++){
		if(it->s > s)
			return it;
	}
	return it;
}

void PHPath::AddPoint(double s, const Posed& pose){
	PHPathPointWithJacobian p;
	p.s = s;
	p.pose = pose;
	if(empty() || s < front().s)
		insert(begin(), p);
	else if(s >= back().s)
		insert(end(), p);
	else
		insert(Find(s), p);
	bReady = false;
}

//6x6行列Jの一番下の行ベクトルは与えられているとして，
//gram-schmidtの直交化で残る5本の行ベクトルを設定する
void Orthogonalize(Matrix6d& J){
	int i, j, k;
	for(i = 4; i >= 0; i--){
		for(k = 0; k < 6; k++){
			J.row(i).clear();
			J.row(i)[k] = 1.0;
			for(j = i + 1; j < 6; j++){
				J.row(i) -= (J.row(i) * J.row(j)) * J.row(j);
			}
			double n = J.row(i).norm();
			if(n < 1.0e-10)
				continue;
			J.row(i) *= (1.0 / n);
			break;
		}
	}
}

void PHPath::CompJacobian(){
	double delta = (back().s - front().s) / 1000.0;		//数値微分の離散化幅．いいかげん．
	double div = 1.0 / (2.0 * delta);
	Posed p, p0, p1, pd;
	Vec3d v, w;
	Quaterniond qd;
	for(iterator it = begin(); it != end(); it++){
		//一般化座標qについて偏微分して相対速度を出す
		GetPose(it->s - delta, p0);
		GetPose(it->s + delta, p1);
		pd = (p1 - p0) * div;
		//一般化速度qdに1を与えたときの相対速度と角速度
		v = pd.Pos();
		qd = pd.Ori();
		w = (it->pose.Ori()).AngularVelocity(qd);		//1/2 * w * q = qd		=> 2 * qd * q~ = w
		it->J.row(5).SUBVEC(0, 3) =  v;
		it->J.row(5).SUBVEC(3, 3) =  w;
		double ninv = 1.0 / it->J.row(5).norm();
		it->J.row(5) *= ninv;
		Orthogonalize(it->J);
		it->J.row(5) *= ninv;
	}
	bReady = true;
}

void PHPath::GetPose(double s, Posed& pose){
	iterator it = Find(s);
	if(it == begin()){
		assert(!bLoop);
		pose = front().pose;
		return;
	}
	if(it == end()){
		assert(!bLoop);
		pose = back().pose;
		return;
	}
	PHPathPointWithJacobian &rhs = *it, &lhs = *--it;
	double tmp = 1.0 / (rhs.s - lhs.s);
	pose.Pos() = ((rhs.s - s) * tmp) * lhs.pose.Pos() + ((s - lhs.s) * tmp) * rhs.pose.Pos();
	Quaterniond qrel = lhs.pose.Ori().Conjugated() * rhs.pose.Ori();
	Vec3d axis = qrel.Axis();
	double angle = qrel.Theta();
	if(angle >  M_PI) angle -= 2 * M_PI;
	if(angle < -M_PI) angle += 2 * M_PI;
	angle *= (s - lhs.s) * tmp;
	pose.Ori() = lhs.pose.Ori() * Quaterniond::Rot(angle, axis);
}

void PHPath::GetJacobian(double s, Matrix6d& J){
	if(!bReady)CompJacobian();
	iterator it = Find(s);
	if(it == begin()){
		assert(!bLoop);
		J = front().J;
		return;
	}
	if(it == end()){
		assert(!bLoop);
		J = back().J;
		return;
	}
	PHPathPointWithJacobian &rhs = *it, &lhs = *--it;
	double tmp = 1.0 / (rhs.s - lhs.s);
	if(lhs.s <= s && s <= rhs.s);
	else{
		int hoge = 0;
	}
	J = ((rhs.s - s) * tmp) * lhs.J + ((s - lhs.s) * tmp) * rhs.J;
}

//----------------------------------------------------------------------------
// PHPathJoint

IF_OBJECT_IMP(PHPathJoint, PHJoint1D)

PHPathJoint::PHPathJoint(){
	q = qd = 0.0;
}

bool PHPathJoint::AddChildObject(ObjectIf* o){
	PHPath* p = DCAST(PHPath, o);
	if(p){
		path = p;
		//可動範囲はリセットされる
		// 周期パスならば可動範囲無し
		// 非周期パスならば初端と終端を可動範囲とする
		if(path->IsLoop()){
			lower = upper = 0.0;
		}
		else{
			lower = path->front().s;
			upper = path->back().s;
		}
		return true;
	}
	return PHConstraint::AddChildObject(o);
}

/*void PHPathJoint::CompConstraintJacobian(){
	if(!path)return;
	CompDof();
	dim_c = 6;
	Matrix6d J;
	path->GetJacobian(q, J);
	Ad.clear();
	Ac.clear();
	for(int i = 0; i < 2; i++){
		Jdv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jdv[i].SUBMAT(3, 0, 3, 3) = Jwv[i];
		Jdw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		Jdw[i].SUBMAT(3, 0, 3, 3) = Jww[i];
		Jdv[i] = J * Jdv[i];
		Jdw[i] = J * Jdw[i];
		Jcv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jcv[i].SUBMAT(3, 0, 3, 3) = Jqv[i];
		Jcw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		Jcw[i].SUBMAT(3, 0, 3, 3) = Jqw[i];
		if(solid[i]->solid->IsDynamical()){
			Tdv[i] = Jdv[i] * solid[i]->minv;
			Tdw[i] = Jdw[i] * solid[i]->Iinv;
			Tcv[i] = Jcv[i] * solid[i]->minv;
			Tcw[i] = Jcw[i] * solid[i]->Iinv;
			for(int j = 0; j < 6; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
			for(int j = 0; j < 6; j++)
				Ac[j] += Jcv[i].row(j) * Tcv[i].row(j) + Jcw[i].row(j) * Tcw[i].row(j);
		}
	}
}*/

void PHPathJoint::CompBias(double dt, double correction_rate){
	if(mode == MODE_VELOCITY){
		b.w.z -= vel_d;
	}

}

/*void PHPathJoint::CompError(double dt){
	if(!path)return;
	
	//velocity update後の関節速度の値
	Vec3d v[2], w[2];
	for(int i = 0; i < 2; i++){
		v[i] = solid[i]->v + solid[i]->dv0 + solid[i]->dv;
		w[i] = solid[i]->w + solid[i]->dw0 + solid[i]->dw;
	}
	//Matrix6d J;
	//path->GetJacobian(q, J);
	//Vec6d Vrel;
	//Vrel.SUBVEC(0, 3) = Jvv[0] * v[0] + Jvw[0] * w[0] + Jvv[1] * v[1] + Jvw[1] * w[1];
	//Vrel.SUBVEC(3, 3) = Jwv[0] * v[0] + Jww[0] * w[0] + Jwv[1] * v[1] + Jww[1] * w[1];
	//double qd = Vrel.norm() / J.row(5).norm();
	//if(Vrel[0] * J.row(5)[0] < 0.0)
	//	qd = -qd;
	double qd = Ad[5] * (Jdv[0].row(5) * v[0] + Jdw[0].row(5) * w[0] +
				Jdv[1].row(5) * v[1] + Jdw[1].row(5) * w[1]);
	qd = 1.0;
	q += qd * dt;
	//q = 0.0;
	path->Rollover(q);
	Posed pnew;
	path->GetPose(q, pnew);
	B.SUBVEC(0, 3) = rjrel - pnew.Pos();
	DSTR << rjrel << pnew.Pos() << qjrel << pnew.Ori() << endl;
	if(qjrel.V() * pnew.Ori().V() < 0.0){
		 B.SUBVEC(3, 3) = qjrel.V() + pnew.Ori().V();
		// DSTR << "p";
	}
	else{
		B.SUBVEC(3, 3) = qjrel.V() - pnew.Ori().V();
		//DSTR << "n";
	}
	//DSTR << B << endl;
	//B.SUBVEC(3, 3) = qjrel.V() - pnew.Ori().V();
	//B = -B;
}*/

void PHPathJoint::Projection(double& f, int k){
	if(k == 5){
		if(on_lower){
			f = max(0.0, f);
		}
		if(on_upper)
			f = min(0.0, f);
	}
}

/*void PHPathJoint::ProjectionCorrection(double& F, int k){
	if(k == 5){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}*/

}
