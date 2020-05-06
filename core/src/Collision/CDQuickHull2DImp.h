/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDQUICKHULL2DIMP_H
#define CDQUICKHULL2DIMP_H

#include "CDQuickHull2D.h"
#include <Base/Affine.h>
#include <Base/BaseDebug.h>
#include <vector>
#include <float.h>

namespace Spr{;

template <class TVtx>
void CDQHLine<TVtx>::Clear(){			///<	メモリクリア．使う前に呼ぶ．
	deleted = false;
}
template <class TVtx>
bool CDQHLine<TVtx>::Visible(TVtx* p){
	Vec2d pos = p->GetPos();
	return pos*normal > dist;
}
template <class TVtx>
int CDQHLine<TVtx>::GetVtxID(TVtx* v){
	int i;
	for(i=0; i<2; ++i) if(v == vtx[i]) break;
	return i;
}
template <class TVtx>
void CDQHLine<TVtx>::CalcNormal(){
	Vec2d pos0 = vtx[0]->GetPos();
	Vec2d pos1 = vtx[1]->GetPos();
	Vec2d a = pos1 - pos0;
	normal = Vec2d(-a.y, a.x);
	assert(normal.norm());
	normal.unitize();
	dist = pos0 * normal;
}
template <class TVtx>
double CDQHLine<TVtx>::CalcDist(TVtx* v){
	Vec2d vPos = v->GetPos();
	Vec2d diffMin = vPos - Vec2d(vtx[0]->GetPos());
	return diffMin * normal;
}
template <class TVtx>
void CDQHLine<TVtx>::Print(std::ostream& os) const {
	os << "Plane:";
	for(int i=0; i<2; ++i){
		os << " " << *vtx[i];
	}
	os << " d:" << dist << " n:" << normal;
	if (deleted) os << " del";
	os << std::endl;
}

template <class TVtx>
std::ostream& operator << (std::ostream& os, const CDQHLine<TVtx>& pl){
	pl.Print(os);
	return os;
}

template <class TVtx>
unsigned CDQHLines<TVtx>::size(){
	return end - begin; 
}
template <class TVtx>
CDQHLines<TVtx>::CDQHLines(int l):epsilon(1e-6), infinite(1e8), len(l){
	buffer = new CDQHLine<TVtx>[len];
	Clear();
}
template <class TVtx>
void CDQHLines<TVtx>::Clear(){
	begin = buffer;
	end = begin;
	nLines = 0;
}
template <class TVtx>
void CDQHLine<TVtx>::Reverse(){	
	normal = -normal;
	dist = -dist;
	std::swap(vtx[0], vtx[1]);
}
template <class TVtx>
CDQHLines<TVtx>::~CDQHLines(){
	delete [] buffer;
}
/**	bからeまでの頂点から凸包を作る．使用した頂点はbからvtxBegin，
使用しなかった頂点は，vtxBeginからeに移動する．	
beginからendは頂点を3つ含む面になる．それらの面うち凸包に使われた面
は CDQHLine::deleted が false になっている．	*/
template <class TVtx>
void CDQHLines<TVtx>::CreateConvexHull(TVtx** b, TVtx** e){
	if (e-b < 2){
		if (e-b == 1){
			end->Clear();
			end->vtx[0] = b[0];
			end->vtx[1] = b[0];
			end->neighbor[0] = end->neighbor[1] = end;
			end++;
		}
		return;
	}
	vtxBeginInput = b;
	vtxEndInput = e;
	vtxBegin = b;
	vtxEnd = e;
	//	最初の面を作る
	if (CreateFirstConvex()){
		HULL_DEBUG_EVAL(
			DSTR << "First:" << begin->vtx[0]->GetPos() << begin->vtx[1]->GetPos() << std::endl;
			if ((begin->vtx[1]->GetPos() - begin->vtx[0]->GetPos()).norm() < 0.001) {
				DSTR << "same error" << std::endl;
			}
			DSTR << begin->dist << begin->normal << std::endl;
		)
		for(CDQHLine<TVtx>* cur = begin; cur != end; ++cur){
			if (cur->deleted) continue;
			TreatPlane(cur);
			assert(end < buffer+len);
		}
	}
}
	
template <class TVtx>
void CDQHLines<TVtx>::Print(std::ostream& os) const {
	int num=0;
	for(const CDQHLine<TVtx>* it = begin; it != end; ++it){
		if (!it->deleted) num ++;
	}
	os << num << " lines." << std::endl;
	for(const CDQHLine<TVtx>* it = begin; it != end; ++it){
		it->Print(os);
	}
}

template <class TVtx>
bool CDQHLines<TVtx>::CreateFirstConvex(){
	CDQHLines& lines = *this;
	double minVal, maxVal;
	//typename TVtxs::iterator it, minVtx, maxVtx;
	TVtx** it, **minVtx, **maxVtx;	
	minVal = maxVal = (*vtxBegin)->GetPos().X();
	minVtx = maxVtx = vtxBegin;
	//	xの最大と最小を見つける
	for(it = vtxBegin+1; it != vtxEnd; ++it){
		double x = (*it)->GetPos().X();
		if (x < minVal){
			minVal = x;
			minVtx = it;
		}
		if (x > maxVal){
			maxVal = x;
			maxVtx = it;
		}
	}
	if (maxVal - minVal < epsilon){	//	もし xの最大・最小が等しかったら
		//	yの最大最小を見つける
		for(it = vtxBegin+1; it != vtxEnd; ++it){
			double x = (*it)->GetPos().Y();
			if (x < minVal){
				minVal = x;
				minVtx = it;
			}
			if (x > maxVal){
				maxVal = x;
				maxVtx = it;
			}
		}
	}
	if (maxVal - minVal < epsilon){	//	どちらも等しい＝1点しかない
		return false;
	}

	//	最大を最初，最小を最初から2番目に置く
	std::swap(*maxVtx, vtxBegin[0]);		//	先頭と最大を入れ替え
	if (minVtx == vtxBegin){				//	先頭が最小だったら
		std::swap(*maxVtx, vtxBegin[1]);	//	最大だった場所=先頭が入っている場所が最小
	}else{
		std::swap(*minVtx, vtxBegin[1]);	//	最小を先頭から2番目と入れ替え
	}

	//	最初の辺を作る
	lines.end->Clear();
	lines.end->vtx[0] = vtxBegin[1];
	lines.end->vtx[1] = vtxBegin[0];
	lines.end->CalcNormal();
	lines.end++;
	
	//	裏表を作って最初の凸多面体にする．
	*lines.end = *lines.begin;
	lines.end->Reverse();
	lines.begin->neighbor[0] = lines.end;
	lines.begin->neighbor[1] = lines.end;
	lines.end->neighbor[0] = lines.begin;
	lines.end->neighbor[1] = lines.begin;
	lines.end++;
	//	使用した頂点を頂点リストからはずす．
	vtxBegin += 2;
	nLines = 2;
	return true;
}

/**	辺curと，その面から一番遠い頂点 top を受け取り，
	curとその周囲の辺を削除し，凸包にtopを含める．
	end[-1], end[-2]が新たに作られた辺になる．	*/
template <class TVtx>
void CDQHLines<TVtx>::CreateCone(CDQHLine<TVtx>* cur, TVtx* top){
	cur->deleted = true;							//	curは削除
	nLines --;
	//	隣の辺を見ていって，頂点からみえる辺は削除する．
	CDQHLine<TVtx>* horizon[2];
	for(int i=0; i<2; ++i){
		horizon[i] = cur->neighbor[i];
		while(horizon[i]->Visible(top) && nLines>1){
			horizon[i]->deleted = true;
			nLines --;
			horizon[i] = horizon[i]->neighbor[i];
		}
	}
	//	削除されなかった辺と頂点の間に辺を作る．
	//	horizon[0]側の辺
	end->Clear();
	horizon[0]->neighbor[1] = end;
	end->vtx[0] = horizon[0]->vtx[1];
	end->vtx[1] = top;
	end->neighbor[0] = horizon[0];
	end->neighbor[1] = end+1;
	end->CalcNormal();
	end++;
	nLines ++;
	//	horizon[1]側の辺
	end->Clear();
	horizon[1]->neighbor[0] = end;
	end->vtx[0] = top;
	end->vtx[1] = horizon[1]->vtx[0];
	end->neighbor[0] = end-1;
	end->neighbor[1] = horizon[1];
	end->CalcNormal();
	end++;
	nLines ++;
}	
/**	一番遠くの頂点を見つける．見つけたらそれを頂点リストからはずす	*/
template <class TVtx>
bool CDQHLines<TVtx>::FindFarthest(CDQHLine<TVtx>* line){
#if 0	
	TVtx** maxVtx=NULL;
	double maxDist = epsilon;
	for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
		double dist = line->CalcDist(*it);
		if (dist > maxDist){
			maxDist = dist; 
			maxVtx = it;
		}
	}
	if (maxVtx){
		std::swap(*vtxBegin, *maxVtx);
		vtxBegin++;
#ifdef _DEBUG
		if (	vtxBegin[-1]->GetPos() == line->vtx[0]->GetPos()
			||	vtxBegin[-1]->GetPos() == line->vtx[1]->GetPos()){
			DSTR << "Error: same position." << std::endl;
			for(int i=0; i<2; ++i){
				DSTR << "V" << i << ": " << line->vtx[i]->GetPos() << std::endl;
			}
			DSTR << "P : " << vtxBegin[-1]->GetPos() << std::endl;
			assert(0);
		}
#endif
		return true;
	}
	return false;
#endif
	
