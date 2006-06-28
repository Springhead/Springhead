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
// PHSolidAux
void PHSolidInfoForLCP::SetupDynamics(double dt){
	minv = solid->GetMassInv();
	Iinv = solid->GetInertiaInv();
	Quaterniond q = solid->GetOrientation();
	v = q.Conjugated() * solid->GetVelocity();
	w = q.Conjugated() * solid->GetAngularVelocity();
	if(solid->IsDynamical()){
		f = q.Conjugated() * solid->nextForce;
		t = q.Conjugated() * solid->nextTorque;
		dv0 = minv * f * dt;
		dw0 = Iinv * (t - w % (solid->GetInertia() * w)) * dt;
	}
	else{
		dv0.clear();
		dw0.clear();
	}
	dv.clear();
	dw.clear();
}
/*void PHSolidInfoForLCP::SetupCorrection(){
	dV.clear();
	dW.clear();
}*/

//----------------------------------------------------------------------------
// PHConstraint
IF_OBJECT_IMP_ABST(PHConstraint, SceneObject);
PHConstraint::PHConstraint(){
	//f.clear();
	//F.clear();
	bEnabled = true;
}

bool PHConstraint::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s){
		PHScene* scene = DCAST(PHScene, GetScene());
		assert(scene);
		PHConstraintEngine* ce;
		scene->engines.Find(ce);
		assert(ce);
		PHSolidInfos<PHSolidInfoForLCP>::iterator it = ce->solids.Find(s);
		if(it == ce->solids.end())
			return false;
		if(!solid[0]){
			solid[0] = *it;
			return true;
		}
		if(!solid[1]){
			solid[1] = *it;
			return true;
		}
	}
	return false;

}

void PHConstraint::SetDesc(const PHConstraintDesc& desc){
	desc.posePlug.Ori().ToMatrix(Rj[0]);
	rj[0] = desc.posePlug.Pos();
	
	desc.poseSocket.Ori().ToMatrix(Rj[1]);
	rj[1] = desc.poseSocket.Pos();
	
	bEnabled = desc.bEnabled;
}

void PHConstraint::CompJacobian(bool bCompAngular){
	Matrix3d	R[2], Rjabs[2];
	Vec3d		r[2];
	R[0] = solid[0]->solid->GetRotation();
	R[1] = solid[1]->solid->GetRotation();
	r[0] = solid[0]->solid->GetCenterPosition();
	r[1] = solid[1]->solid->GetCenterPosition();
	Rjabs[0] = R[0] * Rj[0];
	Rjabs[1] = R[1] * Rj[1];
	Rjrel = Rjabs[0].trans() * Rjabs[1];
	qjrel.FromMatrix(Rjrel);
	rjrel = Rjabs[0].trans() * ((R[1] * rj[1] + r[1]) - (R[0] * rj[0] + r[0]));
	Jvv[0] = -Rj[0].trans();
	Jvw[0] = -Rj[0].trans() * (-Matrix3d::Cross(rj[0]));
	Jvv[1] =  Rjabs[0].trans() * R[1];
	Jvw[1] =  Jvv[1] * (-Matrix3d::Cross(rj[1]));
	vjrel = Jvv[0] * solid[0]->v + Jvw[0] * solid[0]->w + Jvv[1] * solid[1]->v + Jvw[1] * solid[1]->w;
	
	if(bCompAngular){
		Jwv[0].clear();
		Jww[0] = Jvv[0];
		Jwv[1].clear();
		Jww[1] = Jvv[1];
		wjrel = Jwv[0] * solid[0]->v + Jww[0] * solid[0]->w + Jwv[1] * solid[1]->v + Jww[1] * solid[1]->w;
		
		//角速度の左からかけるとquaternionの時間微分が得られる行列
		/*Matrix3d E(
			 qjrel.W(),  qjrel.Z(), -qjrel.Y(),
			-qjrel.Z(),  qjrel.W(),  qjrel.X(),
			 qjrel.Y(), -qjrel.X(),  qjrel.W());
		E *= 0.5;
		Jqv[0].clear();
		Jqw[0] = E * Jww[0];
		Jqv[1].clear();
		Jqw[1] = E * Jww[1];*/
	}
	int i, j;
	Av.clear();
	Aw.clear();
	for(i = 0; i < 2; i++){
		if(solid[i]->solid->IsDynamical()){
			Tvv[i] = Jvv[i] * solid[i]->minv;
			Tvw[i] = Jvw[i] * solid[i]->Iinv;
			Twv[i] = Jwv[i] * solid[i]->minv;
			Tww[i] = Jww[i] * solid[i]->Iinv;
			for(j = 0; j < 3; j++)
				Av[j] += Jvv[i].row(j) * Tvv[i].row(j) + Jvw[i].row(j) * Tvw[i].row(j);
			for(j = 0; j < 3; j++)
				Aw[j] += Jwv[i].row(j) * Twv[i].row(j) + Jww[i].row(j) * Tww[i].row(j);
		}
	}
	//特異姿勢でAd, Acの成分が0になるケースがある
	const double eps = 1.0e-3;
	for(j = 0; j < 3; j++)
		if(Av[j] < eps)Av[j] = eps;
	for(j = 0; j < 3; j++)
		if(Aw[j] < eps)Aw[j] = eps;

}

