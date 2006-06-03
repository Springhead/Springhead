#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHContactPoint
//OBJECTIMP(PHContactPoint, PHConstraint);
IF_IMP(PHContactPoint, PHConstraint);

PHContactPoint::PHContactPoint(const Matrix3d& local, CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1){
	shapePair = sp;
	pos = p;
	solid[0] = s0, solid[1] = s1;

	Vec3d rjabs[2];
	for(int i = 0; i < 2; i++){
		rjabs[i] = pos - solid[i]->solid->GetCenterPosition();	//剛体の中心から接触点までのベクトル
	}
	// local: 接触点の関節フレーム は，x軸を法線, y,z軸を接線とする
	for(int i = 0; i < 2; i++){
		Rj[i] = solid[i]->solid->GetRotation().trans() * local;
		rj[i] = solid[i]->solid->GetRotation().trans() * rjabs[i];
	}
}

PHContactPoint::PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1){
	shapePair = sp;
	pos = p;
	solid[0] = s0, solid[1] = s1;

	Vec3d rjabs[2], vjabs[2];
	for(int i = 0; i < 2; i++){
		rjabs[i] = pos - solid[i]->solid->GetCenterPosition();	//剛体の中心から接触点までのベクトル
		vjabs[i] = solid[i]->solid->GetVelocity() + solid[i]->solid->GetAngularVelocity() % rjabs[i];	//接触点での速度
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
	
	for(int i = 0; i < 2; i++){
		Rj[i] = solid[i]->solid->GetRotation().trans() * Rjabs;
		rj[i] = solid[i]->solid->GetRotation().trans() * rjabs[i];
	}
}

void PHContactPoint::CompConstraintJacobian(){
	dim_d = 3;
	dim_c = 1;
	
	Ad.clear();
	for(int i = 0; i < 2; i++){
		Jdv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jdw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		Jcv[i].row(0) = Jdv[i].row(0);
		Jcw[i].row(0) = Jdw[i].row(0);
		if(solid[i]->solid->IsDynamical()){
			Tdv[i].SUBMAT(0, 0, 3, 3) = Jdv[i].SUBMAT(0, 0, 3, 3) * solid[i]->minv;
			Tdw[i].SUBMAT(0, 0, 3, 3) = Jdw[i].SUBMAT(0, 0, 3, 3) * solid[i]->Iinv;
			Tcv[i].row(0) = Tdv[i].row(0);
			Tcw[i].row(0) = Tdw[i].row(0);
			for(int j = 0; j < 3; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
		}
	}
	Ac[0] = Ad[0];
}

void PHContactPoint::ProjectionDynamics(double& f, int k){
	static double flim;
	if(k == 0){	//垂直抗力 >= 0の制約
		f = max(0.0, f);
		flim = 0.5 * (shapePair->shape[0]->shape->material.mu0 + shapePair->shape[1]->shape->material.mu0) * f;	//最大静止摩擦
	}
	else{
		//|摩擦力| <= 最大静止摩擦の制約
		//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
		//	・静止摩擦と動摩擦が同じ値でないと扱えない。
		//摩擦係数は両者の静止摩擦係数の平均とする
		f = min(max(-flim, f), flim);
	}
}

void PHContactPoint::CompError(double dt){
	const double eps = 0.0;
	//衝突判定アルゴリズムの都合上、Correctionによって完全に剛体が離れてしまうのは困るので
	//誤差をepsだけ小さく見せる
	B[0] = min(0.0, -shapePair->depth + eps);
}

void PHContactPoint::ProjectionCorrection(double& F, int k){
	//垂直抗力 >= 0の制約
	F = Spr::max(0.0, F);
}

}
