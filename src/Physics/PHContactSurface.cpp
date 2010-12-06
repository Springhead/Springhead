/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#pragma hdrstop

#define USE_ROTATEFRICTION 1//回転摩擦の制限を行うかどうか

using namespace PTM;
using namespace std;
namespace Spr{;

PHContactSurface::PHContactSurface(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1, std::vector<Vec3d> sec){
	shapePair = sp;
	pos = p;
	solid[0] = s0, solid[1] = s1;
	section = sec;
	lastL = lastR = 0;

	//回転摩擦用
#if USE_ROTATEFRICTION
	rotateFriction = 0.0;
	segmentationNum = 0;
	interval = 0.5;
	IntegrateArea();
#endif

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
}

PHContactSurface::PHContactSurface(PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1, std::vector<Vec3d> sec){
	shapePair = sp;
	pos = p;
	solid[0] = s0, solid[1] = s1;
	section = sec;
	lastL = lastR = 0;

	//回転摩擦用
	rotateFriction = 0.0;
	segmentationNum = 0;
	interval = 0.5;
	IntegrateArea();

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
}

void PHContactSurface::SetConstrainedIndex(int* con){
//	con[0] = con[1] = con[2] = con[3] = con[4] = con[5] = true;
	 for(int i = 0; i<6;i++){
		con[i] = i;
	}
	targetAxis = 6;
}

void PHContactSurface::CompBias(){
	//	correctionを位置LCPで別に行う場合は、速度を変更しての位置補正はしない。
	if (engine->numIterContactCorrection) return;
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	double overlap = 0.002;

#if 1
	//	接触用の correctionRate
	double contactCorrectionRate = 0;

	double e;
	//	速度が小さい場合は、跳ね返りなし。
	if (vjrel[0]*GetScene()->GetTimeStep() > -0.1){
		e = 0;
		contactCorrectionRate = engine->contactCorrectionRate;
	}else{
		//	跳ね返り係数: 2物体の平均値を使う
		e = 0.5 * (shapePair->shape[0]->GetMaterial().e + shapePair->shape[1]->GetMaterial().e);
		//	跳ね返るときは補正なし
		contactCorrectionRate = 0;
	}

	if (overlap > shapePair->depth) overlap = shapePair->depth;
	db[0] = - contactCorrectionRate * (shapePair->depth - overlap) / GetScene()->GetTimeStep() + e * vjrel[0];
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

void PHContactSurface::Projection(double& f, int k){
	static double flim;
	if(k == 0){	//垂直抗力 >= 0の制約
		f = max(0.0, f);
		//	最大静止摩擦
		flim = 0.5 * (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0) * f;	}
	else if(k == 1 || k == 2){
		//	動摩擦を試しに実装してみる。
		double fu;
		if (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0 == 0){
			fu = 0;
		}else{
			fu = (shapePair->shape[0]->GetMaterial().mu + shapePair->shape[1]->GetMaterial().mu)
				/ (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0)
				* flim;	
		}
		if (-0.01 < vjrel[1] && vjrel[1] < 0.01){	//	静止摩擦
			if (f > flim) f = fu;
			else if (f < -flim) f = -fu;
		}else{					//	動摩擦
			if (f > fu) f = fu;
			else if (f < -fu) f = -fu;		
		}
#if 0
		//|摩擦力| <= 最大静止摩擦の制約
		//	・摩擦力の各成分が最大静止摩擦よりも小さくても合力は超える可能性があるので本当はおかしい。
		//	・静止摩擦と動摩擦が同じ値でないと扱えない。
		//摩擦係数は両者の静止摩擦係数の平均とする
		f = min(max(-flim, f), flim);
#endif
		assert(f < 10000);
	}
}

void PHContactSurface::IterateLCP(){
	if(!bEnabled || !bFeasible || bArticulated)
		return;
	FPCK_FINITE(f.v());

	SpatialVector fnew, df;

	for(int j = 0; j < targetAxis; j++){
//		if(!constr[j])continue;
		int i = numCondition[j];
		fnew[i] = f[i] - engine->accelSOR * Ainv[i] * (dA[i] * f[i] + b[i] + db[i] 
				+ J[0].row(i) * solid[0]->dv + J[1].row(i) * solid[1]->dv);

		// とりあえず落ちないように間に合わせのコード
		if (!FPCK_FINITE(fnew[i])) fnew[i] = f[i]; //naga 特定条件下では間に合わせのコードでも落ちる
		if (!FPCK_FINITE(fnew[0])){
			FPCK_FINITE(b[0]);
//			DSTR << AinvJ[0].vv << AinvJ[1].vv;
//			DSTR << AinvJ[0].vw << AinvJ[1].vw;
//			DSTR << dA.v[j];
//			DSTR << std::endl;
//			DSTR << "f.v:" << f.v << "b.v:" << b.v << std::endl;
			DSTR << "s0:" << (solid[0]->dv) << std::endl;
			DSTR << "s1:" << (solid[1]->dv)  << std::endl;
		}
		if(j > 2)continue;	//トルクは全トルクの計算が終わってから、Projectionする
		Projection(fnew[i], i);
		df[j] = fnew[i] - f[i];
		CompResponse(df[i],i);
		f[j] = fnew[i];
	}

	//トルクのProjection　
	//fnew全部使って行う
	ProjectionTorque(fnew);

	//剛体に加える(各トルクでProjection以降のことやる)
	for(int j = 3; j < 6; j++){
		df[j] = fnew[j] - f[j];
		CompResponse(df[j],j);
		f[j] = fnew[j];
	}
}

/**
<< void ProjectionTorque(SpatialVector fnew) >>
====================================================================================
  brief		IterateLCPで呼ばれるトルクの制限を行う関数
  param		SpatialVector fnew 制限する前の力とトルク（力は制限済み）
  return	なし
====================================================================================
【基本の流れ】
	1. 力の大きさが０の場合はトルクを０として終わり
	2. ０でないとき、まず回転摩擦となるトルクを制限する．
	3．ZMPを求める
		3-1.力と直交するトルクを求める
		3-2.ZMP×力=トルクの関係と,ZMPは接触平面上にありx軸の座標=0であることから求まる
	4. 作用点が接触面内か面外か調べ、面内なら終わり
	5. 面外にあるとき、一番近い点を新しい作用点とする
	6. トルクを求め直して、fnewに代入
*/
void PHContactSurface::ProjectionTorque(SpatialVector& fnew){
	Vec3d F(f[0], f[1], f[2]);
	Vec3d N(fnew[3], fnew[4], fnew[5]);
	
	if(F.norm() != 0.0){	//力の大きさが0でないなら制限する。
		//まず回転摩擦の制限
#if	USE_ROTATEFRICTION
		//回転摩擦の実装
		//rotateFrictionに摩擦係数と、垂直抗力をsegmentationNumで割ったものをかける
		double rotateFrictionLimit;//最大静止回転摩擦
		rotateFrictionLimit = rotateFriction * 0.5 * (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0);
		if(segmentationNum != 0){
			rotateFrictionLimit *= f[0]/segmentationNum;
		}
		else{
			//cout << "segmentation = 0" << endl;
			rotateFrictionLimit *= f[0];
		}
		double activeRotateFriction;//回転動摩擦
		if (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0 == 0){
			activeRotateFriction = 0;
		}else{
			activeRotateFriction = (shapePair->shape[0]->GetMaterial().mu + shapePair->shape[1]->GetMaterial().mu)
			/ (shapePair->shape[0]->GetMaterial().mu0 + shapePair->shape[1]->GetMaterial().mu0)
			* rotateFrictionLimit;	
		}
		//接触面の垂直方向の角速度が発生してないなら静止摩擦、していれば動摩擦
		if (-0.01 < vjrel[3] && vjrel[3] < 0.01){	//	静止摩擦
			if (fnew[3] > rotateFrictionLimit) fnew[3] = activeRotateFriction;
			else if (fnew[3] < -rotateFrictionLimit) fnew[3] = -activeRotateFriction;
		}else{					//	動摩擦
			if (fnew[3] > activeRotateFriction) fnew[3] = activeRotateFriction;
			else if (fnew[3] < -activeRotateFriction) fnew[3] = -activeRotateFriction;		
		}
#endif
		N.x = fnew[3];
		
		//回転摩擦以外のトルクの制限
		//ZMPを求める		

		//NはFと直交しているとは限らない
		//NのFに直交している成分でZMPを求めて制限する
		Vec3d N_orthogonalF;//NのFに直交している成分ベクトル
		Vec3d N_parallelF;//NのF成分ベクトル

		N_parallelF = N*F.unit()*F.unit();
		N_orthogonalF = N - N_parallelF;

		fpoint.x = 0.0;
		fpoint.y = -N_orthogonalF.z / F.x;
		fpoint.z = N_orthogonalF.y / F.x;

		//面内か面外か
		int L, R;	//面のどの領域に作用点があるか保持するための変数 L 面外から見て辺の左側の頂点　R 右側の頂点
		if(!CheckInside(L, R)){
			//面外ならZMPを面内に直す
			//垂線に下ろせるかどうかの判定
			if(lastL != lastR){
				//繰り返し計算の2回目からは前回、最近点があった領域から探索をする。
				//lastL,lastRは前回、最近点があった領域を示している。
				L = lastL;
				R = lastR;
			}
			if(dot(section[L]-section[R],fpoint-section[R])>=0 && dot(section[R]-section[L],fpoint-section[L])>=0 && CalcSignedArea(section[L],section[R],fpoint) < 0){
				//垂線を下ろせる場合
				//ZMPに垂線の長さ×辺の法線単位ベクトルを足して交点を求める
				//辺の法線単位ベクトル（接触面内への）を求める
				//辺RLのベクトルは(0,section[L].y - section[R].y, section[L].z - section[R].z)
				//これに直交するベクトルは2種類あってそのうち接触面内へのベクトルが↓
				Vec3d side_normal;
				side_normal.x = 0;
				side_normal.y = section[L].z - section[R].z;
				side_normal.z = -(section[L].y - section[R].y);
				side_normal.unitize();

				//垂線の長さを求める
				double dist_of_normal;
				Vec3d tempVec = (section[L]-section[R]) ^ (fpoint-section[R]);
				Vec3d tempVec2(section[L]-section[R]);
				dist_of_normal = tempVec.norm() / tempVec2.norm();

				//新しい作用点を求める
				//作用点に垂線の長さ倍した辺の法線ベクトル(内向き)を足す
				fpoint = fpoint + (dist_of_normal * side_normal);
			}
			else{
				//下ろせなければ、最近点を探索し最近点を作用点とする
				SearchClosestPoint(L, R);
			}
			//新しいZMPから制限されたトルクを求める
			N_orthogonalF = cross(fpoint, F);
			N = N_orthogonalF;
			fnew[4] = N[1];
			fnew[5] = N[2];
		}
	}
	else{		//力の大きさが0ならトルク0にする。
		fnew[3] = fnew[4] = fnew[5] = 0.0;
	}
}

/**
<< void IntegrateArea() >>
====================================================================================
  brief		回転摩擦を求める為に接触面の積分を行う関数
====================================================================================
*/
void PHContactSurface::IntegrateArea()
{
	std::vector<Spr::Vec3d> Factor; //接触面の辺を表す直線の方程式の係数　ay+bz=cのa,b,cが(x,y,z)に対応
	std::vector<Spr::Vec2d> RangeY; //直線のy座標の範囲(小,大)が(x,y)に対応
	std::vector<Spr::Vec2d> RangeZ; //直線のz座標の範囲(小,大)が(x,y)に対応
	double y_min,y_max;//積分範囲
	double z_min,z_max;//積分範囲(各yに対して設定する)

	Factor.resize(section.size());
	RangeY.resize(section.size());
	RangeZ.resize(section.size());
	for(unsigned int i=0; i<section.size();i++){
		//係数a,b,定数cの設定
		//係数a:Factor[i].x,係数b:Factor[i].y,定数c:Factor[i].z
		//sectionのy,zから設定.2点を通る直線の方程式から
		int s0 = i;
		int s1 = i+1;
		if(s1 == section.size()){
			s1 = 0;
		}

		Factor[i].x =(section[s1].z-section[s0].z);
		Factor[i].y = (section[s0].y-section[s1].y);
		Factor[i].z = (section[s0].y*section[s1].z - section[s1].y*section[s0].z);
		
		//y座標の範囲を設定
		//Range.x 小さい側　Range.y 大きい側
		//sectionのyから設定
		if(section[s0].y<=section[s1].y){
			RangeY[i].x = section[s0].y;
			RangeY[i].y = section[s1].y;
		}
		else{
			RangeY[i].x = section[s1].y;
			RangeY[i].y = section[s0].y;
		}
		//y座標の全体の範囲 (y_min,y_max)を設定
		if(i==0){
			y_min = RangeY[i].x;
			y_max = RangeY[i].y;
		}
		else{
			if(y_min > RangeY[i].x){
				y_min = RangeY[i].x;
			}
			if(y_max < RangeY[i].y){
				y_max = RangeY[i].y;
			}
		}

		//z座標の範囲を設定
		if(section[s0].z<=section[s1].z){
			RangeZ[i].x = section[s0].z;
			RangeZ[i].y = section[s1].z;
		}
		else{
			RangeZ[i].x = section[s1].z;
			RangeZ[i].y = section[s0].z;
		}
	}

	//ContactSurfaceがある原点からの距離を積分
	double Y,Z;//積分するY座標,Z座標
	Y=y_min;
	for(int i=0; Y<=y_max; i++){
		//Y座標を決定
		Y = y_min + interval*i;
		if(Y > y_max){
			Y = y_max;
		}
		
		std::vector<int> eq;//Yが範囲にある直線番号を持つ
		//どの式がYの範囲にあるか
		for(unsigned int j=0; j<RangeY.size(); j++){
			if((Y>=RangeY[j].x && Y<=RangeY[j].y)){
				eq.push_back(j);
			}
		}

		//範囲内にYがある辺の式からZの範囲(z_min,z_max)を求める．
		//ay+bz=c z=c/b-(a/b)y
		double temp_z;
		for(unsigned int j=0; j<eq.size(); j++){
			if(fabs(Factor[eq[j]].y) < 1e-10){
				//yの係数が0の辺の場合
				//その辺のz座標がそのまま範囲となる
				z_min = RangeZ[eq[j]].x;
				z_max = RangeZ[eq[j]].y;
				break;
			}
			if(j==0){
				z_min = Factor[eq[j]].z/Factor[eq[j]].y-(Factor[eq[j]].x/Factor[eq[j]].y)*Y;
			}
			else{
				temp_z = Factor[eq[j]].z/Factor[eq[j]].y-(Factor[eq[j]].x/Factor[eq[j]].y)*Y;
				if(z_min <= temp_z){
					z_max = temp_z;
				}
				else{
					z_max = z_min;
					z_min = temp_z;
				}
			}
		}
		
		Z = z_min;
		//Zの範囲で積分を行う．
		for(int j=0; Z<=z_max;j++){
			Z = z_min + interval*j;
			if(Z > z_max){
				Z = z_max;
			}
			rotateFriction += sqrt(Y*Y+Z*Z);
			segmentationNum++;
			if(Z==z_max){
				break;
			}
		}
		if(Y==y_max){
			break;
		}
	}
}

/**
<< double CalcSignedArea(Vec3d p, Vec3d q, Vec3d r) >>
====================================================================================
  brief		三角形の符号付き面積の計算を行う関数
  param		Vec3d	p,q,r	頂点
  return	符号付き面積
====================================================================================
*/
double PHContactSurface::CalcSignedArea(Vec3d p, Vec3d q, Vec3d r){
	return ( (p.y-r.y)*(q.z-r.z) + (q.y-r.y)*(r.z-p.z) ) / 2;
}

/**
<< bool CheckInside(void) >> 
====================================================================================
  brief		ZMPfpointが接触面内にあるか
  param		int& L	作用点がある領域の頂点を記録
			int& R　作用点がある領域の頂点を記録
  return	面内：true	面外：false
====================================================================================
※接触面内の点は原点としている．
*/
bool PHContactSurface::CheckInside(int& L, int& R){
	int l,m,r;	//m 探索範囲の中点
	l = 0;
	r = section.size();	//探索範囲の初期化
	while(r-l>1){
		m = (l+r)/2;
		if(CalcSignedArea(section[l], Vec3d(0.0,0.0,0.0), section[m]) <= 0.0){/* if angle(l, g, m)<=180) */
			if(CalcSignedArea(section[l], Vec3d(0.0,0.0,0.0), fpoint) <= 0.0 && CalcSignedArea(fpoint, Vec3d(0.0,0.0,0.0), section[m]) <= 0.0){
				r = m;
			}
			else{
				l = m;
			}
		}
		else{/*angle(l, g, m) > 180*/
			if(CalcSignedArea(section[l], Vec3d(0.0,0.0,0.0), fpoint) >= 0.0 && CalcSignedArea(fpoint, Vec3d(0.0,0.0,0.0), section[m]) >= 0.0){
				l = m;
			}
			else{
				r = m;
			}
		}
	}
	if(r == section.size()){	//section[section.size()] は section[0]なので
		r = 0;
	}
	if(CalcSignedArea(section[l], section[r], fpoint) >= 0.0){
		return true;	//面内
	}
	else{ 
		L = l;
		R = r;
		return false;	//面外
	}
}

/**
<< void PHContactSurface::SearchClosestPoint >>
====================================================================================
  brief		作用点の最近点を見つけて作用点とする
  param		int L  作用点がある領域の頂点（面外から見て左側）
			int R  作用点がある領域の頂点（面外から見て右側）
  return	なし
====================================================================================
  【基本の流れ】
  作用点がある領域の辺に作用点から垂線が下ろせなかった場合に働く
  1.作用点がある領域の辺の頂点と作用点との距離を記録
  作用点がある領域の隣接する領域を 時計回りと反時計回りの２つの順で探索し始める。
  作用点がある領域の辺の頂点で、作用点との距離が離れている側は探索をしない。
  2.隣接する領域の辺に垂線が下ろせるかを確認する→下ろせたら、この順序での探索を終える。
  3.下ろせなかった場合、辺の頂点と作用点との距離と、記録してある距離を比べる。
  4-1.より短かった場合、距離を記録しさらに先の領域で2から繰り返す。
  4-2.より長かった場合、探索を終了し、短い頂点を作用点の候補とし、この順序での探索を終える。
  
  両方の順序で探索を終えたら
  5.領域を時計回りと反時計回りに探索し終えたことになるので、作用点の候補を比べ、より短い側を作用点とする。
*/
void PHContactSurface::SearchClosestPoint(int L, int R){
	
	Vec3d L_fp(fpoint-section[L]);  //頂点から作用点へのベクトル
	Vec3d R_fp(fpoint-section[R]);
	double L_distance = L_fp.norm(); //頂点から作用点までの距離
	double R_distance = R_fp.norm(); 
	
	Vec3d ClosestPointL=section[L];	//時計回りの探索での最近点のベクトル
	Vec3d ClosestPointR=section[R];	//反時計回りの探索での最近点のベクトル
	
	int l,r;
	
	for(unsigned int i=0; i<section.size(); i++,L--){	//時計回りの探索 
		//最初にLの頂点がRの頂点より作用点から離れているか調べ、離れていれば終わりにする
		if(i==0 && L_distance > R_distance){
			break;
		}
		if(L<0){
			L = section.size()-1;
		}
		if(L==0){
			l = section.size()-1;
			r = L;
		}
		else{
			l = L-1;
			r = L;
		}		
		if(dot(section[l]-section[r],fpoint-section[r])>=0 && dot(section[r]-section[l],fpoint-section[l])>=0 && CalcSignedArea(section[l], section[r], fpoint) < 0){
			//面内を通らない垂線が下ろせる場合			
			
			//辺直交し接触面内を向く単位ベクトルを求める
			Vec3d side_normal;
			side_normal.x = 0;
			side_normal.y = section[l].z - section[r].z;
			side_normal.z = -(section[l].y - section[r].y);
			side_normal.unitize();

			//垂線の長さを求める
			double dist_of_normal;
			Vec3d tempVec  = (section[l]-section[r]) ^ (fpoint-section[r]);
			Vec3d tempVec2 = section[l]-section[r];
			dist_of_normal = tempVec.norm() / tempVec2.norm();

			//新しいZMPを求める
			//ZMPに垂線の長さ倍した辺の法線ベクトル(内向き)を足す
			ClosestPointL = fpoint + (dist_of_normal * side_normal);
			break;
		}
		else{
			L_fp = fpoint-section[l];
			if(L_distance > L_fp.norm()){
				//短くなった場合
				L_distance = L_fp.norm();
				ClosestPointL = section[l];
			}
			else{
				//長くなった場合
				if(i != 0){
					L++;
					if(L == section.size()){
						L = 0;
					}
				}
				break;
			}
		}
	}

	for(unsigned int i=0; i<section.size(); i++,R++){	//反時計回りの探索
		//最初にRの頂点がLの頂点より作用点から離れているか調べ、離れていれば終わりにする
		if(i==0 && R_distance > L_distance){
			break;
		}
		if(R>=(signed)section.size()){
			R = 0;
		}
		if(R==section.size()-1){
			l = R;
			r = 0;
		}
		else{
			l = R;
			r = R+1;
		}		
		if(dot(section[l]-section[r],fpoint-section[r])>=0 && dot(section[r]-section[l],fpoint-section[l])>=0 && CalcSignedArea(section[l], section[r], fpoint) < 0){
			//面内を通らない垂線が下ろせる場合

			//辺に直交し接触面内を向く単位ベクトルを求める
			Vec3d side_normal;
			side_normal.x = 0;
			side_normal.y = section[l].z - section[r].z;
			side_normal.z = -(section[l].y - section[r].y);
			side_normal.unitize();

			//垂線の長さを求める
			double dist_of_normal;
			Vec3d tempVec  = (section[l]-section[r]) ^ (fpoint-section[r]);
			Vec3d tempVec2 = section[l]-section[r];
			dist_of_normal = tempVec.norm() / tempVec2.norm();

			//新しいZMPを求める
			//ZMPに垂線の長さ倍した辺の法線ベクトル(内向き)を足す
			ClosestPointR = fpoint + (dist_of_normal * side_normal);
			break;
		}
		else{
			R_fp = fpoint-section[r];
			if(R_distance > R_fp.norm()){
				//短くなった場合
				R_distance = R_fp.norm();
				ClosestPointR = section[r];
			}
			else{
				//長くなった場合
				if(i != 0){
					R--;
					if(R < 0){
						R = section.size()-1;
					}
					break;
				}
			}
		}
	}
	
	L_fp = fpoint-ClosestPointL;
	R_fp = fpoint-ClosestPointR;
	
	if(L_fp.norm() <= R_fp.norm()){
		fpoint = ClosestPointL;
		lastL = L-1;
		if(lastL < 0){
			lastL = section.size() - 1;
		}
		lastR = L;
	}
	else{
		fpoint = ClosestPointR;
		lastL = R;
		lastR = R+1;
		if(lastR == section.size()){
			lastR = 0;
		}
	}
}

}
