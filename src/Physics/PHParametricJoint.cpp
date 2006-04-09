#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHJointTrajectory

PHJointTrajectory::iterator PHJointTrajectory::Find(double q){
	iterator it;
	for(it = begin(); it != end(); it++){
		if(it->q > q)
			return it;
	}
	return it;
}

void PHJointTrajectory::AddPoint(const Posed& pose, double q){
	PHJointTrajectoryPoint p;
	p.q = q;
	p.pose = pose;
	insert(Find(q), p);
}

//6x6行列Jの一番下の行ベクトルは与えられているとして，
//gram-schmidtの直交化で残る5本の行ベクトルを設定する
void Orthogonalize(Matrix6d& J){
	Vec6d v = J.row(5);
	v.unitize();
	int i, j, k;
	for(i = 4; i <= 0; i--){
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

void PHJointTrajectory::CreateDB(){
	double delta = (back().q - front().q) / 1000.0;		//数値微分の離散化幅．いいかげん．
	double div = 1.0 / (2.0 * delta);
	Posed p, p0, p1, pd;
	Vec3d v, w;
	Quaterniond qd;
	for(iterator it = begin(); it != end(); it++){
		//一般化座標qについて偏微分して相対速度を出す
		GetPose(p0, it->q - delta);
		GetPose(p1, it->q + delta);
		pd = (p1 - p0) * div;
		v = pd.Pos();
		qd = pd.Ori();
		w = (it->pose.Ori()).AngularVelocity(qd);		//1/2 * w * q = qd		=> 2 * qd * q~ = w
		it->J.row(5).SUBVEC(0, 3) =  v;
		it->J.row(5).SUBVEC(3, 3) =  w;
		Orthogonalize(it->J);
	}
}

void PHJointTrajectory::GetPose(Posed& pose, double q){
	iterator it = Find(q);
	if(it == begin()){
		pose = front().pose;
		return;
	}
	if(it == end()){
		pose = back().pose;
		return;
	}
	PHJointTrajectoryPoint &rhs = *it, &lhs = *--it;
	double tmp = 1.0 / (rhs.q - lhs.q);
	pose = ((rhs.q - q) * tmp) * lhs.pose + ((q - lhs.q) * tmp) * rhs.pose;
}

void PHJointTrajectory::GetJacobian(Matrix6d& J, double q){
	iterator it = Find(q);
	if(it == begin()){
		J = front().J;
		return;
	}
	if(it == end()){
		J = back().J;
		return;
	}
	PHJointTrajectoryPoint &rhs = *it, &lhs = *--it;
	double tmp = 1.0 / (rhs.q - lhs.q);
	J = ((rhs.q - q) * tmp) * lhs.J + ((q - lhs.q) * tmp) * rhs.J;
}

//----------------------------------------------------------------------------
// PHParametricJoint

IF_IMP(PHParametricJoint, PHJoint1D)

PHParametricJoint::PHParametricJoint(){
	q = qd = 0.0;
}

double PHParametricJoint::GetPosition(){
	return q;
}

double PHParametricJoint::GetVelocity(){
	return qd;
}

void PHParametricJoint::CompConstraintJacobian(){
	CompDof();
	Matrix6d J;
	trajectory->GetJacobian(J, q);
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
			solid[i]->dv += Tdv[i].row(5) * torque;
			solid[i]->dw += Tdw[i].row(5) * torque;
			Tcv[i] = Jcv[i] * solid[i]->minv;
			Tcw[i] = Jcw[i] * solid[i]->Iinv;
			for(int j = 0; j < 6; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
			for(int j = 0; j < 6; j++)
				Ac[j] += Jcv[i].row(j) * Tcv[i].row(j) + Jcw[i].row(j) * Tcw[i].row(j);
		}
	}
}

void PHParametricJoint::CompBias(double dt){

}

void PHParametricJoint::CompError(double dt){
	//B.SUBVEC(0, 3) = rjrel;
	//B.SUBVEC(3, 3) = qjrel.V();
	PHConstraint::CompError();
	
	//velocity update後の関節速度の値
	Vec3d v[2], w[2];
	for(int i = 0; i < 2; i++){
		v[i] = solid[i]->v + solid[i]->dv0 + solid[i]->dv;
		w[i] = solid[i]->w + solid[i]->dw0 + solid[i]->dw;
	}
	double qd = Jdv[0].row(5) * v[0] + Jdw[0].row(5) * w[0] +
				Jdv[1].row(5) * v[1] + Jdw[1].row(5) * w[1];
	q += qd * dt;
	Posed pnew;
	trajectory->GetPose(pnew, q);
	B.SUBVEC(0, 3) -= pnew.Pos();
	B.SUBVEC(3, 3) -= pnew.Ori().V();
}

void PHParametricJoint::ProjectionDynamics(double& f, int k){

}

void PHParametricJoint::ProjectionCorrection(double& F, int k){

}

}
