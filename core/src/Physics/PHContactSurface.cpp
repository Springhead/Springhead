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
	
	int n = (int)sec.size();
	section.resize(n);
	normal .resize(n);
	for(int i = 0; i < n; i++){
		section[i] = sec[i];
	}
	for(int i = 0; i < n; i++){
		Vec3d p0 = section[i];
		Vec3d p1 = section[(i+1)%n];
		Vec3d d  = p1 - p0;
		normal[i] = Vec3d(0.0, d.z, -d.y);
		normal[i].unitize();
	}

	Vec3d rjabs[2];
	for(int i = 0; i < 2; i++){
		rjabs[i] = pose.Pos() - solid[i]->GetCenterPosition();	//剛体の中心から接触点までのベクトル
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
			contactArea += 0.5 * (section[i] % section[i+1]).norm();
		contactRadius = std::max(contactRadius, section[i].norm());
	}

	movableAxes.Clear();
}
void PHContactSurface::CompError() {
	 PHSceneIf* scene = GetScene();
	//衝突判定アルゴリズムの都合上、Correctionによって完全に剛体が離れてしまうのは困るので
	//誤差をepsだけ小さく見せる
	double tol = scene->GetContactTolerance();
	double diff = std::max(shapePair->depth - tol, 0.0);
	B[0] = -diff;
	
	//B[3 - 5]  ++change line 67 - 95
	// 剛体の法線ベクトル (normal0)
    Vec3d normal0 = solid[0]->GetOrientation() * Vec3d(0, 1, 0); // 剛体の上面がy方向を向いていると仮定
    // 床の法線ベクトル   (normal1)
    Vec3d normal1 = solid[1]->GetOrientation() * Vec3d(0, 1, 0); // 床の上面がy方向を向いていると仮定

    // 法線ベクトルを正規化
    normal0.unitize();
    normal1.unitize();

    // 2つの法線ベクトル間の角度を計算
    double cosAngle = normal0 * normal1; // 内積
    double angle = acos(cosAngle); // 角度を求める

    // 回転軸の計算（外積）CC  qqQ  
    Vec3d rotationAxis = normal0 % normal1;
    if (rotationAxis.norm() > 1e-10) { // 回転軸がほぼゼロベクトルでない場合
        rotationAxis.unitize();
    } else {
        rotationAxis = Vec3d(1, 0, 0); // デフォルトの回転軸（ゼロに近い場合の処理）DIIbbbbbb
    }


    // 回転角をB[3], B[4], B[5]に設定
    B[3] = angle * rotationAxis.x;
    B[4] = angle * rotationAxis.y;
    B[5] = angle * rotationAxis.z;

}

bool PHContactSurface::Iterate() {
	bool updated = false;

	// 垂直力 0
	for(int i = 0; i <= 0; ++i){
		//if(!dv_changed[i]) continue;
		//dv_changed[i] = false;
		dv  [i] = J[0].row(i) * solid[0]->dv + J[1].row(i) * solid[1]->dv;
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];

		ProjectNormalForce(fnew);

		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];
		
		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponse(df[i], i);
		}
	}
	// 摩擦力 1,2
	for(int i = 1; i <= 2; i++){
		//if(!dv_changed[i]) continue;
		//dv_changed[i] = false;
		dv  [i] = J[0].row(i) * solid[0]->dv + J[1].row(i) * solid[1]->dv;
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];
	}
	ProjectFrictionForce(fnew);
	for(int i = 1; i <= 2; i++){
		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];
		
		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponse(df[i], i);
		}
	}

	// 横モーメント 4,5
	for(int i = 4; i <= 5; ++i){
		//if(!dv_changed[i]) continue;
		//dv_changed[i] = false;
		dv  [i] = J[0].row(i) * solid[0]->dv + J[1].row(i) * solid[1]->dv;
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];
	}
	ProjectLateralMoment(fnew);
	for(int i = 4; i <= 5; ++i) {
		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];

		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponse(df[i], i);
		}
	}

	// 法線モーメント
	for(int i = 3; i <= 3; ++i){
		//if(!dv_changed[i]) continue;
		//dv_changed[i] = false;
		dv  [i] = J[0].row(i) * solid[0]->dv + J[1].row(i) * solid[1]->dv;
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];

		ProjectNormalMoment(fnew);
	
		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];

		if(std::abs(df[i]) > engine->dfEps){
			updated = true;
			CompResponse(df[i], i);
		}
	}

	return updated;
}

void PHContactSurface::ProjectNormalForce(SpatialVector& fnew){
	// 接触深度が許容値以下なら反力を出さない
	if(shapePair->depth < GetScene()->GetContactTolerance()){
		fnew[0] = 0.0;
		return;
	}
	//垂直抗力 >= 0の制約
	if(fnew[0] < 0.0){
		fnew[0] = 0.0;
		return;
	}
}

