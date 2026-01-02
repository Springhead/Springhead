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

	int fmodel0 = mat[0]->frictionModel;
	int fmodel1 = mat[1]->frictionModel;
	frictionModel = max(fmodel0, fmodel1);
	if ( frictionModel >= FrictionModel::LUGRE) {
		// LuGre model parameters
		sigma0 = std::max(mat[0]->bristlesSpringK, mat[1]->bristlesSpringK);
		sigma1 = std::max(mat[0]->bristlesDamperD, mat[1]->bristlesDamperD);
		sigma2 = std::max(mat[0]->bristlesViscosityV, mat[1]->bristlesViscosityV);
		timeVaryA = std::max(mat[0]->timeVaryFrictionA, mat[1]->timeVaryFrictionA);
		timeVaryB = std::max(mat[0]->timeVaryFrictionB, mat[1]->timeVaryFrictionB);
		timeVaryC = std::max(mat[0]->timeVaryFrictionC, mat[1]->timeVaryFrictionC);

		double dt = s->GetTimeStep();
		CDShapePairState st;
		sp->GetSt(st);
		unsigned int contactDuration = st.contactDuration;

		PHLuGreSt lgs = sp->LuGreState;

		if (contactDuration == 0) {
			// Initialize LuGre state
			//printf("initialize lugre state\n");
			lgs.T = 0.0;
			lgs.z = Vec3d::Zero();
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
			// Apply rotation to constraint coordinate
			pose.Ori() = pose.Ori() * Quaterniond::Rot(-lgs.rot.angle(), 'x');// *pose.Ori();
			lugreDirection = lgs.rot.angle();
		}
		lgs.local_p = local;
		sp->LuGreState = lgs;
		z_p = lgs.z;
		T_p = lgs.T;

		g = 1.0f;
		D = 1.0f;
		Dinv2 = 1.0f;
		dDdv.col(0) = Vec3d::Zero();
		req = 0.0001;

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

	for (int i = 0; i < 2; i++) {
		poseSolid[i].Pos() = solid[i]->GetFramePosition();
		poseSolid[i].Ori() = solid[i]->GetOrientation();
		// local: 接触点の関節フレーム は，x軸を法線, y,z軸を接線とする
		(i == 0 ? poseSocket : posePlug).Ori() = Xj[i].q = poseSolid[i].Ori().Conjugated() * pose.Ori();
		(i == 0 ? poseSocket : posePlug).Pos() = Xj[i].r = poseSolid[i].Ori().Conjugated() * (pose.Pos() - poseSolid[i].Pos());
	}

	if (rotationFriction == 0.0f) {
		//movableAxes.Enable(3);
	}
	movableAxes.Enable(4);
	movableAxes.Enable(5);

}

