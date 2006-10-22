/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#include <Foundation/Scene.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
double relError = 1e-6;
double absError = 1e-10;


static Vec3d p[4];			// Aのサポートポイント(ローカル系)
static Vec3d q[4];			// Bのサポートポイント(ローカル系)
static Vec3d p_q[4];		// ミンコスキー和上でのサポートポイント(ワールド系)

//	過去のSupportPointが張る形状(点・線分・3角形，4面体)．最大4点まで．
static int usedPoints;		//	下位4ビットが，どの4点で現在の形状が構成されるかを表す．
static int lastPoint;		//	最後に見つけたSupportPointをしまった場所(0～3)
static int lastUsed;		//	lastUsed = 1<<lastPoint (ビットで表したもの）
static int allUsedPoints;	//	lastUsedを加えたもの
static double det[16][4];	//	係数
//	det[3][0] * p[0] + det[3][1]*p[1] + det[3][2]*p[2] / sum で最近傍点が求まる
//	p_qベクトルたちが張る形状の体積(or長さor面積)．4点から作れる16通りについて

/*
      a   |
     /    | v = (1-k) a + k b
    /     | va = (1-k)aa + kba = vb = (1-k)ab + kbb
   /v     | -kaa + kba + kab - kbb = -aa + ab
  b       | k = a^2-ab / (a^2+b^2-2ab), 1-k = b^2-ab / (a^2+b^2-2ab)
----------+ なので， k = a^2-ab を と (k-1) = b^2-abを記録しておく	*/


inline void CalcDet() {
	static double dotp[4][4];	//	p_q[i] * p_q[j] 

	//	新しく増えた点(lastPoint)について，内積を計算
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<=1){
		if (usedPoints & curPoint) 
			dotp[i][lastPoint] = dotp[lastPoint][i] = p_q[i] * p_q[lastPoint];
	}
	dotp[lastPoint][lastPoint] = p_q[lastPoint] * p_q[lastPoint];

	det[lastUsed][lastPoint] = 1;
	for (int j = 0, sj = 1; j < 4; ++j, sj <<= 1) {
		if (usedPoints & sj) {		
			int s2 = sj|lastUsed;	//	新しく増えた点について係数の計算
			det[s2][j] = dotp[lastPoint][lastPoint] - dotp[lastPoint][j];	//	a^2-ab
			det[s2][lastPoint] = dotp[j][j] - dotp[j][lastPoint];			//	b^2-ab
			for (int k = 0, sk = 1; k < j; ++k, sk <<= 1) {	//	3点の場合
				if (usedPoints & sk) {
					int s3 = sk|s2;
					det[s3][k] = det[s2][j] * (dotp[j][j] - dotp[j][k]) + 
						det[s2][lastPoint] * (dotp[lastPoint][j] - dotp[lastPoint][k]);
					det[s3][j] = det[sk|lastUsed][k] * (dotp[k][k] - dotp[k][j]) + 
						det[sk|lastUsed][lastPoint] * (dotp[lastPoint][k] - dotp[lastPoint][j]);
					det[s3][lastPoint] = det[sk|sj][k] * (dotp[k][k] - dotp[k][lastPoint]) + 
						det[sk|sj][j] * (dotp[j][k] - dotp[j][lastPoint]);
				}
			}
		}
	}
	if (allUsedPoints == 15) {	//	4点の場合
		det[15][0] =	det[14][1] * (dotp[1][1] - dotp[1][0]) + 
						det[14][2] * (dotp[2][1] - dotp[2][0]) + 
						det[14][3] * (dotp[3][1] - dotp[3][0]);
		det[15][1] =	det[13][0] * (dotp[0][0] - dotp[0][1]) + 
						det[13][2] * (dotp[2][0] - dotp[2][1]) + 
						det[13][3] * (dotp[3][0] - dotp[3][1]);
		det[15][2] =	det[11][0] * (dotp[0][0] - dotp[0][2]) + 
						det[11][1] * (dotp[1][0] - dotp[1][2]) +	
						det[11][3] * (dotp[3][0] - dotp[3][2]);
		det[15][3] =	det[ 7][0] * (dotp[0][0] - dotp[0][3]) + 
						det[ 7][1] * (dotp[1][0] - dotp[1][3]) + 
						det[ 7][2] * (dotp[2][0] - dotp[2][3]);
	}	
}

//	最近傍点が渡したSimplexの中にあるかどうか．なければ false
inline bool IsVaildPoint(int s) {
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (allUsedPoints & curPoint) {
			if (s & curPoint) { if (det[s][i] <= 0) return false; }
			else if (det[s|curPoint][i] > 0) return false;	//	>eplisionにした方がよいかも
		}
	}
	return true;
}

