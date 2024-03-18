/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHConstraint.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHConstraintEngine.h>

#include <iomanip>
#include <fstream>

using namespace std;
using namespace PTM;
namespace Spr{

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHConstraint

// コンストラクタ
PHConstraint::PHConstraint() {
	solid[0] = solid[1] = NULL;

	f .clear();
	F .clear();
	
	bEnabled = true;
	bInactive[0] = true;
	bInactive[1] = true;
	treeNode = 0;
}

PHConstraint::~PHConstraint(){
}

// ----- エンジンから呼び出される関数
bool PHConstraint::IsArticulated(){
	return (treeNode && treeNode->IsEnabled());
}

bool PHConstraint::IsFeasible(){
	return solid[0]->IsDynamical() || solid[1]->IsDynamical();
}

void PHConstraint::UpdateState() {
	// 剛体の相対位置からヤコビアン，関節速度・位置を逆算する
	CompJacobian();

	// 親剛体の中心から見た速度と，子剛体の中心から見た速度を
	// Socket座標系から見た速度に両方直して，相対速度を取る．
	vjrel = J[1] * solid[1]->v + J[0] * solid[0]->v;

	// 関節座標の位置・速度を更新する
	UpdateJointState();
}

void PHConstraint::SetupAxisIndex(){
	// 拘束する自由度の決定
	//  可動自由度(movableAxes)の補集合が関節により拘束される自由度(axes)となる
	//  targetAxesはこのあとJointLimitとJointMotorによって適宜Enableされる
	axes.Clear();
	if(!IsArticulated()){
		for(int i = 0; i < 6; ++i) {
			bool bMovable = false;
			for (int n=0; n < movableAxes.size(); ++n) {
				if(i == movableAxes[n])
					bMovable = true;
			}
			if(bMovable)
				continue;
			axes.Enable(i);
		}
	}
	targetAxes = axes;
}

inline double Dot6(const double* v1, const double* v2){
#ifdef USE_AVX
	__m256d vec11, vec12;
	__m256d vec21, vec22;
	__m256d y1, y2;
	__m256d y;
	vec11 = _mm256_loadu_pd(v1);
	vec12 = _mm256_loadu_pd(v1+4);
	vec21 = _mm256_loadu_pd(v2);
	vec22 = _mm256_loadu_pd(v2+4);
	y1 = _mm256_mul_pd(vec11, vec21);
	y2 = _mm256_mul_pd(vec12, vec22);
	y  = _mm256_hadd_pd(y1, y2);
	double* _y = (double*)&y;
	return _y[0] + _y[1] + _y[2];
#else
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]*v2[3] + v1[4]*v2[4] + v1[5]*v2[5];
#endif
}

inline double QuadForm(const double* v1, const double* M, const double* v2){
	double y = 0.0;
	int k = 0;
	for(int i = 0; i < 6; i++)for(int j = 0; j < 6; j++, k++)
		y += v1[i] * M[k] * v2[j];
	return y;
}

void PHConstraint::CompResponseMatrix(){
	bool sameTree = (solid[0]->IsArticulated() && solid[1]->IsArticulated() && solid[0]->treeNode == solid[1]->treeNode);
	SpatialVector J0  ;
	SpatialVector J1  ;
	Vec3d Jv, Jw;
	const double* Minv;

	for(int n = 0; n < targetAxes.size(); n++){
		int j = targetAxes[n];
	
		A[j] = 0.0;
		
		for(int k = 0; k < 2; k++){
			if(!solid[k]->IsDynamical())
				continue;
	
			(Vec6d&)J0 = J[ k].row(j);
			(Vec6d&)J1 = J[!k].row(j);
			if(solid[k]->IsArticulated()){
				Minv = solid[k]->treeNode->dZdv_map[solid[k]->treeNode->id];
				A[j] += (-1.0) * QuadForm((const double*)&J0, Minv, (const double*)&J0);
			}
			else{
				Jv = J0.v();
				Jw = J0.w();
				A[j] += solid[k]->minv * Jv.square() +  Jw * (solid[k]->Iinv * Jw);
			}
			
			// 剛体が同じABAツリーに属する場合
			if(sameTree){
				Minv = solid[!k]->treeNode->dZdv_map[solid[k]->treeNode->id];
				A[j] += (-1.0) * QuadForm(J1, Minv, J0);
			}
		}
	}
}

