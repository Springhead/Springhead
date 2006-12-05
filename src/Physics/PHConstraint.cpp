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
// PHConstraint
IF_OBJECT_IMP_ABST(PHConstraint, SceneObject);
PHConstraint::PHConstraint(){
	bEnabled = true;
	bInactive[0] = true;
	bInactive[1] = true;
	bArticulated = false;
}

bool PHConstraint::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s){
		PHSolids::iterator it = scene->constraintEngine->solids.Find(s);
		if(it == scene->constraintEngine->solids.end())
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
	desc.poseSocket.Ori().ToMatrix(Xj[0].R);
	Xj[0].r = desc.poseSocket.Pos();
	desc.posePlug.Ori().ToMatrix(Xj[1].R);
	Xj[1].r = desc.posePlug.Pos();
	bEnabled = desc.bEnabled;
}

void PHConstraint::UpdateState(){
	// ツリーを構成していない拘束の場合，剛体の相対位置からヤコビアン，関節速度・位置を逆算する
	if(!bArticulated){
		CompJacobian();
		vjrel = Js[1] * solid[1]->v - Js[0] * solid[0]->v;
		UpdateJointState();
	}
}

// 拘束する2つの剛体の各速度から相対速度へのヤコビアンを計算
void PHConstraint::CompJacobian(){
	SpatialTransform X[2];
	Matrix3d	Rjabs[2];
	X[0].R = solid[0]->GetRotation();
	X[0].r = solid[0]->GetCenterPosition();
	X[1].R = solid[1]->GetRotation();
	X[1].r = solid[1]->GetCenterPosition();
	Xjrel = Xj[1] * X[1] * X[0].inv() * Xj[0].inv();
	qjrel.FromMatrix(Xjrel.R);
	
	Js[0] = Xj[0];
	Js[1] = Xjrel.inv() * Xj[1];
	J[0] = Js[0];
	J[0] *= -1.0;	//反作用
	J[1] = Js[1];
	
	/*
	//角速度の左からかけるとquaternionの時間微分が得られる行列
	Matrix3d E(
			qjrel.W(),  qjrel.Z(), -qjrel.Y(),
		-qjrel.Z(),  qjrel.W(),  qjrel.X(),
			qjrel.Y(), -qjrel.X(),  qjrel.W());
	E *= 0.5;
	Jqv[0].clear();
	Jqw[0] = E * Jww[0];
	Jqv[1].clear();
	Jqw[1] = E * Jww[1];
	*/
}

/*	Aの対角成分を計算する．A = J * M^-1 * J^T
	A行列は拘束力から速度変化への影響の強さを表す行列なので，
	その対角成分はある拘束力成分から自分自身の拘束速度成分への影響の強さを表す
 */