	TVtx** maxVtx=NULL;
	double maxDist = epsilon;

	for(TVtx** it=vtxBegin; it!= vtxEnd; ++it){
		double dist = line->CalcDist(*it);
		if (dist > maxDist){
			maxDist = dist; 
			maxVtx = it;
		}
	}
	if (maxVtx){
		if (maxVtx[0]->GetPos() == line->vtx[0]->GetPos()
			||	maxVtx[0]->GetPos() == line->vtx[1]->GetPos()){
				return false;
		} else {
			std::swap(*vtxBegin, *maxVtx);
			vtxBegin++;
			
#ifdef _DEBUG
			if (vtxBegin[-1]->GetPos() == line->vtx[0]->GetPos()
				||	vtxBegin[-1]->GetPos() == line->vtx[1]->GetPos()){
			
				DSTR << "Error: same position." << std::endl;
				for(int i=0; i<2; ++i){
					DSTR << "V" << i << ": " << line->vtx[i]->GetPos() << std::endl;
				}
				DSTR << "P : " << vtxBegin[-1]->GetPos() << std::endl;
				assert(0);
			}
#endif
			return true;
		}
	}
	return false;
	
}
/*	外側 内側 の順に並べる．
外側の終わり＝内側の始まりが inner	*/
template <class TVtx>
TVtx** CDQHLines<TVtx>::DivideByPlaneR(CDQHLine<TVtx>* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = epsilon * plane->dist;
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
template <class TVtx>
TVtx** CDQHLines<TVtx>::DivideByPlane(CDQHLine<TVtx>* plane, TVtx** start, TVtx** end){
	double INNER_DISTANCE = epsilon * plane->dist;
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
void CDQHLines<TVtx>::TreatPlane(CDQHLine<TVtx>* cur){
	//	一番遠くの頂点の探索
	if (!FindFarthest(cur)) return;
	HULL_DEBUG_EVAL(
		DSTR << "Farthest:" << vtxBegin[-1]->GetPos();
		DSTR << " cmp:" << vtxBegin[-1]->GetPos()*cur->normal << " > " << cur->dist << std::endl;
		if (cur->dist < 0){
			DSTR << "faceDist:" << cur->dist << std::endl;
		}
	)
	//	新しい頂点で凸包を作る．
	CreateCone(cur, vtxBegin[-1]);
	HULL_DEBUG_EVAL(
		if (!hor){
			DSTR << "No Horizon Error!!" << std::endl;
			assert(hor);
		}
	)
	//	注目した辺(cur)と新たな辺(end-2,end-1)によって閉じ込められる頂点をvtxEndの後ろに移動
	TVtx** inner = DivideByPlaneR(cur, vtxBegin, vtxEnd);
	for(CDQHLine<TVtx>* it=end-2; it!=end; ++it){
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
std::ostream& operator << (std::ostream& os, const CDQHLines<TVtx>& pls){
	pls.Print(os);
	return os;
}
template <class CDQHVtx2DSample>
inline std::ostream& operator << (std::ostream& os, const TYPENAME CDQHLines<CDQHVtx2DSample>::TVtxs& f){
	f.Print(os); return os;
}

inline void CDQHVtx2DSample::Print(std::ostream& os) const {
//		os << Vtx();
		os << id_ << " ";
}
inline std::ostream& operator << (std::ostream& os, const CDQHVtx2DSample& f){
	f.Print(os); return os;
}

}

#endif
