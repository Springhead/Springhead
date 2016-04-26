/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHContactSurface.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHContactSurface

PHContactSurface::PHContactSurface(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1, std::vector<Vec3d> sec):
    PHContactPoint(local, sp, p, s0, s1){
	
	section   = sec;

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

	// 接触断面の面積と半径（原点からの最長距離）
	contactArea   = 0.0;
	contactRadius = 0.0;
	for(int i = 0; i < (int)section.size(); i++){
		if(i != section.size()-1)
			contactArea += 0.5 * std::abs((section[i] % section[i+1]).norm());
		contactRadius = std::max(contactRadius, section[i].norm());
	}

	movableAxes.Clear();
}

bool PHContactSurface::Iterate() {
	bool updated = false;

	// -- 力
	for (int i=0; i<3; ++i) {
		if(!dv_changed[i])
			continue;
		dv_changed[i] = false;

		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];

		Projection(fnew[i], i);

		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];
		
		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponseDirect(df[i], i);
		}
	}

	// -- トルク
	for (int i=3; i<6; ++i) {
		if(!dv_changed[i])
			continue;
		dv_changed[i] = false;

		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];
	}

	// Projection
	ProjectionTorque(fnew);

	// Comp Response & Update f
	for (int i=3; i<6; ++i) {
		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];

		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponseDirect(df[i], i);
		}
	}
	return updated;
}

void PHContactSurface::ProjectionTorque(SpatialVector& fnew){
	PHConstraint::Projection(fnew[3], 3);
	PHConstraint::Projection(fnew[4], 4);
	PHConstraint::Projection(fnew[5], 5);
	fnew[3] = fnew[4] = fnew[5] = 0.0;
	return;

	PHSceneIf* scene = GetScene();

	const double eps = 1.0e-10;
	if(f[0] < eps){
		fnew[3] = fnew[4] = fnew[5] = 0.0;
		return;
	}
	
	// 回転摩擦の制限
	double Nlim0 = contactRadius * mu0 * f[0];
	double Nlim  = contactRadius * mu  * f[0];

	// 静止摩擦
	double vth = scene->GetImpactThreshold();
	double vn  = vjrel[3] * contactRadius;
	if (-vth < vn && vn < vth){
		fnew[3] = std::min(fnew[3],  Nlim0);
		fnew[3] = std::max(fnew[3], -Nlim0);
	}
	// 動摩擦
	else{
		fnew[3] = std::min(fnew[3],  Nlim);
		fnew[3] = std::max(fnew[3], -Nlim);
	}

	//回転摩擦以外のトルクの制限
	// ZMPを求める
	fpoint = Vec3d(0.0, -fnew[5]/f[0], fnew[4]/f[0]);

	Vec2d s;
	Vec3d d;
	Matrix2d A;
	double k = 0;
	int n = (int)section.size();
	for(int i = 0; i < n; i++){
		d = section[(i+1)%n] - section[i];
		A.col(0) = Vec2d(d.y, d.z);
		A.col(1) = Vec2d(-fpoint.y, -fpoint.z);
		if(std::abs(A.det()) < eps)
			continue;
		s = - A.inv() * Vec2d(section[i].y, section[i].z);
		if(0.0 <= s[0] && s[0] <= 1.0 && s[1] >= 0.0){
			k = std::max(k, s[1]);
			break;
		}
	}

	if(k < 1.0){
		fnew[4] *= k;
		fnew[5] *= k;
	}
	//DSTR << k << " " << section.size() << " " << contactArea << endl;

	/*
	// 接触断面の境界上のZMP最近点
	double d, dmin = numeric_limits<double>::max();
	Vec3d p0, p1, pmid, pmin;
	for(int i = 0; i < (int)section.size()-1; i++){
		p0 = section[i+0];
		p1 = section[i+1];
		d = (p0 - fpoint).square();
		if(d < dmin){
			pmin = p0;
			dmin = d;
		}
		double l = (p1 - p0).square();
		if(l > eps){
			double s = (fpoint - p0) * (p1 - p0) / l;
			pmid = (1-s) * p0 + s * p1;
			d = (pmid - fpoint).square();
			if(d < dmin){
				pmin = pmid;
				dmin = d;
			}
		}
	}

	// ZMPが境界上の最近点よりも遠ければ射影
	pmin = section[0];
	if((pmin - contactCenter).square() < (fpoint - contactCenter).square()){
		fpoint = pmin;
		fnew[4] =  fpoint.z * f[0];
		fnew[5] = -fpoint.y * f[0];
	}
	*/
}

}
