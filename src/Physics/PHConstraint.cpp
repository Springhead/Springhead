/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#include <Base/VectorDebug.h>
#pragma hdrstop

using namespace std;
using namespace PTM;
namespace Spr{;

//----------------------------------------------------------------------------
// PHConstraint
//コンストラクタ
PHConstraint::PHConstraint(){
	solid[0] = solid[1] = NULL;
	f.clear();
	F.clear();
	bEnabled = true;
	bInactive[0] = true;
	bInactive[1] = true;
	bArticulated = false;	
}

PHSceneIf* PHConstraint::GetScene() const{
	return DCAST(PHSceneIf, SceneObject::GetScene());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// インタフェース(PHConstraintIfの機能)の実装,オーバーライド   cf.SprPHJoint.h

bool PHConstraint::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s){
		//PHSolids::iterator it = (PHSolids::iterator) GetScene()->constraintEngine->solids.Find(s);
		//if(it == GetScene()->constraintEngine->solids.end())
		//	return false;
		if(!solid[0]){
			solid[0] = s;
			return true;
		}
		if(!solid[1]){
			solid[1] = s;
			return true;
		}
	}
	return false;
}
size_t PHConstraint::NChildObject(){
	return (solid[0] ? 1 : 0) + (solid[1] ? 1 : 0);
}
ObjectIf* PHConstraint::GetChildObject(size_t i){
	return solid[i]->Cast();
}