void PHContactPoint::CompLuGreState() {
	PHSceneIf* scene = GetScene();
	double dt = scene->GetTimeStep();
	if (frictionModel >= FrictionModel::LUGRE) {
		PHLuGreSt lgs = shapePair->LuGreState;
		// Get relative velocity
		//v = lgs.rot * Vec2d(vjrel[1], vjrel[2]);
		v = Vec3d(vjrel[1] + dv[1], vjrel[2] + dv[2], req*(vjrel[3] + dv[3]));

		// Plast Elastic Model
#if 0
		const double z_ba = 0.8*normalForce / sigma0;
		double alpha = 0.0f;
		if (z_ba < z.norm()) {
			alpha = 1.0f;
		}
#endif
		if (v.norm() < 1.0e-4){
			v = Vec3d::Zero();
		}
		lgs.v = v;

		// g(T)
		g = 1.0f;
		// D = 1 + dt * sigma0 * |v| / g(v)
		Vec3d dgdv = Vec3d::Zero();
		switch (frictionModel) {
		case FrictionModel::LUGRE:
			g = timeVaryA + timeVaryB * exp(- pow(v.norm() / timeVaryC, 2));
			dgdv = -2.0f * timeVaryB * exp(-pow(v.norm() / timeVaryC, 2)) * (v / (timeVaryC * timeVaryC));
			break;

		case FrictionModel::LUGRE_TV:
			g = timeVaryA + timeVaryB * log(timeVaryC * T_p + 1);
			dgdv = -(dt * sigma0 * T_p) / g * timeVaryC * timeVaryB / (timeVaryC * T_p + 1) * (v.unit());
			break;

		case FrictionModel::LUGRE_OC:
			// TODO

			break;
		}

		if (v.norm() < 1.0e-4) {
			dgdv = Vec3d::Zero();
		}
		else
			dDdv.col(0) = dt* sigma0* (v.unit() * g - v.norm() * dgdv) / (g * g);

		// T
		// dT/dt = 1 - (sigma0 * |v|) / g(T) * T
		double T = (T_p + dt) / (1.0 + sigma0 / g * v.norm() * dt);
		lgs.T = T;
		stickT = lgs.T;
		isSticking = (lgs.T >= T_p);

		// Calculate the denominator of implicit LuGre equation
		D = 1.0f + dt * sigma0 * v.norm() / g;
		Dinv2 = 1.0f / (D * D);

		// z
		// dz/dt = v - (sigma0 * |v|) / g(T) * z
		z = (z_p + dt * v) / D;
		dz =  (z - z_p) / dt;
		//dz = v - (sigma0 * v.norm() / g) * z_pn;

		Vec3d vs2d = v - dz;
		vs = Vec3d(vs2d.x, vs2d.y, 0.0f);
		//std::cout << z << dz << normalForce << g <<  std::endl;

		frictionForce = -(sigma0 * z + sigma1 * dz + sigma2 * v);

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

	//CompLuGreState(1.0);

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

	if (frictionModel >= LUGRE) {
		// LuGre initial Bias
		CompLuGreState();
		CompLuGreDfDvInv();
		Vec3d db2 = dfdvInv * frictionForce - v;
		db[1] = db2.x;
		db[2] = db2.y;
		db[3] = db2.z / req;
		//db[1] = (1.0 / (sigma1 + sigma0 * dt)) * sigma0 * z_p.x;
		//db[2] = (1.0 / (sigma1 + sigma0 * dt)) * sigma0 * z_p.x;
		PHSceneIf* scene = GetScene();
		double dt = scene->GetTimeStep();
		if (true) {
			//Matrix2d dfdvInv = CompLuGreDfDvInv();
			//Vec2d dfdvInv = 1.0f * CompLuGreDfDvInv() * Vec2d(1.0f, 1.0f);
			dA[1] = -dfdvInv[0][0] / dt;
			dA[2] = -dfdvInv[1][1] / dt;
			dA[3] = -dfdvInv[2][2] / dt / (req*req);
			//CompLuGreState();
		}
	}
}

Matrix3d PHContactPoint::CompLuGreDfDvInv() {
	PHSceneIf* scene = GetScene();
	double dt = scene->GetTimeStep();

	TMatrixCol<3, 1, double> z_p_;
	z_p_.col(0) = z_p;
	TMatrixCol<3, 1, double> v_;
	v_.col(0) = v;

	//dgdv.col(0) = Vec2d(1.0f, 1.0f);
	Matrix3d dfdv = (
		-(sigma0 * Dinv2 * (D * dt * Matrix3d::Unit() - (z_p_ + dt * v_) * dDdv.trans()))
		- (sigma1 * Dinv2 * (D * Matrix3d::Unit() - (1.0f / dt * z_p_ + v_) * dDdv.trans()))
		- (sigma2 * Matrix3d::Unit())
		);
	dfdvInv = dfdv.inv();
	//std::cout << "dfdv" << dfdv << "D:" << D << "f: " << frictionForce <<  std::endl;
	return dfdvInv;
}

bool PHContactPoint::Iterate() {
	if (frictionModel < LUGRE) {
		return PHConstraint::Iterate();
	}
	PHSceneIf* scene = GetScene();
	double dt = scene->GetTimeStep();
	bool updated = false;
	int i = 0;
	// x-axis (normal)
	dA[i] += engine->regularization;
	// Gauss-Seidel Update
	dv[i] = J[0].row(i) * solid[0]->dv +J[1].row(i) * solid[1]->dv;
	res[i] = b[i] + dA[i] * f[i] + dv[i];
	fnew[i] = f[i] - Ainv[i] * res[i];
	// Projection
	Projection(fnew[i], i);
	// Comp Response & Update f
	df[i] = fnew[i] - f[i];
	f[i] = fnew[i];

	if (std::abs(df[i]) > engine->dfEps) {
		updated = true;
		CompResponse(df[i], i);
	}
	fx = f[i];
	if(fx <= 1.0e-6)
		fx = 1.0e-6;

	// y, z, rotation friction
	for (int n = 1; n < 4; ++n) {
		i = n;

		dA[i] += engine->regularization;
		Ainv[i] = engine->accelSOR / (A[i] + dA[i] / (fx / dt));

		//std::cout << "Axis[" << i << "] f: " << f[i] << " Ainv: " << Ainv[i] << " b: " << b[i] << " dA: " << dA[i] << std::endl;

		// Gauss-Seidel Update
		dv[i] = J[0].row(i) * solid[0]->dv +J[1].row(i) * solid[1]->dv;
		res[i] = b[i] + dv[i];
		res[i] -= dfdvInv.row(i-1) * Matrix3d::Diag(1.0f, 1.0f, 1.0f/(req)) * f.sub_vector(1, Vec3d())  / fx;
		if (i == 3)
			res[i] /= req;
		fnew[i] = f[i] - Ainv[i] * res[i];

		// Projection
		Projection(fnew[i], i);

		// Comp Response & Update f
		df[i] = fnew[i] - f[i];
		f[i] = fnew[i];

		if (std::abs(df[i]) > engine->dfEps) {
			updated = true;
			CompResponse(df[i], i);
		}
	}	
	for (int n = 4; n < axes.size(); ++n) {
		i = n;

		dA[i] += engine->regularization;
		Ainv[i] = engine->accelSOR / (A[i] + dA[i]);

		// Gauss-Seidel Update
		dv[i] = J[0].row(i) * solid[0]->dv + J[1].row(i) * solid[1]->dv;
		res[i] = b[i] + dA[i] * f[i] + dv[i];
		fnew[i] = f[i] - Ainv[i] * res[i];
		
		// Projection
		Projection(fnew[i], i);

		// Comp Response & Update f
		df[i] = fnew[i] - f[i];
		f[i] = fnew[i];

		if (std::abs(df[i]) > engine->dfEps) {
			updated = true;
			CompResponse(df[i], i);
		}
	}
	CompLuGreState();
	return updated;
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

		float lim = isStatic ? flim0 : flim;
		if(frictionModel >= FrictionModel::LUGRE ) {
			return false;
			PHLuGreSt lgs = shapePair->LuGreState;
#if 0
			if (i == 1 || (i == 2 && v.square() <= 1.0e-6)) { 
				// cf) PHConstraintEngine::EnumVertex   if(local.Ey().square() > 1e-6)
				// If the relative velocity is zero, apply the friction force in the x and y directions (constraint coordinate).
				f_ = -fx * (lgs.rot.trans() * (sigma0 * lgs.z + sigma1 * dz + sigma2 * v))[i - 1];
				return true;
			}
#else
			if (i == 1 || i == 2) {
				f_ = -fx * (sigma0 * z[i - 1] + sigma1 * dz[i - 1] + sigma2 * v[i - 1]);
				return true;
			}
#endif
		}
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
