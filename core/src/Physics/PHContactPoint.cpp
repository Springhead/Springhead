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

PHContactPoint::PHContactPoint(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1) {
	shapePair = sp;
	const PHMaterial* mat[2];
	mat[0] = &sp->shape[0]->GetMaterial();
	mat[1] = &sp->shape[1]->GetMaterial();

	PHScene* s = DCAST(PHScene, s0->GetScene());

	Posed  poseSolid[2];

	pose.Pos() = p;
	pose.Ori().FromMatrix(local);

	solid[0] = s0;
	solid[1] = s1;

	for (int i = 0; i < 2; i++) {
		poseSolid[i].Pos() = solid[i]->GetFramePosition();
		poseSolid[i].Ori() = solid[i]->GetOrientation();
		// local: 接触点の関節フレーム は，x軸を法線, y,z軸を接線とする
		(i == 0 ? poseSocket : posePlug).Ori() = Xj[i].q = poseSolid[i].Ori().Conjugated() * pose.Ori();
		(i == 0 ? poseSocket : posePlug).Pos() = Xj[i].r = poseSolid[i].Ori().Conjugated() * (pose.Pos() - poseSolid[i].Pos());
	}

	if (rotationFriction == 0.0f) {
		movableAxes.Enable(3);
	}
	movableAxes.Enable(4);
	movableAxes.Enable(5);

	double dt = s->GetTimeStep();
	CDShapePairState st;
	sp->GetSt(st);
	unsigned int contactDuration = st.contactDuration;

	if (mat[0]->frictionModel == 1 || mat[1]->frictionModel == 1) {
		frictionModel = 1;

		PHLuGreSt lgs = sp->LuGreState;

		// LuGre model parameters
		sigma0 = std::max(mat[0]->bristlesSpringK, mat[1]->bristlesSpringK);
		sigma1 = std::max(mat[0]->bristlesDamperD, mat[1]->bristlesDamperD);
		sigma2 = std::max(mat[0]->bristlesViscosityV, mat[1]->bristlesViscosityV);
		timeVaryA = std::max(mat[0]->timeVaryFrictionA, mat[1]->timeVaryFrictionA);
		timeVaryB = std::max(mat[0]->timeVaryFrictionB, mat[1]->timeVaryFrictionB);
		timeVaryC = std::max(mat[0]->timeVaryFrictionC, mat[1]->timeVaryFrictionC);

		if (contactDuration == 0) {
			// Initialize LuGre state
			//printf("initialize lugre state\n");
			lgs.T = 0.0;
			lgs.z = Vec2d::Zero();
			lgs.rot = Matrix2d::Unit();
		}
		else {
			// Align z-vector with relative velocity rotation in local plane
			TMatrixCol<3, 2, double> Q; // Q = (u, v) (3x2 matrix)    L = (n, u, v)
			Q.col(0) = local.Ey().unit(); // u
			Q.col(1) = local.Ez().unit(); // v
			// 2D rotation matrix R = Q^T * L_n * L_{n-1}^T * Q
			Matrix2d r2d = Q.trans() * local * lgs.local_p.trans() * Q;
			//lgs.z = r2d.trans() * lgs.z;  // Rotate z
			lgs.rot = r2d * lgs.rot;  // Rotate local coordinate
		}
		lgs.local_p = local;
		sp->LuGreState = lgs;
	}
	else {
		frictionModel = COULOMB;
	}
	switch (s->blendMode) {
	case PHSceneDesc::BLEND_MIN:
		mu0 = std::min(mat[0]->mu0, mat[1]->mu0);
		mu = std::min(mat[0]->mu, mat[1]->mu);
		e = std::min(mat[0]->e, mat[1]->e);
		rotationFriction = std::min(mat[0]->rotationFriction, mat[1]->rotationFriction);
		break;
	case PHSceneDesc::BLEND_MAX:
		mu0 = std::max(mat[0]->mu0, mat[1]->mu0);
		mu = std::max(mat[0]->mu, mat[1]->mu);
		e = std::max(mat[0]->e, mat[1]->e);
		rotationFriction = std::max(mat[0]->rotationFriction, mat[1]->rotationFriction);
		break;
	case PHSceneDesc::BLEND_AVE_ADD:
		mu0 = 0.5 * (mat[0]->mu0 + mat[1]->mu0);
		mu = 0.5 * (mat[0]->mu + mat[1]->mu);
		e = 0.5 * (mat[0]->e + mat[1]->e);
		rotationFriction = 0.5 * (mat[0]->rotationFriction + mat[1]->rotationFriction);
		break;
	case PHSceneDesc::BLEND_AVE_MUL:
		mu0 = sqrt(mat[0]->mu0 * mat[1]->mu0);
		mu = sqrt(mat[0]->mu * mat[1]->mu);
		e = sqrt(mat[0]->e * mat[1]->e);
		rotationFriction = sqrt(mat[0]->rotationFriction * mat[1]->rotationFriction);
		break;
	}

	if (mat[0]->spring == 0.0f) {
		if (mat[1]->spring == 0.0f)
			spring = 0.0;
		else spring = mat[1]->spring;
	}
	else {
		if (mat[1]->spring == 0.0f)
			spring = mat[0]->spring;
		else spring = (mat[0]->spring * mat[1]->spring) / (mat[0]->spring + mat[1]->spring);
	}

	if (mat[0]->damper == 0.0f) {
		if (mat[1]->damper == 0.0f)
			damper = 0.0;
		else damper = mat[1]->damper;
	}
	else {
		if (mat[1]->damper == 0.0f)
			damper = mat[0]->damper;
		else damper = (mat[0]->damper * mat[1]->damper) / (mat[0]->damper + mat[1]->damper);
	}

}

