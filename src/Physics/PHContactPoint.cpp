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
	const PHMaterial& mat0 = sp->shape[0]->GetMaterial();
	const PHMaterial& mat1 = sp->shape[1]->GetMaterial();
	
	PHScene* s = DCAST(PHScene, s0->GetScene());
	switch(s->blendMode){
	case PHSceneDesc::BLEND_MIN:
		mu0 = std::min(mat0.mu0, mat1.mu0);
		mu  = std::min(mat0.mu , mat1.mu );
		e   = std::min(mat0.e  , mat1.e  );
		break;
	case PHSceneDesc::BLEND_MAX:
		mu0 = std::max(mat0.mu0, mat1.mu0);
		mu  = std::max(mat0.mu , mat1.mu );
		e   = std::max(mat0.e  , mat1.e  );
		break;
	case PHSceneDesc::BLEND_AVE_ADD:
		mu0 = 0.5 * (mat0.mu0 + mat1.mu0);
		mu  = 0.5 * (mat0.mu  + mat1.mu );
		e   = 0.5 * (mat0.e   + mat1.e  );
		break;
	case PHSceneDesc::BLEND_AVE_MUL:
		mu0 = sqrt(mat0.mu0 * mat1.mu0);
		mu  = sqrt(mat0.mu  * mat1.mu );
		e   = sqrt(mat0.e   * mat1.e  );
		break;
	}
	
	if(mat0.spring == 0.0f){
		if(mat1.spring == 0.0f)
			 spring = 0.0;
		else spring = mat1.spring;
	}
	else{
		if(mat1.spring == 0.0f)
			 spring = mat0.spring;
		else spring = (mat0.spring * mat1.spring) / (mat0.spring + mat1.spring);
	}
	
	if(mat0.damper == 0.0f){
		if(mat1.damper == 0.0f)
			 damper = 0.0;
		else damper = mat1.damper;
	}
	else{
		if(mat1.damper == 0.0f)
			 damper = mat0.damper;
		else damper = (mat0.damper * mat1.damper) / (mat0.damper + mat1.damper);
	}

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

	movableAxes.Enable(3);
	movableAxes.Enable(4);
	movableAxes.Enable(5);
}

void PHContactPoint::CompBias(){
	PHSceneIf* scene = GetScene();
	double dt    = scene->GetTimeStep();
	double dtinv = scene->GetTimeStepInv();
	double tol   = scene->GetContactTolerance();
	double vth   = scene->GetImpactThreshold();

	//	速度が小さい場合は、跳ね返りなし。
	if(vjrel[0] > - vth){
		double diff = std::max(shapePair->depth - tol, 0.0);
		// 粘弾性なし
		if(spring == 0.0 && damper == 0.0){
			db[0] = - engine->contactCorrectionRate * diff * dtinv;
		}
		// 粘弾性あり
		else{
			double tmp = 1.0 / (damper + spring * dt);
			dA[0] = tmp * dtinv;
			db[0] = - tmp * spring * diff;
		}
	}
	else{
		//	跳ね返るときは補正なし
		db[0] = e * vjrel[0];
	}
}

bool PHContactPoint::Projection(double& f_, int i) {
	PHConstraint::Projection(f_, i);
	if(i == 0){	
		// 接触深度が許容値以下なら反力を出さない
		if(shapePair->depth < GetScene()->GetContactTolerance()){
			f_ = fx = flim = 0.0;
			return true;
		}
		//垂直抗力 >= 0の制約
		if(f_ < 0.0){
			f_ = fx = flim = 0.0;
			return true;
		}
		// 垂直抗力
		fx = f_;
		// 最大静止摩擦力
		flim = mu0 * fx;

		return false;
	}
	else{
		double fu = mu * fx;
		//if (-0.01 < vjrel[1] && vjrel[1] < 0.01){	//	静止摩擦
		double vth = GetScene()->GetFrictionThreshold();
		if(-vth < vjrel[1] && vjrel[1] < vth){
			if (f_ > flim){
				f_ = flim;
				return true;
			}
			if (f_ < -flim){
				f_ = -flim;
				return true;
			}
			return false;
		}
		else{					//	動摩擦
			if (f_ > fu){
				f_ = fu;
				return true;
			}
			if (f_ < -fu){
				f_ = -fu;
				return true;
			}
			return false;
		}
	}
}

void PHContactPoint::CompError(){
	const double eps = 0.001;
	//衝突判定アルゴリズムの都合上、Correctionによって完全に剛体が離れてしまうのは困るので
	//誤差をepsだけ小さく見せる
	B[0] = min(0.0, -shapePair->depth + eps) * engine->posCorrectionRate;
}

void PHContactPoint::ProjectionCorrection(double& F, int k){
	if(k == 0){	//垂直抗力 >= 0の制約
		F = max((double)0.0, F);
	}
	else if(k == 1 || k == 2)
		F = 0;
}

}