void PHConstraint::SetupDynamics(double dt, double correction_rate){
	bFeasible = solid[0]->solid->IsDynamical() || solid[1]->solid->IsDynamical();
	if(!bEnabled || !bFeasible)
		return;

	//各剛体の速度，角速度から相対速度，相対角速度へのヤコビ行列を計算
	//　接触拘束の場合は相対角速度へのヤコビ行列は必要ない
 	CompJacobian(GetConstraintType() != PHConstraintDesc::CONTACT);
	
	//相対速度，相対角速度から拘束速度へのヤコビ行列を計算
	//	拘束の種類ごとに異なる
	//CompConstraintJacobian();
	
	int i, j;
	for(i = 0; i < 2; i++){
		if(solid[i]->solid->IsDynamical()){
			solid[i]->dv += Tvv[i].trans() * fv + Twv[i].trans() * fw;
			solid[i]->dw += Tvw[i].trans() * fv + Tww[i].trans() * fw;
		}
	}

	bv = Jvv[0] * (solid[0]->v + solid[0]->dv0) +
		 Jvw[0] * (solid[0]->w + solid[0]->dw0) +
		 Jvv[1] * (solid[1]->v + solid[1]->dv0) +
		 Jvw[1] * (solid[1]->w + solid[1]->dw0);
	bw = Jwv[0] * (solid[0]->v + solid[0]->dv0) +
		 Jww[0] * (solid[0]->w + solid[0]->dw0) +
		 Jwv[1] * (solid[1]->v + solid[1]->dv0) +
		 Jww[1] * (solid[1]->w + solid[1]->dw0);
	
	CompBias(dt, correction_rate);	// 目標速度，バネダンパによる補正項を計算
	
	// iterationでの手間を省くためにあらかじめA行列の対角要素でbとJを割っておく
	double tmp;
	for(j = 0; j < 3; j++){
		tmp = 1.0 / Av[j];
		bv[j] *= tmp;
		Jvv[0].row(j) *= tmp;
		Jvw[0].row(j) *= tmp;
		Jvv[1].row(j) *= tmp;
		Jvw[1].row(j) *= tmp;
	}
	for(j = 0; j < 3; j++){
		tmp = 1.0 / Aw[j];
		bw[j] *= tmp;
		Jwv[0].row(j) *= tmp;
		Jww[0].row(j) *= tmp;
		Jwv[1].row(j) *= tmp;
		Jww[1].row(j) *= tmp;
	}
}