void PHConstraint::CompResponseMatrix(){
	int i, j;
	A.v.clear();
	A.w.clear();
	PHRootNode* root[2];
	if(solid[0]->treeNode)
		root[0] = solid[0]->treeNode->GetRootNode();
	if(solid[1]->treeNode)
		root[1] = solid[1]->treeNode->GetRootNode();

	SpatialVector df;
	for(i = 0; i < 2; i++){
		if(solid[i]->IsDynamical()){
			if(solid[i]->treeNode){
				for(j = 0; j < 3; j++){
					df.v = J[i].vv.row(j);
					df.w = J[i].vw.row(j);
					solid[i]->treeNode->CompResponse(df, false);
					A.v[j] += J[i].vv.row(j) * solid[i]->treeNode->da.v
							+ J[i].vw.row(j) * solid[i]->treeNode->da.w;
					int ic = !i;
					//もう片方の剛体も同一のツリーに属する場合はその影響項も加算
					if(solid[ic]->treeNode && root[i] == root[ic])
						A.v[j] += J[ic].vv.row(j) * solid[ic]->treeNode->da.v
								+ J[ic].vw.row(j) * solid[ic]->treeNode->da.w;
				}
				for(j = 0; j < 3; j++){
					df.v = J[i].wv.row(j);
					df.w = J[i].ww.row(j);
					solid[i]->treeNode->CompResponse(df, false);
					A.w[j] += J[i].wv.row(j) * solid[i]->treeNode->da.v
							+ J[i].ww.row(j) * solid[i]->treeNode->da.w;
					int ic = !i;
					if(solid[ic]->treeNode && root[i] == root[ic])
						A.w[j] += J[ic].wv.row(j) * solid[ic]->treeNode->da.v
								+ J[ic].ww.row(j) * solid[ic]->treeNode->da.w;
				}
			}
			else{
				T[i].vv = J[i].vv * solid[i]->minv;
				T[i].vw = J[i].vw * solid[i]->Iinv;
				T[i].wv = J[i].wv * solid[i]->minv;
				T[i].ww = J[i].ww * solid[i]->Iinv;
				for(j = 0; j < 3; j++)
					A.v[j] += J[i].vv.row(j) * T[i].vv.row(j) + J[i].vw.row(j) * T[i].vw.row(j);
				for(j = 0; j < 3; j++)
					A.w[j] += J[i].wv.row(j) * T[i].wv.row(j) + J[i].ww.row(j) * T[i].ww.row(j);
			}
		}
	}
	//特異姿勢でAd, Acの成分が0になるケースがある
	const double eps = 1.0e-3;
	for(j = 0; j < 3; j++)
		if(A.v[j] < eps)A.v[j] = eps;
	for(j = 0; j < 3; j++)
		if(A.w[j] < eps)A.w[j] = eps;
}


void PHConstraint::SetupDynamics(){
	FPCK_FINITE(f.v);

	bFeasible = solid[0]->IsDynamical() || solid[1]->IsDynamical();
	if(!bEnabled || !bFeasible || bArticulated)
		return;

	/* 前回の値を縮小したものを初期値とする．
	   前回の値そのままを初期値にすると，拘束力が次第に増大するという現象が生じる．
	   これは，LCPを有限回（実際には10回程度）の反復で打ち切るためだと思われる．
	   0ベクトルを初期値に用いても良いが，この場合比較的多くの反復回数を要する．
	  */
	f *= engine->shrinkRate;
	if(mode == MODE_TORQUE)
		AddMotorTorque();

	// 制約する自由度の決定
	CompDof();

	// LCPの座標の取り方が特殊な関節はヤコビアンに座標変換をかける
	ModifyJacobian();

	// LCPのbベクトル
	//b = vjrel;
	b = J[0] * solid[0]->v + J[1] * solid[1]->v;
	CompBias();	// 目標速，バネダンパによる補正項を計算
	b += db;
	
	// LCPのA行列の対角成分を計算
	CompResponseMatrix();

	// 拘束力初期値による速度変化量を計算
	int i, j;
	SpatialVector fs;
	for(i = 0; i < 2; i++){
		if(solid[i]->IsDynamical() && IsInactive(i)){
			if(solid[i]->treeNode){
				fs = (i == 0 ? -1.0 : 1.0) * (Js[i].trans() * f);
				solid[i]->treeNode->CompResponse(fs);
			}
			else{
				solid[i]->dv.v += T[i].vv.trans() * f.v + T[i].wv.trans() * f.w;
				solid[i]->dv.w += T[i].vw.trans() * f.v + T[i].ww.trans() * f.w;
			}
		}
	}

	// 反復での手間を省くためにあらかじめA行列の対角成分でbとJを割っておく
	for(j = 0; j < 3; j++){
		Ainv.v[j] = 1.0 / (A.v[j] + dA.v[j]);
		dA.v[j] *= Ainv.v[j];
		b.v[j] *= Ainv.v[j];
		AinvJ[0].vv.row(j) = Ainv.v[j] * J[0].vv.row(j);
		AinvJ[0].vw.row(j) = Ainv.v[j] * J[0].vw.row(j);
		AinvJ[1].vv.row(j) = Ainv.v[j] * J[1].vv.row(j);
		AinvJ[1].vw.row(j) = Ainv.v[j] * J[1].vw.row(j);
	}
	for(j = 0; j < 3; j++){
		Ainv.w[j] = 1.0 / (A.w[j] + dA.w[j]);
		dA.w[j] *= Ainv.w[j];
		b.w[j] *= Ainv.w[j];
		AinvJ[0].wv.row(j) = Ainv.w[j] * J[0].wv.row(j);
		AinvJ[0].ww.row(j) = Ainv.w[j] * J[0].ww.row(j);
		AinvJ[1].wv.row(j) = Ainv.w[j] * J[1].wv.row(j);
		AinvJ[1].ww.row(j) = Ainv.w[j] * J[1].ww.row(j);
	}
}