void PHConstraint::Setup() {
	for(int k = 0; k < 2; k++){
		if(!solid[k]->IsDynamical() || !IsInactive(k))
			 solidState[k] = 0;
		else if(solid[k]->IsArticulated())
			 solidState[k] = 1;
		else solidState[k] = 2;
	}

	double fmax = GetScene()->GetMaxForce ();
	double tmax = GetScene()->GetMaxMoment();
	double dt   = GetScene()->GetTimeStep ();

	// 拘束力の最大・最小値をリセットする
	for(int i = 0; i < 3; i++) {
		fMinDt[i] = -fmax * dt;
		fMaxDt[i] =  fmax * dt;
	}
	for(int i = 3; i < 6; i++) {
		fMinDt[i] = -tmax * dt;
		fMaxDt[i] =  tmax * dt;
	}

	// A行列の対角成分
	CompResponseMatrix();

	// LCPの係数A, bの補正値dA, dbを計算
	dA.clear();
	db.clear();
	CompBias();

	// LCPのbベクトル == 論文中のw[t]を計算
	b  = J[0] * (solid[0]->v + solid[0]->dv0)
	   + J[1] * (solid[1]->v + solid[1]->dv0);
    b  += db;
	
	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];

		dA[j] += engine->regularization;
		Ainv[j] = engine->accelSOR / (A[j] + dA[j]);

		// 拘束力の初期値を更新
		//   拘束力は前回の値を縮小したものを初期値とする．
		//   前回の値そのままを初期値にすると，拘束力が次第に増大するという現象が生じる．
		//   これは，LCPを有限回（実際には10回程度）の反復で打ち切るためだと思われる．
		//   0ベクトルを初期値に用いても良いが，この場合比較的多くの反復回数を要する．
		f[j] *= engine->shrinkRate;
	}
}

bool PHConstraint::Iterate() {
	bool updated = false;
	for (int n=0; n<axes.size(); ++n) {
		int i = axes[n];
		
		// Gauss-Seidel Update
		dv[i] = Dot6((const double*)J[0].row(i), (const double*)solid[0]->dv)
			  + Dot6((const double*)J[1].row(i), (const double*)solid[1]->dv);
		res [i] = b[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - Ainv[i] * res[i];

		// Projection
		Projection(fnew[i], i);
		
		// Comp Response & Update f
		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];

		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponse(df[i], i);
		}
	}
	return updated;
}

void PHConstraint::SetupCorrection() {
	for (int n=0; n<axes.size(); ++n) {
		F[axes[n]] *= engine->shrinkRateCorrection;
	}
	B.clear();
	CompError();
	
	// velocity updateによる影響を加算
	B += ( J[0] * (solid[0]->v + solid[0]->dv0 + solid[0]->dv)
		 + J[1] * (solid[1]->v + solid[1]->dv0 + solid[1]->dv) ) * GetScene()->GetTimeStep();
	B *= engine->posCorrectionRate;
}

bool PHConstraint::IterateCorrection() {
	bool updated = false;

	for (int n=0; n<axes.size(); ++n) {
		int i = axes[n];
		dV[i] = J[0].row(i) * solid[0]->dV + J[1].row(i) * solid[1]->dV;
		Fnew[i] = F[i] - Ainv[i] * (B[i] + dV[i]);
		ProjectionCorrection(Fnew[i], i);
		dF[i] = Fnew[i] - F[i];
		F[i] = Fnew[i];
			
		if(std::abs(dF[i]) > engine->dfEps){
			updated = true;
			CompResponseCorrection(dF[i], i);
		}
	}

	return updated;
}

// ----- このクラスで実装する機能

