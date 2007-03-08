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
// PHContactPoint
IF_OBJECT_IMP(PHContactPoint, PHConstraint);

PHContactPoint::PHContactPoint(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1){
	shapePair = sp;
	pos = p;
	solid[0] = s0, solid[1] = s1;

	Vec3d rjabs[2];
	for(int i = 0; i < 2; i++){
		rjabs[i] = pos - solid[i]->GetCenterPosition();	//剛体の中心から接触点までのベクトル
	}
	// local: 接触点の関節フレーム は，x軸を法線, y,z軸を接線とする
	Quaterniond qlocal;
	qlocal.FromMatrix(local);
	for(int i = 0; i < 2; i++){
		Xj[i].q = solid[i]->GetOrientation().Conjugated() * qlocal;
		Xj[i].r = solid[i]->GetOrientation().Conjugated() * rjabs[i];
	}
}

PHContactPoint::PHContactPoint(PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1){
	shapePair = sp;
	pos = p;
	solid[0] = s0, solid[1] = s1;

	Vec3d rjabs[2], vjabs[2];
	for(int i = 0; i < 2; i++){
		rjabs[i] = pos - solid[i]->GetCenterPosition();	//剛体の中心から接触点までのベクトル
		vjabs[i] = solid[i]->GetVelocity() + solid[i]->GetAngularVelocity() % rjabs[i];	//接触点での速度
	}
	//接線ベクトルt[0], t[1] (t[0]は相対速度ベクトルに平行になるようにする)
	Vec3d n, t[2], vjrel, vjrelproj;
	n = shapePair->normal;
	vjrel = vjabs[1] - vjabs[0];
	vjrelproj = vjrel - (n * vjrel) * n;		//相対速度ベクトルを法線に直交する平面に射影したベクトル
	double vjrelproj_norm = vjrelproj.norm();
	if(vjrelproj_norm < 1.0e-10){
		t[0] = n % Vec3d(1.0, 0.0, 0.0);	
		if(t[0].norm() < 1.0e-10)
			t[0] = n % Vec3d(0.0, 1.0, 0.0);
		t[0].unitize();
	}
	else{
		t[0] = vjrelproj / vjrelproj_norm;
	}
	t[1] = t[0] % n;
	Matrix3d Rjabs;
	// 接触点の関節フレームはx軸, y軸を接線，z軸を法線とする
	Rjabs.col(0) = n;
	Rjabs.col(1) = t[0];
	Rjabs.col(2) = t[1];
	Quaterniond qjabs;
	qjabs.FromMatrix(Rjabs);
	for(int i = 0; i < 2; i++){
		Xj[i].q = solid[i]->GetOrientation().Conjugated() * qjabs;
		Xj[i].r = solid[i]->GetOrientation().Conjugated() * rjabs[i];
	}
}

void PHContactPoint::SetConstrainedIndex(bool* con){
	con[0] = con[1] = con[2] = true;
	con[3] = con[4] = con[5] = false;
}
void PHContactPoint::CompBias(){
	double dtinv = 1.0 / scene->GetTimeStep();
//	db.v.x = 0.1*engine->correctionRate * (-shapePair->depth * dtinv + vjrel.v.x);
	/*	hase	本当は 1e-3は引きすぎ
		depth 分だけCorrectionを入れると接触が不連続になるので，depth-epsilonで良いが，
		epsilonが大きすぎる．

		これは，接触面が四角形で接触点が4点ある場合など，
		4点間の力の分散がまれに不均一になり回転が始まり，
		接触面が右よりの状態と左よりの状態の間で細かく発振することがあるため．
		現状は，その場合にも接触面の形が変わらないようにすることで，安定化している．

		2剛体間の接触をひとつの制約としてあらわせるようになれば解決すると思う．	
	*/
#if 0
	double err = (shapePair->depth - 1e-3)*dtinv - 0.2*vjrel.v().x;
	if (err < 0) err = 0;
	if (err){
//		HASE_REPORT
//		DSTR << shapePair->state;
//		DSTR << shapePair->shapePoseW[1].Pos();
//		DSTR << "err: " << err << std::edl;
		db.v().x = -err * engine->correctionRate;
	}
#else
	const double damper = 50.0, spring = 0.0;
	double tmp = 1.0 / (damper + spring * scene->GetTimeStep());
	dA[0] = tmp * dtinv;
	db[0] = -spring * (shapePair->depth - 1e-3) * tmp;
#endif
}

void PHContactPoint::Projection(double& f, int k){
	static double flim;
	if(k == 0){	//垂直抗力 >= 0の制約
		f = max(0.0, f);
		flim = 0.5 * (shapePair->shape[0]->material.mu0 + shapePair->shape[1]->material.mu0) * f;	//最大静止摩擦
	}
	else{
		//|摩擦力| <= 最大静止摩擦の制約
		//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
		//	・静止摩擦と動摩擦が同じ値でないと扱えない。
		//摩擦係数は両者の静止摩擦係数の平均とする
		f = min(max(-flim, f), flim);
	}
}

void PHContactPoint::CompError(){
	const double eps = 0.0;
	//衝突判定アルゴリズムの都合上、Correctionによって完全に剛体が離れてしまうのは困るので
	//誤差をepsだけ小さく見せる
	B.v().x = min(0.0, -shapePair->depth + eps);
}

void PHContactPoint::ProjectionCorrection(double& F, int k){
	if(k == 0){	//垂直抗力 >= 0の制約
		F = max(0.0, F);
	}
	else if(k == 1 || k == 2)
		F = 0;
}

}