void PHConstraint::IterateDynamics(){
	if(!bEnabled || !bFeasible)return;

	Vec3d fvnew, fwnew, dfv, dfw;
	int i, j;
	for(j = 0; j < 3; j++){
		if(!constr[j])continue;
		fvnew[j] = fv[j] - (bv[j] + 
			Jvv[0].row(j) * (solid[0]->dv) + Jvw[0].row(j) * (solid[0]->dw) +
			Jvv[1].row(j) * (solid[1]->dv) + Jvw[1].row(j) * (solid[1]->dw));
		Projection(fvnew[j], j);
		dfv[j] = fvnew[j] - fv[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dv += Tvv[i].row(j) * dfv[j];
				solid[i]->dw += Tvw[i].row(j) * dfv[j];
			}
		}
		fv[j] = fvnew[j];
	}
	for(j = 0; j < 3; j++){
		if(!constr[j + 3])continue;
		fwnew[j] = fw[j] - (bw[j] + 
			Jwv[0].row(j) * (solid[0]->dv) + Jww[0].row(j) * (solid[0]->dw) +
			Jwv[1].row(j) * (solid[1]->dv) + Jww[1].row(j) * (solid[1]->dw));
		Projection(fwnew[j], j + 3);
		dfw[j] = fwnew[j] - fw[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dv += Twv[i].row(j) * dfw[j];
				solid[i]->dw += Tww[i].row(j) * dfw[j];
			}
		}
		fw[j] = fwnew[j];
	}
}

/*void PHConstraint::SetupCorrection(double dt, double max_error){
	if(!bEnabled || !bFeasible || dim_c == 0)return;

	CompError(dt);

	int i, j;
	for(i = 0; i < 2; i++){
		if(solid[i]->solid->IsDynamical()){
			solid[i]->dV += Tcv[i].trans() * F;
			solid[i]->dW += Tcw[i].trans() * F;
		}
	}
	
	Vec3d v[2], w[2];
	for(i = 0; i < 2; i++){
		v[i] = solid[i]->v + solid[i]->dv0 + solid[i]->dv;
		w[i] = solid[i]->w + solid[i]->dw0 + solid[i]->dw;
	}
	// velocity updateによる影響を加算
	//for(j = 0; j < dim_c; j++){
	//	B[j] += (Jcv[0].row(j) * v[0] + Jcw[0].row(j) * w[0] +
	//			 Jcv[1].row(j) * v[1] + Jcw[1].row(j) * w[1]) * dt;
	//}
	//一度に誤差を0にしようとすると振動的になるので適当に誤差を小さく見せる
	//誤差はmax_errorで飽和させる
	B *= 0.1;	
	double tmp = B[0];
	for(j = 1; j < dim_c; j++)
		if(tmp < B[j])
			tmp = B[j];
	if(tmp > max_error)
		B *= (max_error / tmp);
	//double tmp;
	for(j = 0; j < dim_c; j++){
		tmp = 1.0 / Ac[j];
		B[j] *= tmp;
		Jcv[0].row(j) *= tmp;
		Jcw[0].row(j) *= tmp;
		Jcv[1].row(j) *= tmp;
		Jcw[1].row(j) *= tmp;
	}
}

void PHConstraint::IterateCorrection(){
	if(!bEnabled || !bFeasible || dim_c == 0)return;

	Vec6d Fnew, dF;
	int i, j;
	for(j = 0; j < dim_c; j++){
		Fnew[j] = F[j] - (B[j] + 
			Jcv[0].row(j) * (solid[0]->dV) + Jcw[0].row(j) * (solid[0]->dW) +
			Jcv[1].row(j) * (solid[1]->dV) + Jcw[1].row(j) * (solid[1]->dW));
		ProjectionCorrection(Fnew[j], j);
		dF[j] = Fnew[j] - F[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dV += Tcv[i].row(j) * dF[j];
				solid[i]->dW += Tcw[i].row(j) * dF[j];
			}
		}
		F[j] = Fnew[j];
	}
}*/

}
