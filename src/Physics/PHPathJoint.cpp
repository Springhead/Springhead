/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHPathJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
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

double PHPath::Rollover(double s){
	double lower = front().s;
	double upper = back().s;
	assert(lower < upper);
	double range = upper - lower;
	while(s >= upper)s -= range;
	while(s <  lower)s += range;
	return s;
}

PHPath::iterator PHPath::Find(double& s){
	if(size() <= 1)return begin();
	if(bLoop)
		s = Rollover(s);
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
void Orthogonalize(SpatialMatrix& J){
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

		SpatialVector V;
		V.v() = v;
		V.w() = w;
		it->J.row(5) = V;
		Orthogonalize(it->J);
	}

	// ヤコビアンの滑らかさ確認
	/*for(int i = 0; i < size()-1; i++){
		DSTR << at(i).J.row(5) * at(i+1).J.row(5) << std::endl;
	}*/
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

void PHPath::GetJacobian(double s, SpatialMatrix& J){
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
	(Matrix6d&)J = ((rhs.s - s) * tmp) * lhs.J + ((s - lhs.s) * tmp) * rhs.J;
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHPathJointNode

void PHPathJointNode::CompJointJacobian(){
	PHPathJoint* j = GetJoint();
	SpatialMatrix Jq;
	j->path->GetJacobian(j->position[0], Jq);
	J.col(0) = Jq.row(5);
	PHTreeNode1D::CompJointJacobian();
}

void PHPathJointNode::CompJointCoriolisAccel(){
	cj.clear();
}

void PHPathJointNode::CompRelativeVelocity(){
	PHPathJoint* j = GetJoint();
	SpatialMatrix Jq;
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
		j->position[0] = j->path->Rollover(j->position[0]);
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHPathJoint

PHPathJoint::PHPathJoint(const PHPathJointDesc& desc){
	SetDesc(&desc);
	
	movableAxes.Enable(5);
}

// ----- エンジンから呼び出される関数

void PHPathJoint::UpdateJointState(){
	/*Matrix6d J;
	path->GetJacobian(position[0], J);
	velocity[0] = vjrel.norm();
	if(vjrel * J.row(5) < 0.0)
		velocity[0] = -velocity[0];
	position[0] += velocity[0] * GetScene()->GetTimeStep();
	path->Rollover(position[0]);*/
}

// ----- PHConstraintの派生クラスで実装される機能

void PHPathJoint::ModifyJacobian(){
	/*Matrix6d Jq;
	path->GetJacobian(position[0], Jq);
	(Matrix6d&)J[0] = Jq * J[0];
	(Matrix6d&)J[1] = Jq * J[1];*/
}

void PHPathJoint::CompBias(){
	/*
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	Posed p;
	path->GetPose(position[0], p);
	db.v() = ((Xjrel.r - p.Pos()) * dtinv);
	db.w().clear();
	Matrix6d Jq;
	path->GetJacobian(position[0], Jq);
	(Vec6d&)db = Jq * db;
	db.w().z = 0.0;
	db *= engine->velCorrectionRate;
	*/
	// 親クラスのCompBias．motor,limitのCompBiasが呼ばれるので最後に呼ぶ
	PH1DJoint::CompBias();
}

// ----- インタフェースの実装

bool PHPathJoint::AddChildObject(ObjectIf* o){
	PHPath* p = DCAST(PHPath, o);
	if(p){
		path = p;
		//可動範囲はリセットされる
		// 周期パスならば可動範囲無し
		// 非周期パスならば初端と終端を可動範囲とする
		if (path->IsLoop()) {
			if (limit) { limit->SetRange(Vec2d(0,0)); }
		} else {
			if (limit) { limit->SetRange(Vec2d(path->front().s, path->back().s)); }
		}
		position[0] = velocity[0] = 0.0;
		return true;
	}
	return PHConstraint::AddChildObject(o);
}

}
