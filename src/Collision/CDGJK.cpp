/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/*	このファイルのライセンスについての注意
	このソースは，一部 SOLID (Software Library for Interference Detection) 2.0
	http://www.win.tue.nl/~gino/solid の src/Convex.cpp を参考に書いています．

	作者は，このソースがSolid2.0の一部の派生物ではないと信じています．
	しかし，似ている箇所があります．もし派生物だと認定された場合，
	ライセンスがGPLとなります．ご注意ください．
*/

#include "Collision.h"
#include <Foundation/Scene.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
const double sqEpsilon = 1e-4;
const double epsilon = 1e-8;
const double epsilon2 = epsilon*epsilon;

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

///	det(長さ・面積・体積)の計算．垂線の足が頂点を何:何に内分するかも計算
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
		if ((allUsedPoints & curPoint) && (p_q[i]-w).square() < epsilon2) return true;
	}
	return false;
}

bool FindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, Vec3d& v, Vec3d& pa, Vec3d& pb) {
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
	} while ( usedPoints < 15 && !(v.square() < epsilon2) ) ;
	CalcPoints(usedPoints, pa, pb);
	return true;
}


struct CDGJKIds{
	char i[3];		//	頂点のID
	char nVtx;		//	使用する頂点の数
	double dist;
	Vec3d normal;
	double k[4];
	CDGJKIds(){
		i[0] = i[1] = i[2] = -1;
		nVtx = -1;
		dist = -1;
	}
};
const char vacants[] = {
	0, 1, 0, 2, 0, 1, 0, 3,
	0, 1, 0, 2, 0, 1, 0, 4,
};
inline char VacantId(char a, char b){
	char bits = (1<<a) | (1<<b);
	return vacants[bits];
}
inline char VacantId(char a, char b, char c){
	char bits = (1<<a) | (1<<b) | (1<<c);
	return vacants[bits];
}
#define XY()	sub_vector( PTM::TSubVectorDim<0,2>() )
#define CalcSupport(v, n)										\
	p[n] = a->Support(a2z.Ori().Conjugated() * (v));			\
	q[n] = b->Support(b2z.Ori().Conjugated() * -(v));			\
	w[n] = b2z * (q[n]) - a2z * (p[n]);							\

int ContFindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, Vec3d& range, Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist){
	//	range が+Zになるような座標系を求める．
	Quaterniond w2z;
	double endLength = range.norm();
	range /= endLength;
	Vec3d u = -range;	//	u: 物体ではなく原点の速度の向きなので - がつく．
	if (u.Z() < -1+epsilon){
		w2z = Quaterniond::Rot(Rad(180), 'x');
	}else if (u.Z() < 1-epsilon){
		Matrix3d matW2z = Matrix3d::Rot(u, Vec3f(0,0,1), 'z');
		w2z.FromMatrix(matW2z);
		w2z = w2z.Inv();
	}
	Posed a2z;
	a2z.Ori() = w2z * a2w.Ori();
	a2z.Pos() = w2z * a2w.Pos();
	Posed b2z;
	b2z.Ori() = w2z * b2w.Ori();
	b2z.Pos() = w2z * b2w.Pos();

	
	//	GJKと似た方法で，交点を求める
	
	static CDGJKIds ids[2];
	static Vec3d w[4], p[4], q[4];
	CDGJKIds* cur = ids;
	CDGJKIds* last = ids+1;

	//	w0を求める
	CalcSupport(Vec3d(0,0,1), 0);
	if (w[0].Z() > endLength) return -1;	//	range内では接触しないが，将来接触するかもしれない．
	if (w[0].Z() < 0){						//	反対側のsupportを求めてみて，範囲外か確認
		CalcSupport(Vec3d(0,0,-1), 3);
		if (w[3].Z() <0) return -2;			//	range内では接触しないが，過去(後ろに延長すると)接触していたかもしれない．
	}
	//	w1を求める
	Vec3d v(w[0].X(), w[0].Y(), 0);
	if (v.XY().square() < epsilon2){	//	w0=衝突点の場合
		normal = u.unit();
		pa = p[0]; pb = q[0];
		dist = w[0].Z();
		return 1;
	}
	CalcSupport(v, 1);
	if (w[1].XY() * v.XY() > 0) return 0;	//	w[1]の外側にOがあるので触ってない
	
	
	//	w[0]-w[1]-w[0] を三角形と考えてスタートして，oが三角形の内部に入るまで繰り返し
	cur->i[0] = 1;	//	新しい頂点
	cur->i[1] = 0;	//	もとの線分
	cur->i[2] = 0;	//	もとの線分
	while(1){
		double s = w[cur->i[0]].XY() ^ w[cur->i[1]].XY();
		if (s > epsilon){		//	1-0からはみ出している
			Vec2d l = w[cur->i[1]].XY() - w[cur->i[0]].XY();
			assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
			double ll_inv = 1/l.square();
			v.XY() = (w[cur->i[1]].XY()*l*ll_inv) * w[cur->i[0]].XY() - (w[cur->i[0]].XY()*l*ll_inv) * w[cur->i[1]].XY();
			v.Z() = 0;
			cur->i[2] = cur->i[0];
			cur->i[0] = VacantId(cur->i[1], cur->i[2]);
		}else{
			s = w[cur->i[2]].XY() ^ w[cur->i[0]].XY();
			if (s > epsilon){	//	0-2からはみ出している
				Vec2d l = w[cur->i[2]].XY() - w[cur->i[0]].XY();
				assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
				double ll_inv = 1/l.square();
				v.XY() = (w[cur->i[2]].XY()*l*ll_inv) * w[cur->i[0]].XY() - (w[cur->i[0]].XY()*l*ll_inv) * w[cur->i[2]].XY();
				v.Z() = 0;
				cur->i[1] = cur->i[0];
				cur->i[0] = VacantId(cur->i[1], cur->i[2]);
			}else{				//	内側
				break;
			}
		}
		CalcSupport(v, cur->i[0]);
		if (w[cur->i[0]].XY() * v.XY() > -epsilon2){	//	0の外側にoがあるので触ってない
			return 0;
		}
		if(	(w[cur->i[0]].XY()-w[cur->i[1]].XY()).square() < epsilon2 || 
			(w[cur->i[2]].XY()-w[cur->i[1]].XY()).square() < epsilon2){
			return 0;								//	同じw: 辺の更新なし＝Oは辺の外側
		}
	}
	last->nVtx = 0;
	//	三角形 cur 0-1-2 の中にoがある．	cur 0が最後に更新した頂点w
	while(1){
		static Vec3d s;		//	三角形の有向面積
		s = (w[cur->i[1]]-w[cur->i[0]]) % (w[cur->i[2]]-w[cur->i[0]]);
		//	頂点の並び順をそろえる．
		if (s.Z() < 0){		//	逆向き
			std::swap(cur->i[1], cur->i[2]);
			s *= -1;
		}
		if (s.Z() < epsilon){	//	線分になる場合
			cur->nVtx = 2;		//	使うのは2点，どちらの2点を使うか判定する．
			double ip1 = w[cur->i[0]].XY() * w[cur->i[1]].XY();
			double ip2 = w[cur->i[0]].XY() * w[cur->i[2]].XY();
			if (ip1 < epsilon && ip2 < epsilon){	//	0-1も0-2もoを含む
				cur->k[cur->i[0]] = w[cur->i[0]].XY().norm();
				cur->k[cur->i[1]] = w[cur->i[1]].XY().norm();
				cur->dist = cur->k[cur->i[0]]*w[cur->i[0]].Z() + cur->k[cur->i[1]]*w[cur->i[1]].Z();
				double l1 = cur->k[cur->i[0]] + cur->k[cur->i[1]];
				cur->dist /= l1;
				
				cur->k[cur->i[2]] = w[cur->i[1]].XY().norm();
				double d2 = cur->k[cur->i[0]]*w[cur->i[0]].Z() + cur->k[cur->i[2]]*w[cur->i[2]].Z();
				double l2 = cur->k[cur->i[0]] + cur->k[cur->i[2]];
				d2 /= l2;
				if (d2 < cur->dist){
					std::swap(cur->i[1], cur->i[2]);
					cur->dist = d2;
				}
			}else{
				if (ip2 < ip1) std::swap(cur->i[1], cur->i[2]);
				cur->k[cur->i[0]] = w[cur->i[0]].XY().norm();
				cur->k[cur->i[1]] = w[cur->i[1]].XY().norm();
				cur->dist = cur->k[cur->i[0]]*w[cur->i[0]].Z() + cur->k[cur->i[1]]*w[cur->i[1]].Z();
				double l1 = cur->k[cur->i[0]] + cur->k[cur->i[1]];
				cur->dist /= l1;
			}
			if (last->nVtx){
				double approach = last->normal * (w[cur->i[0]] - w[last->i[0]]);
				if (approach > -epsilon || cur->dist <= last->dist) break;	//	return last
			}
			Vec3d l = w[cur->i[0]] - w[cur->i[1]];
			cur->normal = Vec3d(0,0,1) - l.Z() / l.square() * l;
			cur->normal.unitize();
			std::swap(cur, last);
			cur->i[1] = last->i[0];
			cur->i[2] = last->i[1];
			cur->i[0] = VacantId(cur->i[1], cur->i[2]);
			CalcSupport(last->normal, 0);
		}else{	//	三角形になる場合
			cur->nVtx = 3;		//	使うのは3点．
			cur->normal = s.unit();
			cur->dist = w[cur->i[0]] * cur->normal / cur->normal.Z();
			if (last->nVtx){
				double approach = last->normal * (w[cur->i[0]] - w[last->i[0]]);
				if (approach > -epsilon || (approach > -sqEpsilon && cur->dist <= last->dist)) break;	//	return last;
			}

			int newVtx = VacantId(cur->i[0], cur->i[1], cur->i[2]);
			CalcSupport(cur->normal, newVtx);
			
			//	新しい点 newVtxと元の△の2点wで原点を囲む△を作る
			//	(w[cur->i]-w[newVtx])^(o-w[newVtx]) が + から - に変化するところを探す．
			bool bPlus = false;
			bool bMinus = false;
			double ow[3];
			int i;
			for(i=0; i<3;++i){
				Vec2d wn = w[cur->i[i]].XY()-w[newVtx].XY();
				ow[i] = wn % (-w[newVtx].XY());
				if (ow[i] < 0){
					bMinus = true;
					if (bPlus) break;
				}else if(ow[i] > 0) {
					bPlus= true;
				}
			}
			int replace = -100;
			if (bPlus && bMinus){
				//	+-が出揃った場合：+から-に移った次の頂点が置き換える頂点
				replace = (i+1)%3;
			}else{
				//	+-が出揃わない場合，全部0の場合：w[newVtx]に近いw[i]を置き換え
				double minDist = DBL_MAX;
				for(int i=0; i<3; ++i){
					double d = (w[cur->i[i]].XY() - w[newVtx].XY()).square();
					if (d<minDist){
						minDist = d; replace = i;
					}
				}
			}
			std::swap(cur, last);
			cur->i[0] = newVtx;
			cur->i[1] = last->i[(replace+1)%3];
			cur->i[2] = last->i[(replace+2)%3];
		}
	}
	if (last->nVtx == 2){	//	線分の場合，計算済みのkを使ってpa,pbを計算
		double sumInv = 1 / (last->k[0]+last->k[1]);
		last->k[0] *= sumInv; last->k[1] *= sumInv;
		pa = last->k[0] * p[last->i[0]] + last->k[1] * p[last->i[1]];
		pb = last->k[0] * q[last->i[0]] + last->k[1] * q[last->i[1]];
	}else{					//	三角形の場合，kの計算をしていないのでここで計算
		Matrix2d m;
		m.Ex() = w[last->i[0]].XY()-w[last->i[1]].XY();
		m.Ey() = w[last->i[0]].XY()-w[last->i[2]].XY();
		Vec2d k = m.inv() * w[last->i[0]].XY();
		double kz = 1-k.x-k.y;
		pa = k.x*p[last->i[1]] + k.y*p[last->i[2]] + kz*p[last->i[0]];
		pb = k.x*q[last->i[1]] + k.y*q[last->i[2]] + kz*q[last->i[0]];
	}
	dist = last->dist;
	normal = w2z.Conjugated() * last->normal;
	return last->nVtx;
}

}