//	係数から，最近傍点 v を計算
inline void CalcVector(int usedPoints, Vec3d& v) {
	double sum = 0;
	v = Vec3d(0, 0, 0);
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (usedPoints & curPoint) {
			sum += det[usedPoints][i];
			v += p_q[i] * det[usedPoints][i];
		}
	}
	v *= 1 / sum;
}

//	係数から，最近傍点と，元の2つの図形上での，その点の位置を計算
inline void CalcPoints(int usedPoints, Vec3d& p1, Vec3d& p2) {
	double sum = 0;
	p1 = Vec3d(0.0, 0.0, 0.0);
	p2 = Vec3d(0.0, 0.0, 0.0);
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (usedPoints & curPoint) {
			sum += det[usedPoints][i];
			p1 += p[i] * det[usedPoints][i];
			p2 += q[i] * det[usedPoints][i];
		}
	}
	if (sum){
		double s = 1 / sum;
		p1 *= s;
		p2 *= s;
	}else{
		p1 = p[0];
		p2 = q[0];
	}
}

//	
inline bool CalcClosest(Vec3d& v) {
	CalcDet();
	for (int s = usedPoints; s; --s) {
		if ((s & usedPoints) == s) {		//	現在の形状から頂点を減らしていったものについて，
			if (IsVaildPoint(s|lastUsed)) {	//	中に最近傍点があるSimplexが見つかったら
				usedPoints = s|lastUsed;
				CalcVector(usedPoints, v);	//	最近傍点を計算して返す．
				return true;
			}
		}
	}
	if (IsVaildPoint(lastUsed)) {			//	最後に見つけた点の真上だったら
		usedPoints = lastUsed;
		v = p_q[lastPoint];
		return true;
	}
	return false;
}

//	縮退している場合＝4点のうちいくつかが同じ位置にあるとか
inline bool IsDegenerate(const Vec3d& w) {
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1){
//	hase	2003.10.24
//		if ((allUsedPoints & curPoint) && p_q[i] == w) return true;
		if ((allUsedPoints & curPoint) && (p_q[i]-w).square() < 1e-6){
//			DSTR << "T:" << (p_q[i]-w).norm() << p_q[i] << w << std::endl;
			return true;
		}
	}
	return false;
}

bool FindCommonPoint(const CDConvex* a, const CDConvex* b,
					 const Posed& a2w, const Posed& b2w,
					 Vec3d& v, Vec3d& pa, Vec3d& pb) {
	Vec3d w;

	usedPoints = 0;
	allUsedPoints = 0;

	do {
		lastPoint = 0;
		lastUsed = 1;
		while (usedPoints & lastUsed) { ++lastPoint; lastUsed <<= 1; }
		p[lastPoint] = a->Support(a2w.Ori().Conjugated() * (-v));
		q[lastPoint] = b->Support(b2w.Ori().Conjugated() * v);
		w = a2w * p[lastPoint]  -  b2w * q[lastPoint];
		if (v*w > 0) return false;
		if (IsDegenerate(w)) return false;
		p_q[lastPoint] = w;
		allUsedPoints = usedPoints|lastUsed;
		if (!CalcClosest(v)) return false;
	} while ( usedPoints < 15 && !(v.square() < 1e-10f) ) ;
	CalcPoints(usedPoints, pa, pb);
	return true;
}


int ContFindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& r, Vec3d& normal, Vec3d& pa, Vec3d& pb){
	#define XY()	sub_vector( PTM::TSubVectorDim<0,2>() )
	const double epsilon = 1e-6;
	const double epsilon2 = epsilon*epsilon;

	//	返り値の座標系の修正ができてない
		
	Quaterniond w2z;
	Vec3d u = -r.unit();	//	u: 物体ではなく原点の速度の向きなので - がつく．
	if (u.Z() < 1-1e-6){
		Matrix3d matW2z = Matrix3d::Rot(u, Vec3f(0,0,1), 'z');
		w2z.FromMatrix(matW2z);
		w2z = w2z.Inv();
		DSTR << "VelLocal:" << w2z * u << std::endl;
	}
	Posed a2z;
	a2z.Ori() = w2z * a2w.Ori();
	a2z.Pos() = w2z * a2w.Pos();
	Posed b2z;
	b2z.Ori() = w2z * b2w.Ori();
	b2z.Pos() = w2z * b2w.Pos();

	Vec3d w[3], v[3], p[3], q[3];

	//	スタートアップ
	//	w0を求める
	int id = 0;
	v[id] = Vec3d(0,0,1);

#define CalcSupport(v, p, q, w)										\
/*	DSTR << "v on a: " << a2z.Ori().Conjugated() * -v << std::endl;	\
	DSTR << "v in b: " << b2z.Ori().Conjugated() * v << std::endl;	\
*/	p = a->Support(a2z.Ori().Conjugated() * (v));					\
	q = b->Support(b2z.Ori().Conjugated() * -(v));					\
	w = b2z * (q) - a2z * (p);										\
/*	DSTR << "v:" << v << " w:" << w;								\
	DSTR << " p:" << a2z*(p) << " q:" << b2z*(q) << std::endl;		\
*/


	CalcSupport(v[id], p[id], q[id], w[id]);

	//	w1を求める
	id = 1;
	v[id] = Vec3d(w[0].X(), w[0].Y(), 0);
	if (v[id].XY().square() < epsilon2){	//	w0 = 衝突点
		normal = u;
		pa = p[0]; pb = q[0];
		return 1;
	}
	CalcSupport(v[id], p[id], q[id], w[id]);
	if (w[id].XY() * v[id].XY() > 0){	//	w[id]の外側にOがあるので触ってない
		return false;
	}
	
	//	w0-w1上の点で，oからの最近傍点を求める．
	id = 2;
	Vec2d va = w[0].XY();
	Vec2d vb = w[1].XY();
	Vec2d l = vb - va;
	assert(l.square() >= epsilon2);	//	w0=w1ならば，すでに抜けているはず．
	double ll_inv = 1/l.square();
	v[id].XY() = vb*l*ll_inv * va  -  va*l*ll_inv * vb;
	v[id].Z() = 0;
	if (v[id].square() < epsilon2){
		v[id] = 0.5 * (v[0] + v[1]);
	}
	//	サポートを求める．
	CalcSupport(v[id], p[id], q[id], w[id]);

	//	繰り返し
	//	はみ出しチェック
	Vec2d seg[2];
	seg[0] = w[0].XY()-w[2].XY();
	seg[1] = w[1].XY()-w[2].XY();
	int segIds[2] = {0,1};
	if (seg[0] % seg[1]<0){
		segIds[0] = 1;
		segIds[1] = 0;
	}

	while(1){
		int idUse=-1;
		bool bInside = true;
		if (seg[segIds[0]] % -w[2].XY() < 0){	//	はみ出しチェック
			idUse = segIds[0];
			w[segIds[1]] = w[2];
			v[segIds[1]] = v[2];
			Vec3d va = w[segIds[0]];
			Vec3d vb = w[2];
			Vec3d l = vb-va;
			double ll_inv = 1/l.square();
			v[2] = vb*l*ll_inv * va  -  va*l*ll_inv * vb;
			bInside = false;
		}
		if(seg[segIds[1]] % -w[2].XY() > 0){	//	はみ出しチェック
			assert(bInside);
			idUse = segIds[1];
			w[segIds[0]] = w[2];
			v[segIds[0]] = v[2];
			Vec2d va = w[segIds[1]].XY();
			Vec2d vb = w[2].XY();
			Vec2d l = vb-va;
			double ll_inv = 1/l.square();
			v[2] = vb*l*ll_inv * va  -  va*l*ll_inv * vb;
			bInside = false;
		}
		if (!bInside){	//	はみ出していたら三角形を更新
			Vec3d wNew;
			CalcSupport(v[2], p[2], q[2], wNew);
			if (wNew.XY() * v[2].XY() > 0){	//	w[2]の外側にOがあるので触ってない
				return false;
			}
			if( (wNew.XY()-w[idUse].XY()).square() < epsilon2 || (wNew.XY()-w[2].XY()).square() < epsilon2){
				//	同じw: 辺の更新なし＝Oは辺の外側
				return false;
			}
			w[2] = wNew;
		}else{	//	中に入っていれば次に進む
			break;
		}
	}
	//	原点は△の内部
	while(1){
		//	頂点の並び順をそろえる．
		Vec3d sTri = (w[2]-w[0]) % (w[1]-w[0]);
		double sDir = sTri.Z();
		if (sDir < 0){		//	逆向き
			std::swap(w[1], w[2]);
			std::swap(p[1], p[2]);
			std::swap(q[1], q[2]);
			sTri *= -1;
			sDir *= -1;
		}
		//	面積0=線分の場合
		if (sDir <= epsilon){
			int seg[3];	//	2は使わない頂点
			if (w[0].XY() * w[1].XY() > 0){
				seg[0] = 2;
				if (w[0].square() < w[1].square()){
					seg[1] = 0;
					seg[2] = 1;
				}else{
					seg[1] = 1;
					seg[2] = 0;
				}
			}else if (w[0].XY() * w[2].XY() > 0){
				seg[0] = 1;
				if (w[0].square() < w[2].square()){
					seg[1] = 0;
					seg[2] = 2;
				}else{
					seg[1] = 2;
					seg[2] = 0;
				}
			}else{
				seg[0] = 0;
				if (w[1].square() < w[2].square()){
					seg[1] = 1;
					seg[2] = 2;
				}else{
					seg[1] = 2;
					seg[2] = 1;
				}
			}
			Vec3d va = w[seg[0]];
			Vec3d vb = w[seg[1]];
			Vec3d l = vb-va;
			double ll_inv = 1/l.square();
			double ka = vb*l*ll_inv;
			double kb = -va*l*ll_inv;
			v[seg[2]] = ka * va  +  kb * vb;
			CalcSupport(v[seg[2]], p[seg[2]], q[seg[2]], w[seg[2]]);
			if ((w[seg[2]]-w[seg[0]]).square() < epsilon2 || (w[seg[2]]-w[seg[1]]).square() < epsilon2){
				ka = w[seg[1]].XY().norm();
				kb = w[seg[2]].XY().norm();
				double l = ka+kb;
				ka /= l;
				kb /= l;
				pa = ka*p[seg[0]] + kb*p[seg[1]];
				pb = ka*q[seg[0]] + kb*q[seg[1]];
				normal = w2z.Conjugated() * v[seg[2]].unit();
				return 2;
			}
		}else{	//	まっとうな三角形
			Vec3d vNew = sTri;
			Vec3d pNew, qNew, wNew;
			CalcSupport(vNew, pNew, qNew, wNew);
			for(int i=0; i<3;++i){
				if ( (wNew.XY()-w[i].XY()).square() < epsilon2){
					normal = vNew.unit();
					Vec3d goal;
					goal.Z() = w[0]*normal / normal.Z();
					Matrix3d m;
					m.Ex() = w[0];
					m.Ey() = w[1];
					m.Ez() = w[2];
					Vec3d k = m.inv() * goal;
					pa = k.x*p[0] + k.y*p[1] + k.z*p[2];
					pb = k.x*q[0] + k.y*q[1] + k.z*q[2];
					normal = w2z.Conjugated() * normal;
					DSTR << "ql:" << q[0] << q[1] << q[2] << std::endl;
					DSTR << "k:" << k.x+k.y+k.z << k << std::endl;
					return 3;
				}
			}				
			
			double sSubTri[3];
			double ow[3];
			int selection=-1;
			double cross=0;
			for(int i=0; i<3;++i){
				sSubTri[i] = (w[(i+1)%3].XY() - wNew.XY()) % (w[i].XY() - wNew.XY());
				ow[i] =	-wNew.XY() % w[i].XY();
			}
			double sNewTri=0;
			for(int i=0; i<3;++i){
				int iinc = (i+1)%3;
				if (sSubTri[i] > epsilon){
					double s = min(ow[i], -ow[iinc]);
					if (s > sNewTri){
						s = sNewTri;
						selection = i;
					}
				}
			}
			assert(selection>=0);
			int replace = (selection+2)%3;
			v[replace] = vNew;
			w[replace] = wNew;
			p[replace] = pNew;
			q[replace] = qNew;
		}
	}
}

