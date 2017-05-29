/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDQUICKHULL3DIMP_H
#define CDQUICKHULL3DIMP_H

#include "CDQuickHull3D.h"
#include <Base/Affine.h>
#include <Base/BaseDebug.h>
#include <vector>
#include <float.h>
//#include <process.h>  // <- windows dependent !

namespace Spr{;

template <class TVtx>
void CDQHPlane<TVtx>::Clear(){			///<	メモリクリア．使う前に呼ぶ．
	deleted = false;
}
template <class TVtx>
bool CDQHPlane<TVtx>::Visible(TVtx* p){
	//	面が有限の距離にある場合
	Vec3d GetPos = p->GetPos();
	const double eps = 1.0e-10;
	return GetPos*normal > dist + eps;
}
template <class TVtx>
int CDQHPlane<TVtx>::GetVtxID(TVtx* v){
	int i;
	for(i=0; i<3; ++i) if(v == vtx[i]) break;
	return i;
}
template <class TVtx>
void CDQHPlane<TVtx>::CalcNormal(){
	Vec3d pos0 = vtx[0]->GetPos();
	Vec3d pos1 = vtx[1]->GetPos();
	Vec3d pos2 = vtx[2]->GetPos();
	Vec3d a = pos1 - pos0;
	Vec3d b = pos2 - pos0;
	normal = a ^ b;
	assert(normal.norm());
	normal.unitize();
	dist = pos0 * normal;
}
template <class TVtx>
void CDQHPlane<TVtx>::Reverse(){	
	normal = -normal;
	dist = -dist;
	std::swap(vtx[0], vtx[2]);
}
template <class TVtx>
double CDQHPlane<TVtx>::CalcDist(TVtx* v){
	//	精度を考慮して距離を計算
	Vec3d vPos = v->GetPos();
	Vec3d diffMin = vPos - Vec3d(vtx[0]->GetPos());
/*
	Vec3d diff = vPos - Vec3d(vtx[1]->GetPos());
	if (diff.square() < diffMin.square()) diffMin = diff;
	diff = vPos - Vec3d(vtx[2]->GetPos());
	if (diff.square() < diffMin.square()) diffMin = diff;
*/
	return diffMin * normal;
}
template <class TVtx>
void CDQHPlane<TVtx>::Print(std::ostream& os) const {
	os << "Plane:";
	for(int i=0; i<3; ++i){
		os << " " << *vtx[i];
	}
	os << " d:" << dist << " n:" << normal;
	if (deleted) os << " del";
	os << std::endl;
}

template <class TVtx>
std::ostream& operator << (std::ostream& os, const CDQHPlane<TVtx>& pl){
	pl.Print(os);
	return os;
}

template <class TVtx>
unsigned CDQHPlanes<TVtx>::size(){
	return end - begin; 
}
template <class TVtx>
CDQHPlanes<TVtx>::CDQHPlanes(int l):len(l){
	buffer = new CDQHPlane<TVtx>[len];
	Clear();
}
template <class TVtx>
void CDQHPlanes<TVtx>::Clear(){
	begin = buffer;
	end = begin;
	nPlanes = 0;
}
template <class TVtx>
CDQHPlanes<TVtx>::~CDQHPlanes(){
	delete [] buffer;
}
/**	bからeまでの頂点から凸包を作る．使用した頂点はbからvtxBegin，
使用しなかった頂点は，vtxBeginからeに移動する．	
beginからendは頂点を3つ含む面になる．それらの面うち凸包に使われた面
は CDQHPlane::deleted が false になっている．	*/
template <class TVtx>
void CDQHPlanes<TVtx>::CreateConvexHull(TVtx** b, TVtx** e){
	vtxBeginInput = b;
	vtxEndInput = e;
	vtxBegin = b;
	vtxEnd = e;
	//	最初の面を作る
	CreateFirstConvex();
	HULL_DEBUG_EVAL(
		DSTR << "First:" << begin->vtx[0]->GetPos() << begin->vtx[1]->GetPos() << begin->vtx[2]->GetPos() << std::endl;
		if ((begin->vtx[1]->GetPos() - begin->vtx[0]->GetPos()).norm() < 0.001) {
			DSTR << "same error" << std::endl;
		}
		DSTR << begin->dist << begin->normal << std::endl;
	)
	for(CDQHPlane<TVtx>* cur = begin; cur != end; ++cur){
		if (cur->deleted) continue;
		TreatPlane(cur);
		assert(end < buffer+len);
	}
}
	
template <class TVtx>
void CDQHPlanes<TVtx>::Print(std::ostream& os) const {
	int num=0;
	for(const CDQHPlane<TVtx>* it = begin; it != end; ++it){
		if (!it->deleted) num ++;
	}
	os << num << " planes." << std::endl;
	for(const CDQHPlane<TVtx>* it = begin; it != end; ++it){
		it->Print(os);
	}
}

template <class TVtx>
void CDQHPlanes<TVtx>::CreateFirstConvex(){
	CDQHPlanes& planes = *this;
	double xMin, xMax;
	TVtx** it;
	TVtx**  xMinVtx;
	TVtx**  xMaxVtx;
	xMin = xMax = (*vtxBegin)->GetPos().X();
	xMinVtx = xMaxVtx = vtxBegin;
	//	最大と最小を見つける
	for(it = vtxBegin+1; it != vtxEnd; ++it){
		double x = (*it)->GetPos().X();
		if (x < xMin){
			xMin = x;
			xMinVtx = it;
		}
		if (x > xMax){
			xMax = x;
			xMaxVtx = it;
		}
	}
	//	最大を最初，最小を最初から2番目に置く
	std::swap(*xMaxVtx, vtxBegin[0]);		//	先頭と最大を入れ替え
	if (xMinVtx == vtxBegin){				//	先頭が最小だったら
		std::swap(*xMaxVtx, vtxBegin[1]);	//	最大だった場所=先頭が入っている場所が最小
	}else{
		std::swap(*xMinVtx, vtxBegin[1]);	//	最小を先頭から2番目と入れ替え
	}
	
	//	2つの頂点が作る辺から一番遠い点を見つける
	Vec3d dir = vtxBegin[1]->GetPos() - vtxBegin[0]->GetPos();
	dir.unitize();
	double maxDist = -1;
	TVtx** third=NULL;
	for(it = vtxBegin+2; it != vtxEnd; ++it){
		Vec3d v0 = Vec3d((*it)->GetPos()) - Vec3d(vtxBegin[0]->GetPos());
		Vec3d v1 = Vec3d((*it)->GetPos()) - Vec3d(vtxBegin[1]->GetPos());
		Vec3d v = v0.square() < v1.square() ? v0 : v1;
		double dist2 = v.square() - Square(v*dir);
		if (dist2 > maxDist){
			maxDist = dist2;
			third = it;
		}
	}
	std::swap(*third, vtxBegin[2]);
	planes.end->Clear();
	planes.end->vtx[0] = vtxBegin[2];
	planes.end->vtx[1] = vtxBegin[1];
	planes.end->vtx[2] = vtxBegin[0];
	planes.end->CalcNormal();
	planes.end++;

	//	裏表を作って最初の凸多面体にする．
	*planes.end = *planes.begin;
	planes.end->Reverse();
	planes.begin->neighbor[0] = planes.end;
	planes.begin->neighbor[1] = planes.end;
	planes.begin->neighbor[2] = planes.end;
	planes.end->neighbor[0] = planes.begin;
	planes.end->neighbor[1] = planes.begin;
	planes.end->neighbor[2] = planes.begin;
	planes.end++;
	//	使用した頂点を頂点リストからはずす．
	vtxBegin += 3;
	nPlanes = 2;
}

/**	horizon を作る． cur が穴をあける面，vtx が新しい頂点．
rv にhorizonを辺に持つ3角形を1つ返す．*/
template <class TVtx>
void CDQHPlanes<TVtx>::FindHorizon(TVtx*& rv, CDQHPlane<TVtx>* cur, TVtx* vtx){
	//	curの削除．隣の面からの参照も消す．
	for(int i=0; i<3; ++i){
		CDQHPlane<TVtx>* next = cur->neighbor[i];
		if (!next) continue;
		for(int j=0; j<3; ++j){
			if (next->neighbor[j] == cur){
				next->neighbor[j] = NULL;
				break;
			}	
		}
	}
	cur->deleted = true;
	nPlanes --;
	//	隣の面について，裏表を判定して処理
	bool bRecurse = false;
	TVtx* rvc=NULL;
	for(int i=0; i<3; ++i){
		CDQHPlane<TVtx>* next = cur->neighbor[i];
		if (!next) continue;
		if (next->Visible(vtx) && nPlanes>1){	//	見える面には穴をあける．
			FindHorizon(rv, next, vtx);
			bRecurse = true;
		}else{
			//	地平線を作る頂点にこの面を登録する．
			cur->vtx[i]->horizon = next;
			rvc = cur->vtx[i];
		}
	}
	if (!bRecurse){
		rv = rvc;
	}
}
/**	頂点とhorizonの間にコーンを作る．*/
template <class TVtx>
void CDQHPlanes<TVtx>::CreateCone(TVtx* firstVtx, TVtx* top){
	CDQHPlanes& planes = *this; 
	TVtx* curVtx = firstVtx;
	CDQHPlane<TVtx>* curHorizon = firstVtx->horizon;
	HULL_DEBUG_EVAL( std::cout << "Horizon:" << *curVtx; )
	//	最初の面を張る
	int curVtxID = curHorizon->GetVtxID(curVtx);
	int prevVtxID = (curVtxID+2)%3;
	//	面の作成
	planes.end->Clear();
	planes.end->vtx[0] = curHorizon->vtx[curVtxID];
	planes.end->vtx[1] = curHorizon->vtx[prevVtxID];
	planes.end->vtx[2] = top;
	planes.end->CalcNormal();
	//	curHorizonと接続
	planes.end->neighbor[0] = curHorizon;
	curHorizon->neighbor[prevVtxID] = planes.end;
	CDQHPlane<TVtx>* firstPlane = planes.end;
	//	面の作成完了
	planes.end ++;
	nPlanes ++;
	
	//	curHorizon の隣りの3角形を見つけて，curHorizonを更新
	curVtx = curHorizon->vtx[prevVtxID];
	curHorizon = curVtx->horizon;

	//	2番目以降を張る
	while(1){
		HULL_DEBUG_EVAL(std::cout << *curVtx;);
		int curVtxID = curHorizon->GetVtxID(curVtx);
		int prevVtxID = (curVtxID+2)%3;
		//	面の作成
		planes.end->Clear();
		planes.end->vtx[0] = curHorizon->vtx[curVtxID];
		planes.end->vtx[1] = curHorizon->vtx[prevVtxID];
		planes.end->vtx[2] = top;
		planes.end->CalcNormal();
		//	curHorizonと接続
		planes.end->neighbor[0] = curHorizon;
		curHorizon->neighbor[prevVtxID] = planes.end;
		//	1つ前に作った面と今作った面を接続
		planes.end[-1].neighbor[1] = planes.end;
		planes.end->neighbor[2] = planes.end-1;
		//	面の作成完了
		planes.end ++;
		if (planes.end - planes.begin > len-1){
			DSTR << "too many planes:" << std::endl;
			for(TVtx** p = vtxBeginInput; p != vtxEndInput; ++p){
				DSTR << (*p)->GetPos() << std::endl;
			}
			DSTR << (int)(vtxEndInput - vtxBeginInput) << " vertices." << std::endl;

			curHorizon = firstVtx->horizon;
			for(TVtx** p = vtxBeginInput; p != vtxEndInput; ++p){
				if (firstVtx == *p){
					DSTR << (int)(p - vtxBeginInput) << " ";
				}
			}
			for(int i=0; i<len; ++i){
				curVtxID = curHorizon->GetVtxID(curVtx);
				prevVtxID = (curVtxID+2)%3;
				curVtx = curHorizon->vtx[prevVtxID];
				curHorizon = curVtx->horizon;
				for(TVtx** p = vtxBeginInput; p != vtxEndInput; ++p){
					if (curVtx == *p){
						DSTR << int(p - vtxBeginInput) << " ";
					}
				}
			}
			DSTR << std::endl;
			exit(0);
		}
		nPlanes ++;
		
		//	curHorizon の隣りの3角形を見つけて，curHorizonを更新
		curVtx = curHorizon->vtx[prevVtxID];
		if (curVtx == firstVtx) break;
		curHorizon = curVtx->horizon;
	}
	HULL_DEBUG_EVAL( std::cout << std::endl;);
	//	最後の面とfirstPlaneを接続
	firstPlane->neighbor[2] = planes.end-1;
	planes.end[-1].neighbor[1] = firstPlane;
}	
/**	一番遠くの頂点を見つける．見つけたらそれを頂点リストからはずす	*/
template <class TVtx>
bool CDQHPlanes<TVtx>::FindFarthest(CDQHPlane<TVtx>* plane){
	TVtx** maxVtx=NULL;
	double maxDist = HULL_EPSILON;
	for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
		double dist = plane->CalcDist(*it);
		if (dist > maxDist){
			maxDist = dist; 
			maxVtx = it;
		}
	}
	if (maxVtx){
		std::swap(*vtxBegin, *maxVtx);
		vtxBegin++;
#ifdef _DEBUG
		if (	vtxBegin[-1]->GetPos() == plane->vtx[0]->GetPos()
			||	vtxBegin[-1]->GetPos() == plane->vtx[1]->GetPos()
			||	vtxBegin[-1]->GetPos() == plane->vtx[2]->GetPos()){
			DSTR << "Error: same position." << std::endl;
			for(int i=0; i<3; ++i){
				DSTR << "V" << i << ": " << plane->vtx[i]->GetPos() << std::endl;
			}
			DSTR << "P : " << vtxBegin[-1]->GetPos() << std::endl;
			assert(0);
		}
#endif
		return true;
	}
	return false;
}
/*	外側 内側 の順に並べる．
	外側の終わり＝内側の始まりが inner	
	面の法線が内側を向いている．(逆向きの面)	*/
