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
namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHConstraint

// コンストラクタ
PHConstraint::PHConstraint() {
	solid[0] = solid[1] = NULL;

	f.clear();
	F.clear();

	bEnabled = true;
	bInactive[0] = true;
	bInactive[1] = true;
	bArticulated = false;
	bProhibitUpdateSolidCacheLCP = false;

	for(int i=0; i<6; i++){
		fMaxDt[i] =  FLT_MAX;
		fMinDt[i] = -FLT_MAX;
	}
}

void PHConstraint::InitTargetAxes() {
	// movableAxesの残りでtargetAxesを作る．
	nTargetAxes = 0;
	for (int i=0; i<6; ++i) {
		bool bMovable = false;
		for (int n=0; n<nMovableAxes; ++n) { if (i==movableAxes[n]) { bMovable = true; }  }
		if (bMovable) { continue; }
		targetAxes[nTargetAxes++] = i;
	}
}

// ----- エンジンから呼び出される関数

void PHConstraint::UpdateState() {
	// 剛体の状態を更新する（ここでやるべきかは要検討！少なくともここ以前にUpdateされてないと正しい値が出ない <!!>）
	if (!bProhibitUpdateSolidCacheLCP) {
		for (int i=0; i<2; i++) { solid[i]->UpdateCacheLCP(GetScene()->GetTimeStep()); }
	}

	// 剛体の相対位置からヤコビアン，関節速度・位置を逆算する
	CompJacobian();

	// 親剛体の中心から見た速度と，子剛体の中心から見た速度を
	// Socket座標系から見た速度に両方直して，相対速度を取る．
	vjrel = Js[1] * solid[1]->v - Js[0] * solid[0]->v;

	// 関節座標の位置・速度を更新する
	UpdateJointState();
}

void PHConstraint::SetupLCP() {
	bProhibitUpdateSolidCacheLCP = true;

	// 実現可能な拘束であるか
	bFeasible = solid[0]->IsDynamical() || solid[1]->IsDynamical();
	if(!bEnabled || !bFeasible) { return; }
	
	// 拘束軸フラグのクリア <<ここからaxes.CreateList()までaxes[n]は使えない．Enable/Disable/IsEnabledのみ>>
	axes.Clear();

	// Projection用の最大・最小値をリセットする
	for (int i=0; i<6; i++) { fMinDt[i] = -FLT_MAX; fMaxDt[i] =  FLT_MAX; }

	// 拘束する自由度の決定
	SetupAxisIndex();

	// LCPの座標の取り方が特殊な関節はヤコビアンに座標変換をかける
	ModifyJacobian();

	// LCPの係数A, bの補正値dA, dbを計算
	dA.clear();
	db.clear();
	CompBias();

	// LCPのA行列の対角成分を計算
	CompResponseMatrix();

	// LCPのbベクトル == 論文中のw[t]を計算
	b = J[0] * solid[0]->v + J[1] * solid[1]->v;  //vjrelではない 値によっては変わって拘束しなくなる

	// ここまでで決定された拘束軸フラグを使って軸番号リストを作成　<<ここからはaxes[n]を使用可能>>
	axes.CreateList();

	// 拘束力の初期値を更新
	//   拘束力は前回の値を縮小したものを初期値とする．
	//   前回の値そのままを初期値にすると，拘束力が次第に増大するという現象が生じる．
	//   これは，LCPを有限回（実際には10回程度）の反復で打ち切るためだと思われる．
	//   0ベクトルを初期値に用いても良いが，この場合比較的多くの反復回数を要する．
	for (int n=0; n<axes.size(); ++n) {
		f[axes[n]] *= axes.IsContinued(axes[n]) ? engine->shrinkRate : 0;
	}

	// 拘束力初期値による速度変化量を計算
	SpatialVector fs;
	for (int i=0; i<2; ++i) {
		if (!solid[i]->IsDynamical() || !IsInactive(i)) { continue; }
		if (solid[i]->IsArticulated()) {
			(Vec6d&)fs = J[i].trans() * f;
			solid[i]->treeNode->CompResponse(fs, true, false);
		} else {
			solid[i]->dv += T[i].trans() * f;
		}
	}
}

void PHConstraint::IterateLCP() {
	if (!bEnabled || !bFeasible) { return; }
	
	SpatialVector fnew, df;

	for (int n=0; n<axes.size(); ++n) {
		// Gauss-Seidel Iteration
		int i = axes[n];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * (dA[i]*f[i] + b[i] + db[i] + J[0].row(i)*solid[0]->dv + J[1].row(i)*solid[1]->dv);

		// Projection
		Projection(fnew[i], i);

		// Comp Response & Update f
		df[i] = fnew[i] - f[i];
		CompResponse(df[i], i);
		f[i] = fnew[i];
	}
}

void PHConstraint::SetupCorrectionLCP() {
	if(!bEnabled || !bFeasible || bArticulated) { return; }

	for (int n=0; n<axes.size(); ++n) {
		F[axes[n]] *= engine->shrinkRateCorrection;
	}
	B.clear();
	CompError();
	
	// velocity updateによる影響を加算
	B += (J[0] * (solid[0]->v + solid[0]->dv) + J[1] * (solid[1]->v + solid[1]->dv)) * GetScene()->GetTimeStep();
	B *= engine->posCorrectionRate;
		
	// 拘束力初期値による位置変化量を計算
	SpatialVector Fs;
	for(int i = 0; i < 2; i++){
		if (!solid[i]->IsDynamical() || !IsInactive(i)) { continue; }
		if (solid[i]->IsArticulated()) {
			(Vec6d&)Fs = J[i].trans() * F;
			solid[i]->treeNode->CompResponse(Fs, true, true);
		} else {
			solid[i]->dV += T[i].trans() * F;
		}
	}
}

