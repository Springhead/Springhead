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
	const PHMaterial* mat[2];
	mat[0] = &sp->shape[0]->GetMaterial();
	mat[1] = &sp->shape[1]->GetMaterial();
	
	PHScene* s = DCAST(PHScene, s0->GetScene());
	switch(s->blendMode){
	case PHSceneDesc::BLEND_MIN:
		mu0 = std::min(mat[0]->mu0, mat[1]->mu0);
		mu  = std::min(mat[0]->mu , mat[1]->mu );
		e   = std::min(mat[0]->e  , mat[1]->e  );
		break;
	case PHSceneDesc::BLEND_MAX:
		mu0 = std::max(mat[0]->mu0, mat[1]->mu0);
		mu  = std::max(mat[0]->mu , mat[1]->mu );
		e   = std::max(mat[0]->e  , mat[1]->e  );
		break;
	case PHSceneDesc::BLEND_AVE_ADD:
		mu0 = 0.5 * (mat[0]->mu0 + mat[1]->mu0);
		mu  = 0.5 * (mat[0]->mu  + mat[1]->mu );
		e   = 0.5 * (mat[0]->e   + mat[1]->e  );
		break;
	case PHSceneDesc::BLEND_AVE_MUL:
		mu0 = sqrt(mat[0]->mu0 * mat[1]->mu0);
		mu  = sqrt(mat[0]->mu  * mat[1]->mu );
		e   = sqrt(mat[0]->e   * mat[1]->e  );
		break;
	}
	
	if(mat[0]->spring == 0.0f){
		if(mat[1]->spring == 0.0f)
			 spring = 0.0;
		else spring = mat[1]->spring;
	}
	else{
		if(mat[1]->spring == 0.0f)
			 spring = mat[0]->spring;
		else spring = (mat[0]->spring * mat[1]->spring) / (mat[0]->spring + mat[1]->spring);
	}
	
	if(mat[0]->damper == 0.0f){
		if(mat[1]->damper == 0.0f)
			 damper = 0.0;
		else damper = mat[1]->damper;
	}
	else{
		if(mat[1]->damper == 0.0f)
			 damper = mat[0]->damper;
		else damper = (mat[0]->damper * mat[1]->damper) / (mat[0]->damper + mat[1]->damper);
	}

	Posed  poseSolid[2];
	Posed  poseRel  [2];
	Vec3d  vc   [2];
	Vec3d  vcabs[2];
	
	pose.Pos() = p;
	pose.Ori().FromMatrix(local);
	
	solid[0] = s0;
	solid[1] = s1;

	for(int i = 0; i < 2; i++){
		poseSolid[i].Pos() = solid[i]->GetCenterPosition();
		poseSolid[i].Ori() = solid[i]->GetOrientation   ();
		poseRel  [i] = sp->frame[i]->pose_abs.Inv() * pose;

		// 接触点での速度場
		Vec3d normal = (i == 0 ? 1.0 : -1.0) * poseRel[i].Ori() * Vec3d(1.0, 0.0, 0.0);
		vc   [i] = mat[i]->CalcVelocity(poseRel[i].Pos(), normal);
		vcabs[i] = poseRel[i].Ori().Conjugated() * vc[i];
	
		// local: 接触点の関節フレーム は，x軸を法線, y,z軸を接線とする
		(i == 0 ? poseSocket : posePlug).Ori() = Xj[i].q = poseSolid[i].Ori().Conjugated() * pose.Ori();
		(i == 0 ? poseSocket : posePlug).Pos() = Xj[i].r = poseSolid[i].Ori().Conjugated() * (pose.Pos() - poseSolid[i].Pos());
	}

	// relative velocity of contact motor in local coord
	velField = vcabs[1] - vcabs[0];
	
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
			double tmp = (shapePair->depth > tol) ? (1.0 / (damper + spring * dt)) : (1.0 / damper);
			dA[0] = tmp * dtinv;
			db[0] = - tmp * spring * diff;
		}
	}
	else{
		//	跳ね返るときは補正なし
		db[0] = e * vjrel[0];
	}

	db[1] = velField[1];
	db[2] = velField[2];

	// determine static/dynamic friction based on tangential relative velocity
	double vt = vjrel[1] + velField[1];
	isStatic = (-vth < vt && vt < vth);
}

bool PHContactPoint::Projection(double& f_, int i) {
	PHConstraint::Projection(f_, i);
	if(i == 0){	
		//垂直抗力 >= 0の制約
		if(f_ < 0.0){
			f_ = fx = flim = 0.0;
			return true;
		}
		// 垂直抗力
		fx = f_;
		// 最大静止摩擦力
		flim0 = mu0 * fx;
		flim  = mu  * fx;

		return false;
	}
	else{
		// 静止摩擦
		if(isStatic){
			if (f_ > flim0){
				f_ = flim0;
				return true;
			}
			if (f_ < -flim0){
				f_ = -flim0;
				return true;
			}
			return false;
		}
		// 動摩擦
		else{
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
	}
}

void PHContactPoint::CompError(){
	PHSceneIf* scene = GetScene();
	
	//衝突判定アルゴリズムの都合上、Correctionによって完全に剛体が離れてしまうのは困るので
	//誤差をepsだけ小さく見せる
	double tol  = scene->GetContactTolerance();
	double diff = std::max(shapePair->depth - tol, 0.0);
	B[0] = -diff;
}

bool PHContactPoint::ProjectionCorrection(double& F_, int i){
	if(i == 0){	//垂直抗力 >= 0の制約
		F_ = max((double)0.0, F_);
	}
	else if(i == 1 || i == 2){
		//F = 0;
	}
	return false;
}

}