void PHConstraint::CompJacobian(){ 
	// 拘束する2つの剛体の各速度から相対速度へのヤコビアンを計算
	// Xj[i] : 剛体の質量中心からソケット/プラグへの変換

	Xj[0].r    = poseSocket.Pos() - solid[0]->center;
	Xj[0].q    = poseSocket.Ori();
	Xj[1].r    = posePlug.Pos() - solid[1]->center;
	Xj[1].q	   = posePlug.Ori();

	X[0].r = solid[0]->GetCenterPosition();
	X[0].q = solid[0]->GetOrientation();
	X[1].r = solid[1]->GetCenterPosition();
	X[1].q = solid[1]->GetOrientation();
	Xjrel =  Xj[1] * X[1] * X[0].inv() * Xj[0].inv();

	Js[0] = Xj[0];
	Js[1] = Xjrel.inv() * Xj[1];
	J[0]  = Js[0];
	J[0] *= -1.0;	//反作用
	J[1]  = Js[1];
}

void PHConstraint::CompResponse(double df, int i) {
	SpatialVector Jrow;
	
	for(int k = 0; k < 2; k++){
		switch(solidState[k]){
		case 0:
			break;
		case 1:
			(Vec6d&)Jrow = J[k].row(i);
			solid[k]->treeNode->CompResponse(Jrow * df);
			break;
		case 2:
			(Vec6d&)Jrow = J[k].row(i);
			solid[k]->dv.v() += (solid[k]->minv * df) * Jrow.v();
			solid[k]->dv.w() += (solid[k]->Iinv * Jrow.w()) * df;
#if 0
			if (!isfinite(solid[k]->dv.norm())) {
				DSTR << " df" << df << " jrow" << Jrow << std::endl;
				DSTR << "dv is not finite" << std::endl;
			}
#endif
		}
	}
}

void PHConstraint::CompResponseCorrection(double dF, int i){
	SpatialVector dFs;
	for (int k=0; k<2; ++k) {
		if (!solid[k]->IsDynamical() || !IsInactive(k)) { continue; }
		(Vec6d&)dFs = J[k].row(i) * dF;
		if (solid[k]->IsArticulated()){
			solid[k]->treeNode->CompResponseCorrection(dFs);
		}
		else{
			solid[k]->dV.v() += solid[k]->minv * dFs.v();
			solid[k]->dV.w() += solid[k]->Iinv * dFs.w();
		}
	}
}

/// 拘束力の射影
bool PHConstraint::Projection(double& f_, int i) {
	f_ = min(max(fMinDt[i], f_), fMaxDt[i]);
	return false;
}

// ----- インタフェースの実装

void PHConstraint::GetRelativeVelocity(Vec3d &v, Vec3d &w) {
	for (int i=0; i<2; i++) {
		solid[i]->UpdateCacheLCP(GetScene()->GetTimeStep());
	}
	UpdateState();
	v = vjrel.v();
	w = vjrel.w();
}

void PHConstraint::GetConstraintForce(Vec3d& _f, Vec3d& _t) {
	_f = f.v() * GetScene()->GetTimeStepInv();
	_t = f.w() * GetScene()->GetTimeStepInv();
}

bool PHConstraint::AddChildObject(ObjectIf* o) {
	PHSolid* s = DCAST(PHSolid, o);
	if (s) {
		if (!solid[0]) {
			solid[0] = s;
			return true;
		}
		if (!solid[1]) {
			solid[1] = s;
			return true;
		}
	}
	return false;
}

size_t PHConstraint::NChildObject() const {
	return (solid[0] ? 1 : 0) + (solid[1] ? 1 : 0);
}

ObjectIf* PHConstraint::GetChildObject(size_t i) {
	return( (i<2) ? (solid[i]->Cast()) : ((ObjectIf*)(NULL)) );
}

bool PHConstraint::GetState(void* s) const {
	PHConstraintState* st = (PHConstraintState*)s;
	st->f        = f;
	st->F        = F;
	st->xs       = xs;
	st->fAvg     = fAvg;
	st->bYielded = bYielded;
	return true;
}

void PHConstraint::SetState(const void* s){
	const PHConstraintState* st = (const PHConstraintState*)s;
	(Vec6d&)f    = st->f       ;
	(Vec6d&)F    = st->F       ;
	(Vec6d&)xs   = st->xs      ;
	(Vec6d&)fAvg = st->fAvg    ;
	bYielded     = st->bYielded;

}

}
