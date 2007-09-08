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
	論文とソースを公開されている Bergen さんに感謝します．

	長谷川は，このソースがSolid2.0の一部の派生物ではないと信じています．
	しかし，似ている箇所があります．もし派生物だと認定された場合，
	ライセンスがLGPLとなります．ご注意ください．
*/

#include "Collision.h"
#include <Foundation/Scene.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
const double sqEpsilon = 1e-3;
const double epsilon   = 1e-6;  // sが2e-6になることもあった．まだだめかもしれない．（mitake）
const double epsilon2  = epsilon*epsilon;

static Vec3d p[4];			// Aのサポートポイント(ローカル系)
static Vec3d q[4];			// Bのサポートポイント(ローカル系)
static Vec3d p_q[4];		// ミンコスキー和上でのサポートポイント(ワールド系)

const char vacants[] = {
	0, 1, 0, 2, 0, 1, 0, 3,
	0, 1, 0, 2, 0, 1, 0, 4,
};
inline char VacantIdFromId(char a, char b){
	char bits = (1<<a) | (1<<b);
	return vacants[(int)bits];
}
inline char VacantIdFromId(char a, char b, char c){
	char bits = (1<<a) | (1<<b) | (1<<c);
	return vacants[(int)bits];
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
#define XY()	sub_vector( PTM::TSubVectorDim<0,2>() )
#define CalcSupport(v, n)										\
	p[n] = a->Support(a2z.Ori().Conjugated() * (v));			\
	q[n] = b->Support(b2z.Ori().Conjugated() * -(v));			\
	w[n] = b2z * (q[n]) - a2z * (p[n]);

int FASTCALL ContFindCommonPoint(const CDConvex* a, const CDConvex* b,
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
		if (w[3].Z() <0){
			return -2;			//	range内では接触しないが，過去(後ろに延長すると)接触していたかもしれない．
		}
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
		double s = w[(int)cur->i[0]].XY() ^ w[(int)cur->i[1]].XY();
		if (s > epsilon){		//	1-0からはみ出している  epsilon=1e-8だと無限ループ，1e-7でも稀に無限ループ
			Vec2d l = w[(int)cur->i[1]].XY() - w[(int)cur->i[0]].XY();
			assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
			double ll_inv = 1/l.square();
			v.XY() = (w[(int)cur->i[1]].XY()*l*ll_inv) * w[(int)cur->i[0]].XY()
				   - (w[(int)cur->i[0]].XY()*l*ll_inv) * w[(int)cur->i[1]].XY();
			v.Z() = 0;
			cur->i[2] = cur->i[0];
			cur->i[0] = VacantIdFromId(cur->i[1], cur->i[2]);
		}else{
			s = w[(int)cur->i[2]].XY() ^ w[(int)cur->i[0]].XY();
			if (s > epsilon){	//	0-2からはみ出している
				Vec2d l = w[(int)cur->i[2]].XY() - w[(int)cur->i[0]].XY();
				assert(l.square() >= epsilon2);		//	w0=w1ならば，すでに抜けているはず．
				double ll_inv = 1/l.square();
				v.XY() = (w[(int)cur->i[2]].XY()*l*ll_inv) * w[(int)cur->i[0]].XY()
					   - (w[(int)cur->i[0]].XY()*l*ll_inv) * w[(int)cur->i[2]].XY();
				v.Z() = 0;
				cur->i[1] = cur->i[0];
				cur->i[0] = VacantIdFromId(cur->i[1], cur->i[2]);
			}else{				//	内側
				break;
			}
		}
		CalcSupport(v, (int)cur->i[0]);
		if (w[(int)cur->i[0]].XY() * v.XY() > -epsilon2){	//	0の外側にoがあるので触ってない
			return 0;
		}
		if(	(w[(int)cur->i[0]].XY()-w[(int)cur->i[1]].XY()).square() < epsilon2 || 
			(w[(int)cur->i[2]].XY()-w[(int)cur->i[1]].XY()).square() < epsilon2){
			return 0;								//	同じw: 辺の更新なし＝Oは辺の外側
		}
	}
	last->nVtx = 0;
	//	三角形 cur 0-1-2 の中にoがある．	cur 0が最後に更新した頂点w
	while(1){
		static Vec3d s;		//	三角形の有向面積
		s = (w[(int)cur->i[1]]-w[(int)cur->i[0]]) % (w[(int)cur->i[2]]-w[(int)cur->i[0]]);
		//	頂点の並び順をそろえる．
		if (s.Z() < 0){		//	逆向き
			std::swap(cur->i[1], cur->i[2]);
			s *= -1;
		}
		if (s.Z() < epsilon){	//	線分になる場合
			cur->nVtx = 2;		//	使うのは2点，どちらの2点を使うか判定する．
			double ip1 = w[(int)cur->i[0]].XY() * w[(int)cur->i[1]].XY();
			double ip2 = w[(int)cur->i[0]].XY() * w[(int)cur->i[2]].XY();
			if (ip1 < epsilon && ip2 < epsilon){	//	0-1も0-2もoを含む
				cur->k[(int)cur->i[0]] = w[(int)cur->i[0]].XY().norm();
				cur->k[(int)cur->i[1]] = w[(int)cur->i[1]].XY().norm();
				cur->dist = cur->k[(int)cur->i[0]]*w[(int)cur->i[0]].Z()
					      + cur->k[(int)cur->i[1]]*w[(int)cur->i[1]].Z();
				double l1 = cur->k[(int)cur->i[0]] + cur->k[(int)cur->i[1]];
				cur->dist /= l1;
				
				cur->k[(int)cur->i[2]] = w[(int)cur->i[1]].XY().norm();
				double d2 = cur->k[(int)cur->i[0]]*w[(int)cur->i[0]].Z()
					      + cur->k[(int)cur->i[2]]*w[(int)cur->i[2]].Z();
				double l2 = cur->k[(int)cur->i[0]] + cur->k[(int)cur->i[2]];
				d2 /= l2;
				if (d2 < cur->dist){
					std::swap(cur->i[1], cur->i[2]);
					cur->dist = d2;
				}
			}else{
				if (ip2 < ip1) std::swap(cur->i[1], cur->i[2]);
				cur->k[(int)cur->i[0]] = w[(int)cur->i[0]].XY().norm();
				cur->k[(int)cur->i[1]] = w[(int)cur->i[1]].XY().norm();
				cur->dist = cur->k[(int)cur->i[0]]*w[(int)cur->i[0]].Z()
					      + cur->k[(int)cur->i[1]]*w[(int)cur->i[1]].Z();
				double l1 = cur->k[(int)cur->i[0]] + cur->k[(int)cur->i[1]];
				cur->dist /= l1;
			}
			if (last->nVtx){
				double approach = last->normal * (w[(int)cur->i[0]] - w[(int)last->i[0]]);
				if (approach > -epsilon || cur->dist >= last->dist) break;	//	return last
			}
			Vec3d l = w[(int)cur->i[0]] - w[(int)cur->i[1]];
			cur->normal = Vec3d(0,0,1) - l.Z() / l.square() * l;
			cur->normal.unitize();
			std::swap(cur, last);
			cur->i[1] = last->i[0];
			cur->i[2] = last->i[1];
			cur->i[0] = VacantIdFromId(cur->i[1], cur->i[2]);
			CalcSupport(last->normal, 0);
		}else{	//	三角形になる場合
			cur->nVtx = 3;		//	使うのは3点．
			cur->normal = s.unit();
			cur->dist = w[(int)cur->i[0]] * cur->normal / cur->normal.Z();
			if (last->nVtx){
				double approach = last->normal * (w[(int)cur->i[0]] - w[(int)last->i[0]]);
				if (approach > -epsilon || (approach > -sqEpsilon && cur->dist >= last->dist)) break;	//	return last;
			}

			int newVtx = VacantIdFromId(cur->i[0], cur->i[1], cur->i[2]);
			CalcSupport(cur->normal, newVtx);
			
			//	新しい点 newVtxと元の△の2点wで原点を囲む△を作る
			//	(w[cur->i]-w[newVtx])^(o-w[newVtx]) が + から - に変化するところを探す．
			bool bPlus = false;
			bool bMinus = false;
			double ow[3];
			int i;
			for(i=0; i<3;++i){
				Vec2d wn = w[(int)cur->i[i]].XY()-w[newVtx].XY();
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
					double d = (w[(int)cur->i[i]].XY() - w[newVtx].XY()).square();
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
		pa = last->k[0] * p[(int)last->i[0]] + last->k[1] * p[(int)last->i[1]];
		pb = last->k[0] * q[(int)last->i[0]] + last->k[1] * q[(int)last->i[1]];
	}else{					//	三角形の場合，kの計算をしていないのでここで計算
		Matrix2d m;
		m.Ex() = w[(int)last->i[0]].XY()-w[(int)last->i[1]].XY();
		m.Ey() = w[(int)last->i[0]].XY()-w[(int)last->i[2]].XY();
		Vec2d k = m.inv() * w[(int)last->i[0]].XY();
		double kz = 1-k.x-k.y;
		pa = k.x*p[(int)last->i[1]] + k.y*p[(int)last->i[2]] + kz*p[(int)last->i[0]];
		pb = k.x*q[(int)last->i[1]] + k.y*q[(int)last->i[2]] + kz*q[(int)last->i[0]];
	}
	dist = last->dist;
	normal = w2z.Conjugated() * last->normal;
//	HASE_REPORT
/*	DSTR << "nVtx:" << (int)last->nVtx;
	DSTR << "  dist:" << dist << "  cur:" << cur->dist;
	CDBox* box = DCAST(CDBox, b);
	if (box){
		DSTR << "  v:" << box->curPos;
	}
	DSTR << "  ori:" << b2w.Ori() * Vec3f(0,1,0);
	DSTR << std::endl;
*/	
	return last->nVtx;
}


#define USE_NON_CONTINUOUS_DETECTORS
#ifdef USE_NON_CONTINUOUS_DETECTORS
/*	GJKアルゴリズム(FindCommonPoint(), FindClosestPoint())の実装．
	SOLIDのソースを参考にして書いた部分です．
	FindClosestPointは使っていません．
	
	FindCommonPointは，
		CDShapePair::Detect()
		CDContactAnalysis::CalcNormal(CDShapePair* cp)
	で使っています．ペナルティ法を使うと呼び出されます．	*/

//	過去のSupportPointが張る形状(点・線分・3角形，4面体)．最大4点まで．
static int usedBits;		//	下位4ビットが，どの4点で現在の形状が構成されるかを表す．
static int lastId;			//	最後に見つけたSupportPointをしまった場所(0～3)
static int lastBit;			//	lastBit = 1<<lastId (ビットで表したもの）
static int allUsedBits;		//	usedBitsにlastBitを加えたもの
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

	//	新しく増えた点(lastId)について，内積を計算
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<=1){
		if (usedBits & curPoint) 
			dotp[i][lastId] = dotp[lastId][i] = p_q[i] * p_q[lastId];
	}
	dotp[lastId][lastId] = p_q[lastId] * p_q[lastId];

	det[lastBit][lastId] = 1;
	for (int j = 0, sj = 1; j < 4; ++j, sj <<= 1) {
		if (usedBits & sj) {		
			int s2 = sj|lastBit;	//	新しく増えた点について係数の計算
			det[s2][j] = dotp[lastId][lastId] - dotp[lastId][j];	//	a^2-ab
			det[s2][lastId] = dotp[j][j] - dotp[j][lastId];			//	b^2-ab
			for (int k = 0, sk = 1; k < j; ++k, sk <<= 1) {	//	3点の場合
				if (usedBits & sk) {
					int s3 = sk|s2;
					det[s3][k] = det[s2][j] * (dotp[j][j] - dotp[j][k]) + 
						det[s2][lastId] * (dotp[lastId][j] - dotp[lastId][k]);
					det[s3][j] = det[sk|lastBit][k] * (dotp[k][k] - dotp[k][j]) + 
						det[sk|lastBit][lastId] * (dotp[lastId][k] - dotp[lastId][j]);
					det[s3][lastId] = det[sk|sj][k] * (dotp[k][k] - dotp[k][lastId]) + 
						det[sk|sj][j] * (dotp[j][k] - dotp[j][lastId]);
				}
			}
		}
	}
	if (allUsedBits == 15) {	//	4点の場合
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


//	係数から，最近傍点 v を計算
inline void CalcVector(int usedBits, Vec3d& v) {
	double sum = 0;
	v.clear();
	for (int i = 0; i < 4; ++i) {
		if (usedBits & (1<<i)) {
			sum += det[usedBits][i];
			v += p_q[i] * det[usedBits][i];
		}
	}
	v *= 1 / sum;
}

//	係数から，最近傍点と，元の2つの図形上での，その点の位置を計算
inline void CalcPoints(int usedBits, Vec3d& p1, Vec3d& p2) {
	double sum = 0;
	p1.clear();
	p2.clear();
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (usedBits & curPoint) {
			sum += det[usedBits][i];
			p1 += p[i] * det[usedBits][i];
			p2 += q[i] * det[usedBits][i];
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

//	最近傍点を返す．
inline bool CalcClosest(Vec3d& v) {
	CalcDet();
	if (!usedBits){
		usedBits = lastBit;
		v = p_q[lastId];
		return true;
	}
	int simplex[5][4];
	int nSimplex[5];
	const char numVertices[] = {
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4
	};
	int nVtx = numVertices[allUsedBits];
	simplex[nVtx][0] = allUsedBits;
	nSimplex[nVtx] = 1;
	
	for(; nVtx>1; --nVtx){
		for(int sid=0; sid<nSimplex[nVtx]; ++sid){
			int s = simplex[nVtx][sid];
			nSimplex[nVtx-1]=0;
			for(int i = 0; i!=4; ++i){
				int bit = 1<<i;
				if ((s&bit) && det[s][i] <= 0){
					simplex[nVtx-1][nSimplex[nVtx-1]] = s & ~bit;
					++ nSimplex[nVtx-1];
				}
			}
			if (nSimplex[nVtx-1] == 0){
				usedBits = s;
				CalcVector(usedBits, v);	//	最近傍点を計算して返す．			
				return true;
			}
		}
	}
	return false;
}

//	新しい点wが，今までの点と等しい場合
inline bool HasSame(const Vec3d& w) {
	for (int i = 0; i < 4; ++i){
		if ((allUsedBits & (1<<i)) && (p_q[i]-w).square() < epsilon2) return true;
	}
	return false;
}
inline char VacantIdFromBits(char bits){
	return vacants[(int)bits];
}

bool FASTCALL FindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, Vec3d& v, Vec3d& pa, Vec3d& pb) {
	Vec3d w;

	usedBits = 0;
	allUsedBits = 0;
	int count = 0;
	do {
		lastId = VacantIdFromBits(usedBits);
		lastBit = 1 << lastId;

		p[lastId] = a->Support(a2w.Ori().Conjugated() * (-v));
		q[lastId] = b->Support(b2w.Ori().Conjugated() * v);
		w = a2w * p[lastId]  -  b2w * q[lastId];
		if (v*w > 0) return false;			//	原点がsupport面の外側
		if (HasSame(w)) return false;		//	supportが1点に集中＝原点は外にある．
		p_q[lastId] = w;					//	新しい点を代入
		allUsedBits = usedBits|lastBit;//	使用中頂点リストに追加
		if (!CalcClosest(v)) return false;

		count ++;
		if (count > 100){
			Vec3d vtxs[3];
			Vec3d notUsed[3];
			int nUsed=0, nNotUsed=0;
			for(int i=0; i<4; ++i){
				if (usedBits & (1<<i)){
					vtxs[nUsed] = p_q[i];
					nUsed++;
				}else{
					notUsed[nNotUsed] = p_q[i];
					nNotUsed++;
				}
			}
			if (nUsed == 3 && allUsedBits == 15){
				Vec3d n = (vtxs[2]-vtxs[0]) ^ (vtxs[1]-vtxs[0]);
				n.unitize();
				if (n * (notUsed[0] - vtxs[0]) > 0){
					n *= -1;
				}
				double dist = n * (-vtxs[0]);
				DSTR << "n:\t" << nUsed << "Dist:\t" << dist << std::endl;
				DSTR << "v:\t" << v.x << "\t" << v.y << "\t" << v.z << std::endl; 
//				DSTR << "Normal:\t" << n.x << "\t" << n.y << "\t" << n.z << std::endl;
				for(int i=0; i<nUsed; ++i){
					DSTR << "Use:\t" << vtxs[i].x << "\t" << vtxs[i].y << "\t" << vtxs[i].z << std::endl;
				}
				for(int i=0; i<nNotUsed; ++i){
					DSTR << "Not:\t" << notUsed[i].x << "\t" << notUsed[i].y << "\t" << notUsed[i].z << std::endl;
				}
			}else{
				DSTR << "n:\t" << nUsed << std::endl;
				DSTR << "v:\t" << v.x << "\t" << v.y << "\t" << v.z << std::endl;
				for(int i=0; i<nUsed; ++i){
					DSTR << "Use:\t" << vtxs[i].x << "\t" << vtxs[i].y << "\t" << vtxs[i].z << std::endl;
				}
				for(int i=0; i<nNotUsed; ++i){
					DSTR << "Not:\t" << notUsed[i].x << "\t" << notUsed[i].y << "\t" << notUsed[i].z << std::endl;
				}
			}
		}
	} while ( usedBits < 15 && !(v.square() < epsilon2) ) ;
	CalcPoints(usedBits, pa, pb);
	return true;
}


#define USE_FIND_CLOSEST_POINT
#ifdef USE_FIND_CLOSEST_POINT

inline bool IsDegenerate(const Vec3d& w) {
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1){
//		if ((allUsedBits & curPoint) && (p_q[i]-w).square() < 1e-6){
		if ((allUsedBits & curPoint) && (p_q[i]-w).square() < epsilon2){
			return true;
		}
	}
	return false;
}
void FASTCALL FindClosestPoints(const CDConvex* a, const CDConvex* b,
					  const Posed& a2w, const Posed& b2w,
					  Vec3d& pa, Vec3d& pb) {
	Vec3d v; 				
	v = a2w * a->Support(Vec3d()) - b2w * b->Support(Vec3d());	
	double dist = v.norm();	
	Vec3d w;				
	double maxSupportDist = 0.0f;

	usedBits = 0;
	allUsedBits = 0;

	while (usedBits < 15 && dist > epsilon) {
		lastId = 0;
		lastBit = 1;
		while (usedBits & lastBit) { ++lastId; lastBit <<= 1; }
		p[lastId] = a->Support(a2w.Ori().Conjugated() * (-v));
		q[lastId] = b->Support(b2w.Ori().Conjugated() * v);
		w = a2w * p[lastId]  -  b2w * q[lastId];
		double supportDist = w*v/dist;
		if (maxSupportDist < supportDist) maxSupportDist= supportDist;
		if (dist - maxSupportDist <= dist * epsilon) break;
		if (IsDegenerate(w)) break;
		p_q[lastId] = w;
		allUsedBits = usedBits|lastBit;
		if (!CalcClosest(v)) break;
		dist = v.norm();
	}
	CalcPoints(usedBits, pa, pb);
}
#endif
#endif

}