void FindClosestPoints(const CDConvex* a, const CDConvex* b,
					  const Posed& a2w, const Posed& b2w,
					  Vec3d& pa, Vec3d& pb) {
	Vec3d v; 				//	現在の，AとBのミンコスキー和の形状中の1点
	v = a2w * a->Support(Vec3d()) - b2w * b->Support(Vec3d());	
	double dist = v.norm();	//	原点からの距離
	Vec3d w;				//	サポートポイント
	double maxSupportDist = 0.0f;

	usedPoints = 0;
	allUsedPoints = 0;

	while (usedPoints < 15 && dist > absError) {
		lastPoint = 0;
		lastUsed = 1;
		while (usedPoints & lastUsed) { ++lastPoint; lastUsed <<= 1; }
		p[lastPoint] = a->Support(a2w.Ori().Conjugated() * (-v));
		q[lastPoint] = b->Support(b2w.Ori().Conjugated() * v);
		w = a2w * p[lastPoint]  -  b2w * q[lastPoint];
		double supportDist = w*v/dist;
		if (maxSupportDist < supportDist) maxSupportDist= supportDist;
		if (dist - maxSupportDist <= dist * relError) break;
		if (IsDegenerate(w)) break;
		p_q[lastPoint] = w;
		allUsedPoints = usedPoints|lastUsed;
		if (!CalcClosest(v)) break;
		dist = v.norm();
	}
	CalcPoints(usedPoints, pa, pb);
}

}