void PHContactSurface::ProjectFrictionForce(SpatialVector& fnew){
	// 接触断面内での最大横速度
	double vmax = sqrt(vjrel[1]*vjrel[1] + vjrel[2]*vjrel[2]) + contactRadius*std::abs(vjrel[3]);
	double vth  = GetScene()->GetFrictionThreshold();
	
	double ftmax = (vmax < vth ? mu0 : mu) * fnew[0];
	double ft    = sqrt(fnew[1]*fnew[1] + fnew[2]*fnew[2]);

	frictionMargin = ftmax - ft;

	if(frictionMargin < 0.0){
		double k = ftmax / ft;
		fnew[1] *= k;
		fnew[2] *= k;
		frictionMargin = 0.0;
	}
}

void PHContactSurface::ProjectLateralMoment(SpatialVector& fnew){
	PHSceneIf* scene = GetScene();

	const double eps = 1.0e-10;
	if(fnew[0] < eps){
		fnew[4] = fnew[5] = 0.0;
		return;
	}
	
	//回転摩擦以外のトルクの制限
	// ZMPを求める
	cop = Vec3d(0.0, fnew[5]/fnew[0], -fnew[4]/fnew[0]);
	copMargin = 1.0e10;

	Matrix2d A;
	Vec2d    b, s;
	Vec3d    p0, p1, n0, n1, d, c;
	int n = (int)section.size();
	for(int i = 0; i < n; i++){
		p0 = section[i];
		p1 = section[(i+1)%n];
		n0 = normal [(i-1+n)%n];
		n1 = normal [i];
		d  = p1  - p0;
		c  = cop - p0;
		
		// p0が最近点か判定
		A.col(0) = Vec2d(n0.y, n0.z);
		A.col(1) = Vec2d(n1.y, n1.z);
		b        = Vec2d(c .y, c .z);
		if(std::abs(A.det()) < eps)
			continue;
		s = A.inv() * b;
		if(s[0] >= 0.0 && s[1] >= 0.0){
			// copを射影
			cop = p0;
			copMargin = 0.0;
			break;
		}

		// p0-p1上に最近点があるか判定
		A.col(0) = Vec2d(d .y, d .z);
		A.col(1) = Vec2d(n1.y, n1.z);
		if(std::abs(A.det()) < eps)
			continue;
		s = A.inv() * b;
		if(0.0 <= s[0] && s[0] <= 1.0 && s[1] >= 0.0){
			cop = (1.0-s[0])*p0 + s[0]*p1;
			copMargin = 0.0;
			break;
		}
	}

	if(copMargin != 0.0){
		for(int i = 0; i < n; i++){
			p0 = section[i];
			n1 = normal [i];
			c  = cop - p0;
			copMargin = std::min(copMargin, -c * n1);
		}
	}

	/*if(std::abs(cop.y) > 0.1 || std::abs(cop.z) > 0.1){
		int hoge = 0;
		int n = (int)section.size();
		for(int i = 0; i < n; i++){
			p0 = section[i];
			p1 = section[(i+1)%n];
			n0 = normal [(i-1+n)%n];
			n1 = normal [i];
			d  = p1  - p0;
			c  = cop - p0;
		
			// p0が最近点か判定
			A.col(0) = Vec2d(n0.y, n0.z);
			A.col(1) = Vec2d(n1.y, n1.z);
			b        = Vec2d(c .y, c .z);
			if(std::abs(A.det()) < eps)
				continue;
			s = A.inv() * b;
			if(s[0] >= 0.0 && s[1] >= 0.0){
				// copを射影
				cop = p0;
				break;
			}

			// p0-p1上に最近点があるか判定
			A.col(0) = Vec2d(d .y, d .z);
			A.col(1) = Vec2d(n1.y, n1.z);
			if(std::abs(A.det()) < eps)
				continue;
			s = A.inv() * b;
			if(0.0 <= s[0] && s[0] <= 1.0 && s[1] >= 0.0){
				cop = (1.0-s[0])*p0 + s[0]*p1;
				break;
			}
		}

	}
	*/
	// 射影したcopからモーメントを逆算
	fnew[4] = -cop.z * fnew[0];
	fnew[5] =  cop.y * fnew[0];
}

void PHContactSurface::ProjectNormalMoment(SpatialVector& fnew){
	PHSceneIf* scene = GetScene();

	const double eps = 1.0e-10;
	if(fnew[0] < eps){
		fnew[3] = 0.0;
		return;
	}

	// 横力によってcopに作用する法線モーメント
	double ncop   = cop.y*fnew[2] - cop.z*fnew[1];
	double margin = (2.0/3.0)*copMargin*frictionMargin; 
	//double margin = (2.0/3.0)*contactRadius*frictionMargin;

	if(fnew[3] - ncop >  margin) fnew[3] = ncop + margin;
	if(fnew[3] - ncop < -margin) fnew[3] = ncop - margin;
}

}
