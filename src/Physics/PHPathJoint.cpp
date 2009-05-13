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

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHPath
PHPath::PHPath(const PHPathDesc& desc){
	SetDesc(&desc);
}

bool PHPath::GetDesc(void* desc)const{
	PHPathDesc* pathdesc = (PHPathDesc*)desc;
	pathdesc->bLoop = bLoop;
	pathdesc->points.resize(size());
	for(int i = 0; i < (int)size(); i++){
		pathdesc->points[i].s = (*this)[i].s;
		pathdesc->points[i].pose = (*this)[i].pose;
	}
	return true;
}

void PHPath::SetDesc(const void* desc){
	const PHPathDesc& pathdesc = *(const PHPathDesc*)desc;
	resize(pathdesc.points.size());
	for(unsigned int i = 0; i < pathdesc.points.size(); i++){
		(*this)[i].s = pathdesc.points[i].s;
		(*this)[i].pose = pathdesc.points[i].pose;
	}
	bLoop = pathdesc.bLoop;
	bReady = false;	
}

void PHPath::Rollover(double& s){
	double lower = front().s;
	double upper = back().s;
	assert(lower < upper);
	double range = upper - lower;
	// 苦肉の策
	if(abs(s) > 1.0e3)
		s = 0.0;
	while(s >= upper)s -= range;
	while(s <  lower)s += range;
}

PHPath::iterator PHPath::Find(double& s){
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
	double div = 1.0 / delta;
	Posed p, p0, p1, pd;
	Vec3d v, w;
	Quaterniond qd;
	for(iterator it = begin(); it != end(); it++){
		//一般化座標qについて偏微分して相対速度を出す
		if(it == begin())
			p0 = it->pose;
		else GetPose(it->s - delta, p0);
		iterator itnext = it; ++itnext;
		if(itnext == end())
			p1 = it->pose;
		else GetPose(it->s + delta, p1);
		// 0deg (q = [1 0 0 0])と360deg (q = [-1 0 0 0])は回転としては同じだが数値的に離れている
		if(abs(p0.Ori().w - p1.Ori().w) > 1.99)
			p0.Ori() = -1.0 * p0.Ori();
		// 差分をとる
		if(it == begin() || itnext == end())
			 pd = (p1 - p0) * div;
		else pd = (p1 - p0) * div * 0.5;
		//一般化速度qdに1を与えたときの相対速度と角速度
		v = pd.Pos();
		qd = pd.Ori();
		w = (it->pose.Ori()).AngularVelocity(qd);		//1/2 * w * q = qd		=> 2 * qd * q~ = w
		it->J.row(5).SUBVEC(0,3) =  v;
		it->J.row(5).SUBVEC(3,3) =  w;
		//double ninv = 1.0 / it->J.row(5).norm();
		//it->J.row(5).unitize();// *= ninv;
		Orthogonalize(it->J);
		//it->J.row(5) *= ninv;
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
	if(!bReady){
		CompJacobian();
	}
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
	J = ((rhs.s - s) * tmp) * lhs.J + ((s - lhs.s) * tmp) * rhs.J;
}

//----------------------------------------------------------------------------
// PHPathJoint

PHPathJoint::PHPathJoint(const PHPathJointDesc& desc){
	SetDesc(&desc);
	axisIndex[0] = 5;
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
		position[0] = velocity[0] = 0.0;
		return true;
	}
	return PHConstraint::AddChildObject(o);
}

void PHPathJoint::UpdateJointState(){
	Matrix6d J;
	path->GetJacobian(position[0], J);
	velocity[0] = vjrel.norm();
	if(vjrel * J.row(5) < 0.0)
		velocity[0] = -velocity[0];
	position[0] += velocity[0] * GetScene()->GetTimeStep();
	path->Rollover(position[0]);
}

void PHPathJoint::ModifyJacobian(){
	Matrix6d Jq;
	path->GetJacobian(position[0], Jq);
	(Matrix6d&)J[0] = Jq * J[0];
	(Matrix6d&)J[1] = Jq * J[1];
}

void PHPathJoint::CompBias(){
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	Posed p;
	path->GetPose(position[0], p);
	db.v() = ((Xjrel.r - p.Pos()) * dtinv/* + vjrel.v()*/);
	//db.w() = (Xjrel.q.AngularVelocity((Xjrel.q - p.Ori()) * dtinv) + vjrel.w());
	db.w().clear();
	Matrix6d Jq;
	path->GetJacobian(position[0], Jq);
	(Vec6d&)db = Jq * db;
	db.w().z = 0.0;
	db *= engine->velCorrectionRate;

	double diff;
	if(mode == PHJointDesc::MODE_VELOCITY){
		db.w().z = -desiredVelocity;
	}
	else if(spring != 0.0 || damper != 0.0){
		diff = GetPosition() - origin;
		//while(diff >  M_PI) diff -= 2 * M_PI;
		//while(diff < -M_PI) diff += 2 * M_PI;
		double tmp = 1.0 / (damper + spring * GetScene()->GetTimeStep());
		dA.w().z = tmp * dtinv;
		db.w().z = spring * (diff) * tmp;
	}
}

/*void PHPathJoint::CompError(double dt){
	if(!path)return;
	
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

/*void PHPathJoint::ProjectionCorrection(double& F, int k){
	if(k == 5){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}*/

//-----------------------------------------------------------------------------
void PHPathJointNode::CompJointJacobian(){
	PHPathJoint* j = GetJoint();
	Matrix6d Jq;
	j->path->GetJacobian(j->position[0], Jq);
	(Vec6d&)J[0] = Jq.row(5);
	PHTreeNode1D::CompJointJacobian();
}
void PHPathJointNode::CompJointCoriolisAccel(){
	cj.clear();
}
void PHPathJointNode::CompRelativeVelocity(){
	PHPathJoint* j = GetJoint();
	Matrix6d Jq;
	j->path->GetJacobian(j->position[0], Jq);
	(Vec6d&)j->vjrel = Jq.row(5) * j->velocity[0];
}
void PHPathJointNode::CompRelativePosition(){
	PHPathJoint* j = GetJoint();
	Posed p;
	j->path->GetPose(j->position[0], p);
	j->Xjrel.q = p.Ori();
	j->Xjrel.r = p.Pos();
}
void PHPathJointNode::UpdateJointPosition(double dt){
	PHTreeNode1D::UpdateJointPosition(dt);
	PHPathJoint* j = GetJoint();
	if(j->path->IsLoop())
		j->path->Rollover(j->position[0]);
}

}
