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
	dv.clear();
	dV.clear();

	bEnabled = true;
	bInactive[0] = true;
	bInactive[1] = true;
	treeNode = 0;

#ifdef USE_OPENMP_PHYSICS
	omp_init_lock(&dv_lock);
#endif
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

void PHConstraint::Setup() {
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

	// LCPの係数A, bの補正値dA, dbを計算
	dA.clear();
	db.clear();
	CompBias();

	// LCPのbベクトル == 論文中のw[t]を計算
	b  = J[0] * solid[0]->v  + J[1] * solid[1]->v;
	dv = J[0] * solid[0]->dv + J[1] * solid[1]->dv;

	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];

		Ainv[j] = 1.0 / (A[j] + dA[j]);

		// 拘束力の初期値を更新
		//   拘束力は前回の値を縮小したものを初期値とする．
		//   前回の値そのままを初期値にすると，拘束力が次第に増大するという現象が生じる．
		//   これは，LCPを有限回（実際には10回程度）の反復で打ち切るためだと思われる．
		//   0ベクトルを初期値に用いても良いが，この場合比較的多くの反復回数を要する．
		f[j] *= engine->shrinkRate;
	}
}

void PHConstraint::Iterate() {
	for (int n=0; n<axes.size(); ++n) {
		int i = axes[n];

		// Gauss-Seidel Update
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];

		// Projection
		Projection(fnew[i], i);
		
		// tightening
		// 多点接触などにおいて拘束を満たす力の組み合わせが一意でないとき
		// なるべく最小エネルギ解に近づくように微小収縮させる　tazz
		//fnew[i] *= 0.99;

		// Comp Response & Update f
		df[i] = fnew[i] - f[i];
		CompResponseDirect(df[i], i);
		f[i] = fnew[i];
	}
}

void PHConstraint::SetupCorrection() {
	for (int n=0; n<axes.size(); ++n) {
		F[axes[n]] *= engine->shrinkRateCorrection;
	}
	B.clear();
	CompError();
	
	// velocity updateによる影響を加算
	B += (J[0] * (solid[0]->v + solid[0]->dv) + J[1] * (solid[1]->v + solid[1]->dv)) * GetScene()->GetTimeStep();
	B *= engine->posCorrectionRate;
		
}

void PHConstraint::IterateCorrection() {
	SpatialVector Fnew;

	for (int n=0; n<axes.size(); ++n) {
		int i = axes[n];
		Fnew[i] = F[i] - Ainv[i] * (B[i] + J[0].row(i) * solid[0]->dV + J[1].row(i) * solid[1]->dV);
		ProjectionCorrection(Fnew[i], i);
		dF[i] = Fnew[i] - F[i];
		CompResponseDirectCorrection(dF[i], i);
		F[i] = Fnew[i];
	}
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
	SpatialVector dfs;
	for (int k=0; k<2; ++k) {
		if (!solid[k]->IsDynamical() || !IsInactive(k)) { continue; }
		(Vec6d&)dfs = J[k].row(i) * df;
		if (solid[k]->IsArticulated()) {
			solid[k]->treeNode->CompResponse(dfs);
		}
		else {
			solid[k]->dv += solid[k]->Minv * dfs;
		}
	}
} 
void PHConstraint::CompResponseCorrection(double dF, int i){
	SpatialVector dFs;
	for (int k=0; k<2; ++k) {
		if (!solid[k]->IsDynamical() || !IsInactive(k)) { continue; }
		(Vec6d&)dFs = J[k].row(i) * dF;
		if (solid[k]->IsArticulated())
			 solid[k]->treeNode->CompResponse(dFs);
		else solid[k]->dV += solid[k]->Minv * dFs;
	}
}
void PHConstraint::CompResponseDirect(double df, int i){
	PHConstraint* dest;
	
	for(int j = 0; j < adj.num; j++){
		dest = adj[j].con;

		for(int n1 = 0; n1 < dest->targetAxes.size(); n1++){
			int i1 = dest->targetAxes[n1];
			
#ifdef USE_OPENMP_PHYSICS
			omp_set_lock(&dest->dv_lock);
#endif
			dest->dv[i1] += adj[j].A[i1][i] * df;

#ifdef USE_OPENMP_PHYSICS
			omp_unset_lock(&dest->dv_lock);
#endif
		}

	}
}
void PHConstraint::CompResponseDirectCorrection(double dF, int i){
	for(int j = 0; j < adj.num; j++){
		PHConstraint* dest = adj[j].con;
		for(int n1 = 0; n1 < dest->targetAxes.size(); n1++){
			int i1 = dest->targetAxes[n1];
			dest->dV[i1] += adj[j].A[i1][i] * dF;
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