void PHConstraint::IterateCorrectionLCP() {
	if(!bEnabled || !bFeasible || bArticulated) { return; }
	
	SpatialVector Fnew, dF, dFs;

	for (int n=0; n<axes.size(); ++n) {
		int k = axes[n];
		Fnew[k] = F[k] - Ainv[k] * (B[k] + J[0].row(k) * solid[0]->dV + J[1].row(k) * solid[1]->dV);
		ProjectionCorrection(Fnew[k], k);
		dF[k] = Fnew[k] - F[k];
		for (int i=0; i<2; i++) {
			if(!solid[i]->IsDynamical() || !IsInactive(i))continue;
			if(solid[i]->IsArticulated()){
				(Vec6d&)dFs = J[i].row(k) * dF[k];
				solid[i]->treeNode->CompResponse(dFs, true, true);			
			} else {
				solid[i]->dV += T[i].row(k) * dF[k];
			}
		}
		F[k] = Fnew[k];
	}
}

// ----- このクラスで実装する機能

void PHConstraint::CompJacobian() {
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
	J[0] = Js[0];
	J[0] *= -1.0;	//反作用
	J[1] = Js[1];
}

void PHConstraint::CompResponseMatrix() {
	// Aの対角成分を計算する．A = J * M^-1 * J^T
	// A行列は拘束力から速度変化への影響の強さを表す行列なので，
	// その対角成分はある拘束力成分から自分自身の拘束速度成分への影響の強さを表す

	A.clear();
	T[0].clear();
	T[1].clear();

	PHRootNode* root[2] = {
		solid[0]->IsArticulated() ? solid[0]->treeNode->GetRootNode() : NULL,
		solid[1]->IsArticulated() ? solid[1]->treeNode->GetRootNode() : NULL,
	};

	SpatialVector df;
	for (int i=0; i<2; ++i) {
		if (solid[i]->IsDynamical()) {
			if (solid[i]->IsArticulated()) {
				// -- ABA --
				for (int j=0; j<6; ++j) {
					// ABAにdf = J^Tを入力することでM^-1 J^Tを計算し，結果にJをかけてAを得る
					(Vec6d&)df = J[i].row(j);
					solid[i]->treeNode->CompResponse(df, false, false);
					A[j] += J[i].row(j) * solid[i]->treeNode->da;
					// もう片方の剛体も同一のツリーに属する場合はその影響項も加算
					if(solid[!i]->IsArticulated() && root[i] == root[!i]) {
						A[j] += J[!i].row(j) * solid[!i]->treeNode->da;
					}
				}
			} else {
				// -- LCP --
				// T = M^-1 * J^T
				T[i].vv() = J[i].vv() * solid[i]->minv;
				T[i].vw() = J[i].vw() * solid[i]->Iinv;
				T[i].wv() = J[i].wv() * solid[i]->minv;
				T[i].ww() = J[i].ww() * solid[i]->Iinv;

				// A == 論文中のJ * M^-1 * J^T, Gauss Seidel法のD
				for(int j=0; j<6; ++j) {
					A[j] += J[i].row(j) * T[i].row(j);
				}
			}
		}
	}

	// 最大の対角要素との比がepsよりも小さい対角要素がある場合，
	// 数値的不安定性の原因となるのでその成分は拘束対象から除外する
	// ＊epsを大きくとると，必要な拘束まで無効化されてしまうので、調整は慎重に。
	const double eps = 1.0e-6, epsabs = 1.0e-10;
	double Amax=0.0, Amin;
	for(int i=0; i<6; ++i) {
		if(axes.IsEnabled(i) && A[i] > Amax) { Amax = A[i]; }
	}
	Amin = Amax * eps;

	for(int i=0; i<6; ++i) {
		if (axes.IsEnabled(i)) {
			if(A[i] < Amin || A[i] < epsabs){
				axes.Disable(i);
				DSTR << this->GetName() << ": Axis " << i << " ill-conditioned! Disabled.  A[" << i << "]= " << A[i] << endl;
			} else {
				Ainv[i] = 1.0 / (A[i] + dA[i]);
			}
		}
	}
}

void PHConstraint::CompResponse(double df, int i) {
	SpatialVector dfs;
	for (int k=0; k<2; ++k) {
		if (!solid[k]->IsDynamical() || !IsInactive(k)) { continue; }
		if (solid[k]->IsArticulated()) {
			(Vec6d&)dfs = J[k].row(i) * df;
			solid[k]->treeNode->CompResponse(dfs, true, false);
		} else {
			solid[k]->dv += T[k].row(i) * df;
		}
	}
}

// ----- 本来は派生クラスで実装する機能の，デフォルト動作

void PHConstraint::SetupAxisIndex() {
	if (!bArticulated) {
		for (int n=0; n<nTargetAxes; ++n) {
			axes.Enable(targetAxes[n]);
		}
	}
}

/// 拘束力の射影
void PHConstraint::Projection(double& f_, int i) {
	f_ = min(max(fMinDt[i], f_), fMaxDt[i]);
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
	_f = f.v() / GetScene()->GetTimeStep();
	_t = f.w() / GetScene()->GetTimeStep();
}

Vec3d PHConstraint::GetForce() {
	return( f.v() / GetScene()->GetTimeStep() );
}

Vec3d PHConstraint::GetTorque() {
	return( f.w() / GetScene()->GetTimeStep() );
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
	return solid[i]->Cast();
}

void PHConstraint::AfterSetDesc() {
	SceneObject::AfterSetDesc();
}

}