void PHContactPoint::CompLuGreState() {
	PHSceneIf* scene = GetScene();
	double dt = scene->GetTimeStep();
	if (frictionModel == 1) {
		PHLuGreSt lgs = shapePair->LuGreState;
		// Get relative velocity
		v = lgs.rot * Vec2d(vjrel[1], vjrel[2]);
		//v.x = (fabs(v.x) < fth) ? 0.0f : v.x;
		//v.y = (fabs(v.y) < fth) ? 0.0f : v.y;
		lgs.v = v;
		double g = timeVaryA + timeVaryB * log(timeVaryC * lgs.T + 1);
		// T
		double T = lgs.T + dt;	// T <= T + dt
		//double T_ = lgs.z.norm() / (v.norm() + 1.0e-12);	// z_ss / v = g(T)/(σ_0|v|)
		double T_ = g / (sigma0 * v.norm() + 1.0e-12);
		//lgs.T = std::min(T, T_);	// T <= min(T, T_)
		if (T < T_) {
			lgs.T = T;
			isSticking = false;
		}
		else {
			lgs.T = T_;
			isSticking = true;
		}
		stickT = lgs.T;

		// z
#if 0
	//dz = v - (sigma0 * v.norm()) / g * lgs.z;
		dz = Vec2d(v.x - (sigma0 * fabs(v.x)) / g * lgs.z.x, v.y - (sigma0 * fabs(v.y)) / g * lgs.z.y);

		lgs.z = lgs.z + dz * dt;
#else
	// Implicit Euler method
		Vec2d z_p = lgs.z;
		z = Vec2d((lgs.z[0] + dt * lgs.v.x) / (1 + dt * sigma0 * fabs(lgs.v.x) / g),
			(lgs.z[1] + dt * lgs.v.y) / (1 + dt * sigma0 * fabs(lgs.v.y) / g));
		dz = (lgs.z - z_p) / dt;
		Vec2d vs2d = v - dz;
		vs = Vec3d(vs2d.x, vs2d.y, 0.0f);
#endif
		lgs.z = z;
		lgs.dz = dz;
		shapePair->LuGreState = lgs;

	}
}

void PHContactPoint::CompBias(){
	PHSceneIf* scene = GetScene();
	double dt    = scene->GetTimeStep();
	double dtinv = scene->GetTimeStepInv();
	double tol   = scene->GetContactTolerance();
	double vth   = scene->GetImpactThreshold();
	double fth	 = scene->GetFrictionThreshold();

	CompLuGreState();

	// Normal direction
	//	速度が小さい場合は、跳ね返りなし。
	if(vjrel[0] > - vth){
		double diff = std::max(shapePair->depth - tol, 0.0);
		// 粘弾性なし
		if(spring == 0.0 && damper == 0.0){
			db[0] = - engine->contactCorrectionRate * diff * dtinv;
			if (!finite(db[0])) {
				DSTR << "db[0] is not finite" << std::endl;
			}
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

	// determine static/dynamic friction based on tangential relative velocity
	double vt = vjrel[1];
	//isStatic = (-vth < vt && vt < vth);
	isStatic = (-fth < vt && vt < fth);
}

bool PHContactPoint::Projection(double& f_, int i) {
	PHSceneIf* scene = GetScene();
	double dt = scene->GetTimeStep();
	PHConstraint::Projection(f_, i);

	if(i == 0){	
		//垂直抗力 >= 0の制約
		if(f_ < 0.0){
			f_ = fx = flim0 = flim = 0.0;
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
		
		if(frictionModel == 1) {
			if (i == 1 ) {
				PHLuGreSt lgs = shapePair->LuGreState;
				f_ = -fx * (lgs.rot.trans() * (sigma0 * lgs.z + sigma1 * dz + sigma2 * v))[i - 1];
				return true;
			}
			else if (i == 2) {
				if (v.square() <= 1.0e-6) { // cf) PHConstraintEngine::EnumVertex   if(local.Ey().square() > 1e-6)
					f_ = 0.0f;
					return true;
				}
				return false;
			}
		}
		
		float lim = isStatic ? flim0 : flim;
		if (i == 3 && rotationFriction != 0.0f) {
			lim *= rotationFriction;
		}

		// 静止摩擦
		if (f_ > lim){
			f_ = lim;
			return true;
		}
		if (f_ < -lim){
			f_ = -lim;
			return true;
		}
		return false;
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