void PHConstraint::IterateDynamics(){
	if(!bEnabled || !bFeasible || bArticulated)return;
	FPCK_FINITE(f.v);

	SpatialVector fnew, df;
	int i, j;
	for(j = 0; j < 3; j++){
		if(!constr[j])continue;
		fnew.v[j] = (1.0 - dA.v[j]) * f.v[j] - (b.v[j] + 
			AinvJ[0].vv.row(j) * (solid[0]->dv.v) + AinvJ[0].vw.row(j) * (solid[0]->dv.w) +
			AinvJ[1].vv.row(j) * (solid[1]->dv.v) + AinvJ[1].vw.row(j) * (solid[1]->dv.w));
		FPCK_FINITE(AinvJ[0].vv.row(j));
		FPCK_FINITE(AinvJ[1].vv.row(j));
		if (!FPCK_FINITE(fnew.v)){
			FPCK_FINITE(b.v);
			DSTR << AinvJ[0].vv << AinvJ[1].vv;
			DSTR << AinvJ[0].vw << AinvJ[1].vw;
			DSTR << dA.v[j];
			DSTR << std::endl;
			DSTR << "f.v:" << f.v << "b.v:" << b.v << std::endl;
			DSTR << "s0:" << (solid[0]->dv.v) << (solid[0]->dv.w) << std::endl;
			DSTR << "s1:" << (solid[1]->dv.v) << (solid[1]->dv.w) << std::endl;
		}
		Projection(fnew.v[j], j);
		FPCK_FINITE(fnew.v);
		df.v[j] = fnew.v[j] - f.v[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->IsDynamical() && IsInactive(i)){
				if(solid[i]->treeNode){
					SpatialVector dfs;
					dfs.v = df[j] * J[i].vv.row(j);
					dfs.w = df[j] * J[i].vw.row(j);
					solid[i]->treeNode->CompResponse(dfs);
				}
				else{
					solid[i]->dv.v += T[i].vv.row(j) * df.v[j];
					solid[i]->dv.w += T[i].vw.row(j) * df.v[j];
				}
			}
		}
		f.v[j] = fnew.v[j];
	}
	for(j = 0; j < 3; j++){
		if(!constr[j + 3])continue;
		fnew.w[j] = (1.0 - dA.w[j]) * f.w[j] - (b.w[j] + 
			AinvJ[0].wv.row(j) * (solid[0]->dv.v) + AinvJ[0].ww.row(j) * (solid[0]->dv.w) +
			AinvJ[1].wv.row(j) * (solid[1]->dv.v) + AinvJ[1].ww.row(j) * (solid[1]->dv.w));
		Projection(fnew.w[j], j + 3);
		df.w[j] = fnew.w[j] - f.w[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->IsDynamical() && IsInactive(i)){
				if(solid[i]->treeNode){
					SpatialVector dfs;
					dfs.v = df[j] * J[i].wv.row(j);
					dfs.w = df[j] * J[i].ww.row(j);
					solid[i]->treeNode->CompResponse(dfs);
				}
				else{
					solid[i]->dv.v += T[i].wv.row(j) * df.w[j];
					solid[i]->dv.w += T[i].ww.row(j) * df.w[j];
				}
			}
		}
		f.w[j] = fnew.w[j];
	}
	FPCK_FINITE(fnew.v);
	FPCK_FINITE(fnew.w);
	FPCK_FINITE(df.v);
	FPCK_FINITE(df.w);

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
