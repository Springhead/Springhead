/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHContactPoint.h>
#include <Physics/PHConstraintEngine.h>

#include <iomanip>

using namespace PTM;
using namespace std;
namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHContactPoint

PHContactPoint::PHContactPoint(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1){
	shapePair = sp;
	mu0 = 0.5 * (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0);
	mu  = 0.5 * (shapePair->shape[0]->GetMaterial().mu  + shapePair->shape[1]->GetMaterial().mu );
	e   = 0.5 * (shapePair->shape[0]->GetMaterial().e   + shapePair->shape[1]->GetMaterial().e  );
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
		(i == 0 ? poseSocket : posePlug).Ori() = Xj[i].q = solid[i]->GetOrientation().Conjugated() * qlocal;
		(i == 0 ? poseSocket : posePlug).Pos() = Xj[i].r = solid[i]->GetOrientation().Conjugated() * rjabs[i];
	}

	nMovableAxes   = 3;
	movableAxes[0] = 3;
	movableAxes[1] = 4;
	movableAxes[2] = 5;
	InitTargetAxes();	
}
/*
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
	t[1] = n % t[0];
	Matrix3d Rjabs;
	// 接触点の関節フレームはx軸, y軸を接線，z軸を法線とする
	Rjabs.col(0) = n;
	Rjabs.col(1) = t[0];
	Rjabs.col(2) = t[1];
	Quaterniond qjabs;
	qjabs.FromMatrix(Rjabs);
	for(int i = 0; i < 2; i++){
		(i == 0 ? poseSocket : posePlug).Ori() = Xj[i].q = solid[i]->GetOrientation().Conjugated() * qjabs;
		(i == 0 ? poseSocket : posePlug).Pos() = Xj[i].r = solid[i]->GetOrientation().Conjugated() * rjabs[i];
	}

	nMovableAxes   = 3;
	movableAxes[0] = 3;
	movableAxes[1] = 4;
	movableAxes[2] = 5;
	InitTargetAxes();
}
*/

void PHContactPoint::CompBias(){
	//	correctionを位置LCPで別に行う場合は、速度を変更しての位置補正はしない。
	if (engine->numIterContactCorrection) return;
	PHSceneIf* scene = GetScene();
	double dt    = scene->GetTimeStep();
	double dtinv = scene->GetTimeStepInv();
	double tol   = scene->GetContactTolerance();
	double vth   = scene->GetImpactThreshold();
#if 1
	//	速度が小さい場合は、跳ね返りなし。
	//if (vjrel[0] * dt > -0.1){
	if(vjrel[0] > - vth){
		db[0] = - engine->contactCorrectionRate * std::max(shapePair->depth - tol, 0.0) * dtinv;
	}
	else{
		//	跳ね返るときは補正なし
		db[0] = e * vjrel[0];
	}

#endif

#if 0
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
	double err = (shapePair->depth - 1e-3)*dtinv - 0.2*vjrel.v().x;
	if (err < 0) err = 0;
	if (err){
//		HASE_REPORT
//		DSTR << shapePair->state;
//		DSTR << shapePair->shapePoseW[1].Pos();
//		DSTR << "err: " << err << std::edl;
		db.v().x = -err * engine->velCorrectionRate;
	}
#endif

#if 0
	double tmp = 1.0 / (correctionDamper + correctionSpring * GetScene()->GetTimeStep());
	dA[0] = tmp * dtinv;
	db[0] = -correctionSpring * max(0.0, shapePair->depth - overlap) * tmp;
#endif
}

void PHContactPoint::Projection(double& f_, int i) {
	static double fx, flim;
	if(i == 0){	
		//垂直抗力 >= 0の制約
		f_ = max(0.0, f_);
		// 垂直抗力
		fx   = f_;
		// 最大静止摩擦力
		flim = mu0 * fx;
	}
	else{
		//	動摩擦を試しに実装してみる。
		double fu = mu * fx;
		/*if (mu0 == 0.0){
			fu = 0;
		}
		else{
			fu = (shapePair->shape[0]->GetMaterial().mu + shapePair->shape[1]->GetMaterial().mu)
				/ (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0)
				* flim;	
		}*/
		//if (-0.01 < vjrel[1] && vjrel[1] < 0.01){	//	静止摩擦
		double vth = GetScene()->GetFrictionThreshold();
		if(-vth < vjrel[1] && vjrel[1] < vth){
			if (f_ > flim) f_ = fu;
			else if (f_ < -flim) f_ = -fu;
		}
		else{					//	動摩擦
			if (f_ > fu) f_ = fu;
			else if (f_ < -fu) f_ = -fu;		
		}
#if 0
		//|摩擦力| <= 最大静止摩擦の制約
		//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
		//	・静止摩擦と動摩擦が同じ値でないと扱えない。
		//摩擦係数は両者の静止摩擦係数の平均とする
		f_ = min(max(-flim, f_), flim);
#endif
		assert(f_ < 10000);
	}
}

void PHContactPoint::CompError(){
	const double eps = 0.001;
	//衝突判定アルゴリズムの都合上、Correctionによって完全に剛体が離れてしまうのは困るので
	//誤差をepsだけ小さく見せる
	B.v().x = min(0.0, -shapePair->depth + eps);

	// 誤差を全部解消すると振動するので、少なめに
	B.v().x *= engine->posCorrectionRate;
}

void PHContactPoint::ProjectionCorrection(double& F, int k){
	if(k == 0){	//垂直抗力 >= 0の制約
		F = max(0.0, F);
	}
	else if(k == 1 || k == 2)
		F = 0;
}

}