void PHConstraint::AfterSetDesc(){
	SceneObject::AfterSetDesc();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// このクラス内の機能.

void PHConstraint::UpdateState(){
	// 剛体の相対位置からヤコビアン，関節速度・位置を逆算する
	CompJacobian();
	//親剛体の中心から見た速度と，子剛体の中心から見た速度をSocket座標系から見た速度に両方直して，相対速度を取る．
	vjrel = Js[1] * solid[1]->v - Js[0] * solid[0]->v;
	UpdateJointState();
}


void PHConstraint::CompJacobian(){ // 拘束する2つの剛体の各速度から相対速度へのヤコビアンを計算
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
	A.clear();
	PHRootNode* root[2];
	if(solid[0]->IsArticulated())
		root[0] = solid[0]->treeNode->GetRootNode();
	if(solid[1]->IsArticulated())
		root[1] = solid[1]->treeNode->GetRootNode();

	SpatialVector df;
	for(i = 0; i < 2; i++){
		if(solid[i]->IsDynamical()){
			if(solid[i]->IsArticulated()){
				for(j = 0; j < 6; j++){
					(Vec6d&)df = J[i].row(j);
					solid[i]->treeNode->CompResponse(df, false, false);
					A[j] += J[i].row(j) * solid[i]->treeNode->da;
					int ic = !i;
					//もう片方の剛体も同一のツリーに属する場合はその影響項も加算
					if(solid[ic]->IsArticulated() && root[i] == root[ic])
						A[j] += J[ic].row(j) * solid[ic]->treeNode->da;
				}
			}
			else{
				// T = M^-1 * J^T
				T[i].vv() = J[i].vv() * solid[i]->minv;
				T[i].vw() = J[i].vw() * solid[i]->Iinv;
				T[i].wv() = J[i].wv() * solid[i]->minv;
				T[i].ww() = J[i].ww() * solid[i]->Iinv;
				for(j = 0; j < 6; j++)
					// A == 論文中のJ * M^-1 * J^T, Gauss Seidel法のD
					A[j] += J[i].row(j) * T[i].row(j);
			}
		}
	}
	/** 最大の対角要素との比がepsよりも小さい対角要素がある場合，
		数値的不安定性の原因となるのでその成分は拘束対象から除外する
		＊epsを大きくとると，必要な拘束まで無効化されてしまうので、調整は慎重に。
	 */
	const double eps = 0.000001, epsabs = 1.0e-10;
	double Amax = 0.0, Amin;
	for(j = 0; j < 6; j++)
		if(constr[j] && A[j] > Amax)
			Amax = A[j];
	Amin = Amax * eps;
	for(j = 0; j < 6; j++){
		if(!constr[j])continue;
		if(A[j] < Amin || A[j] < epsabs){
			constr[j] = false;
			DSTR <<this->GetName()<<":"<< j << "-th constraint ill-conditioned! disabled." << endl;
		}
		else
			Ainv[j] = 1.0 / (A[j] + dA[j]);
	}
}

void PHConstraint::CompResponse(double df, int j){
	SpatialVector dfs;
	for(int i = 0; i < 2; i++){
		if(!solid[i]->IsDynamical() || !IsInactive(i))continue;
		if(solid[i]->IsArticulated()){
			(Vec6d&)dfs = J[i].row(j) * df;
			solid[i]->treeNode->CompResponse(dfs, true, false);
		}
		else solid[i]->dv += T[i].row(j) * df;
	}
}

void PHConstraint::SetupLCP(){
	bFeasible = solid[0]->IsDynamical() || solid[1]->IsDynamical();
	if(!bEnabled || !bFeasible)
		return;

	/* 拘束力は前回の値を縮小したものを初期値とする．
	   前回の値そのままを初期値にすると，拘束力が次第に増大するという現象が生じる．
	   これは，LCPを有限回（実際には10回程度）の反復で打ち切るためだと思われる．
	   0ベクトルを初期値に用いても良いが，この場合比較的多くの反復回数を要する．
	  */
	
	// 拘束する自由度の決定，拘束力の初期化
	//bool con[6];
	SetConstrainedIndex(constr);
	for(int i = 0; i < 6; i++){
		//if(con[i] && constr[i]){				// 継続して拘束される場合
			f[i] *= engine->shrinkRate;
		//}else{
		//	f[i] = 0.0;							// 新規に拘束される or 拘束されない
		//}
		//constr[i] = con[i];
	}

	FPCK_FINITE(f.v());

	// LCPの座標の取り方が特殊な関節はヤコビアンに座標変換をかける
	ModifyJacobian();

	// LCPの係数A, bの補正値dA, dbを計算
	dA.clear();
	db.clear();
	
	// LCPのA行列の対角成分を計算
	CompResponseMatrix();

	// ABAの場合はここまで
	if(bArticulated)return;

	CompBias();	// 誤差修正のための補正値．
	
	// LCPのbベクトル == 論文中のw[t], バネ・ダンパはdbで補正する
	b = J[0] * solid[0]->v + J[1] * solid[1]->v;	//vjrelではない	値によっては変わって拘束しなくなる

	// 拘束力初期値による速度変化量を計算
	SpatialVector fs;
	for(int i = 0; i < 2; i++){
		if(!solid[i]->IsDynamical() || !IsInactive(i))continue;
		if(solid[i]->IsArticulated()){
			(Vec6d&)fs = J[i].trans() * f;
			solid[i]->treeNode->CompResponse(fs, true, false);
		}
		else solid[i]->dv += T[i].trans() * f;
	}

}

void PHConstraint::IterateLCP(){
	if(!bEnabled || !bFeasible || bArticulated)
		return;
	FPCK_FINITE(f.v());

	SpatialVector fnew, df;
	for(int j = 0; j < 6; j++){
		if(!constr[j])continue;
		
		fnew[j] = f[j] - engine->accelSOR * Ainv[j] * (dA[j] * f[j] + b[j] + db[j] 
				+ J[0].row(j) * solid[0]->dv + J[1].row(j) * solid[1]->dv);

		// とりあえず落ちないように間に合わせのコード
		if (!FPCK_FINITE(fnew[j])) fnew[j] = f[j]; //naga 特定条件下では間に合わせのコードでも落ちる

		if (!FPCK_FINITE(fnew[0])){
			FPCK_FINITE(b[0]);
//			DSTR << AinvJ[0].vv << AinvJ[1].vv;
//			DSTR << AinvJ[0].vw << AinvJ[1].vw;
//			DSTR << dA.v[j];
//			DSTR << std::endl;
//			DSTR << "f.v:" << f.v << "b.v:" << b.v << std::endl;
			DSTR << "s0:" << (solid[0]->dv) << std::endl;
			DSTR << "s1:" << (solid[1]->dv)  << std::endl;
		}
		Projection(fnew[j], j);
		df[j] = fnew[j] - f[j];
		CompResponse(df[j], j);
		f[j] = fnew[j];
	}
}

void PHConstraint::SetupCorrectionLCP(){
	if(!bEnabled || !bFeasible || bArticulated)
		return;
	//	拘束する自由度の決定
	//bool con[6];
	//SetConstrainedIndexCorrection(con);
	for(int i = 0; i < 6; i++){
		//if(con[i] && constrCorrection[i]){		// 継続して拘束される場合
			 F[i] *= engine->shrinkRateCorrection;
		//}else{
		//	F[i] = 0.0;							// 新規に拘束される or 拘束されない
		//}
		//constrCorrection[i] = con[i];
	}
	B.clear();
	CompError();
	
	// velocity updateによる影響を加算
	B += (J[0] * (solid[0]->v + solid[0]->dv)
			+ J[1] * (solid[1]->v + solid[1]->dv)) * GetScene()->GetTimeStep();
	B *= engine->posCorrectionRate;
		
	// 拘束力初期値による位置変化量を計算
	SpatialVector Fs;
	for(int i = 0; i < 2; i++){
		if(!solid[i]->IsDynamical() || !IsInactive(i))continue;
		if(solid[i]->IsArticulated()){
			(Vec6d&)Fs = J[i].trans() * F;
			solid[i]->treeNode->CompResponse(Fs, true, true);
		}
		else solid[i]->dV += T[i].trans() * F;
	}
}

void PHConstraint::IterateCorrectionLCP(){
	if(!bEnabled || !bFeasible || bArticulated)
		return;
	
	SpatialVector Fnew, dF, dFs;
	int i, j;
	for(j = 0; j < 6; j++){
		if(!constrCorrection[j]) continue;
		Fnew[j] = F[j] - Ainv[j] * (B[j] + J[0].row(j) * solid[0]->dV + J[1].row(j) * solid[1]->dV);
		ProjectionCorrection(Fnew[j], j);
		dF[j] = Fnew[j] - F[j];
		for(i = 0; i < 2; i++){
			if(!solid[i]->IsDynamical() || !IsInactive(i))continue;
			if(solid[i]->IsArticulated()){
				(Vec6d&)dFs = J[i].row(j) * dF[j];
				solid[i]->treeNode->CompResponse(dFs, true, true);			
			}
			else solid[i]->dV += T[i].row(j) * dF[j];
		}
		F[j] = Fnew[j];
	}
}

void PHConstraint::GetRelativeVelocity(Vec3d &v, Vec3d &w){
	for(int i = 0; i < 2; i++){
		solid[i]->UpdateCacheLCP(GetScene()->GetTimeStep());
	}
	UpdateState();
	v = vjrel.v(); 
	w = vjrel.w();
}

void PHConstraint::GetConstraintForce(Vec3d& _f, Vec3d& _t){
	_f = f.v() / GetScene()->GetTimeStep();
	_t = f.w() / GetScene()->GetTimeStep();
}

}