template <class TVtx>
TVtx** CDQHPlanes<TVtx>::DivideByPlaneR(CDQHPlane<TVtx>* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = HULL_EPSILON * plane->dist;
	while(start != end){
		double d = -plane->CalcDist(*start);
		if (d <= INNER_DISTANCE){	//	内側の場合は後ろに移動
			-- end;
			std::swap(*end, *start);
		}else{
			++ start;
		}
	}
	return start;
}
/*	外側 内側 の順に並べる．
	外側の終わり＝内側の始まりが inner．
	面の法線は外側を向いている．*/
template <class TVtx>
TVtx** CDQHPlanes<TVtx>::DivideByPlane(CDQHPlane<TVtx>* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = HULL_EPSILON * plane->dist;
	while(start != end){
		double d = plane->CalcDist(*start);
		if (d <= INNER_DISTANCE){	//	内側の場合は後ろに移動
			-- end;
			std::swap(*end, *start);
		}else{
			++ start;
		}
	}
	return start;
}
/**	一つの面に対する処理を行う．一番遠くの頂点を見つけ，
地平線を調べ，コーンを作り，内部の頂点をはずす．*/
template <class TVtx>
void CDQHPlanes<TVtx>::TreatPlane(CDQHPlane<TVtx>* cur){
	if (!FindFarthest(cur)) return;
	HULL_DEBUG_EVAL(
		DSTR << "Farthest:" << vtxBegin[-1]->GetPos();
		DSTR << " cmp:" << vtxBegin[-1]->GetPos()*cur->normal << " > " << cur->dist << std::endl;
		if (cur->dist < 0){
			DSTR << "faceDist:" << cur->dist << std::endl;
		}
	)
	//	地平線の調査
	TVtx* hor=NULL;
	FindHorizon(hor, cur, vtxBegin[-1]);
	HULL_DEBUG_EVAL(
		if (!hor){
			DSTR << "No Horizon Error!!" << std::endl;
			assert(hor);
		}
	)
	//	コーンの作成
	CDQHPlane<TVtx>* coneBegin = end;
	CreateCone(hor, vtxBegin[-1]);
	CDQHPlane<TVtx>* coneEnd = end;
	//	コーンに閉じ込められる頂点をvtxEndの後ろに移動
	TVtx** inner = DivideByPlaneR(cur, vtxBegin, vtxEnd);
	for(CDQHPlane<TVtx>* it=coneBegin; it!=coneEnd; ++it){
		if (it->deleted) continue;
		HULL_DEBUG_EVAL(
			std::cout << "Inner:";
		for(TVtx** v = inner; v != vtxEnd; ++v){
			std::cout << **v;
		}
		std::cout << std::endl;
		);
		inner = DivideByPlane(it, inner, vtxEnd);
	}
	HULL_DEBUG_EVAL(
		std::cout << "InnerFinal:";
	for(TVtx** v = inner; v != vtxEnd; ++v){
		std::cout << **v;
	}
	std::cout << std::endl;
	);
	
	vtxEnd = inner;
}

template <class TVtx>
std::ostream& operator << (std::ostream& os, const CDQHPlanes<TVtx>& pls){
	pls.Print(os);
	return os;
}
template <class TVtx>
inline std::ostream& operator << (std::ostream& os, const TYPENAME CDQHPlanes<TVtx>::TVtxs& f){
	f.Print(os); return os;
}

inline Vec3f CDQHVtx3DSample::GetPos() const {
	return dir * dist;
}
inline void CDQHVtx3DSample::SetPos(Vec3f p){
	float d = p.norm();
	dir = p / d;
	dist = d;
}
inline void CDQHVtx3DSample::Print(std::ostream& os) const {
//		os << Vtx();
		os << id_ << " ";
}
inline std::ostream& operator << (std::ostream& os, const CDQHVtx3DSample& f){
	f.Print(os); return os;
}

}

#endif
