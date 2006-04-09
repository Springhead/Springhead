#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHPath
IF_IMP(PHPath, SceneObject)

PHPath::iterator PHPath::Find(double s){
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
	insert(Find(s), p);
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
		v = pd.Pos();
		qd = pd.Ori();
		w = (it->pose.Ori()).AngularVelocity(qd);		//1/2 * w * q = qd		=> 2 * qd * q~ = w
		it->J.row(5).SUBVEC(0, 3) =  v;
		it->J.row(5).SUBVEC(3, 3) =  w;
		Orthogonalize(it->J);
	}
}

void PHPath::GetPose(double s, Posed& pose){
	iterator it = Find(s);
	if(it == begin()){
		pose = front().pose;
		return;
	}
	if(it == end()){
		pose = back().pose;
		return;
	}
	PHPathPointWithJacobian &rhs = *it, &lhs = *--it;
	double tmp = 1.0 / (rhs.s - lhs.s);
	pose = ((rhs.s - s) * tmp) * lhs.pose + ((s - lhs.s) * tmp) * rhs.pose;
}

void PHPath::GetJacobian(double s, Matrix6d& J){
	iterator it = Find(s);
	if(it == begin()){
		J = front().J;
		return;
	}
	if(it == end()){
		J = back().J;
		return;
	}
	PHPathPointWithJacobian &rhs = *it, &lhs = *--it;
	double tmp = 1.0 / (rhs.s - lhs.s);
	J = ((rhs.s - s) * tmp) * lhs.J + ((s - lhs.s) * tmp) * rhs.J;
}

//----------------------------------------------------------------------------
// PHPathJoint

IF_IMP(PHPathJoint, PHJoint1D)

PHPathJoint::PHPathJoint(){
	q = qd = 0.0;
}

bool PHPathJoint::AddChildObject(ObjectIf* o){
	PHPath* p = OCAST(PHPath, o);
	if(p){
		path = p;
		return true;
	}
	return PHConstraint::AddChildObject(o);
}

double PHPathJoint::GetPosition(){
	return q;
}

double PHPathJoint::GetVelocity(){
	return qd;
}

void PHPathJoint::CompConstraintJacobian(){
	if(!path)return;
	CompDof();
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

void PHPathJoint::CompBias(double dt){

}

void PHPathJoint::CompError(double dt){
	if(!path)return;
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
	path->GetPose(q, pnew);
	B.SUBVEC(0, 3) -= pnew.Pos();
	B.SUBVEC(3, 3) -= pnew.Ori().V();
}

void PHPathJoint::ProjectionDynamics(double& f, int k){

}

void PHPathJoint::ProjectionCorrection(double& F, int k){

}

}
