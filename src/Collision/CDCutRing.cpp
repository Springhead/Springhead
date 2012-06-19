/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDCutRing.h>

namespace Spr{;
CDQHLines<CDCutLine> CDCutRing::vtxs(1000);		//	一番内側の凸多角形の頂点

void CDCutRing::MakeRing(){
	//	Mullar＆Preparataで，形を求める
	//	cutLines->CreateHull();
	std::vector<CDCutLine*> linePtrs;
	linePtrs.resize(lines.size());
	for(unsigned i=0; i<lines.size(); ++i) linePtrs[i] = &lines[i];
	vtxs.Clear();
	vtxs.CreateConvexHull(&*linePtrs.begin(), &*linePtrs.begin() + linePtrs.size());
}
void CDCutRing::Print(std::ostream& os){
	if (vtxs.size()){
		int nDel=0;
		for(CDQHLine<CDCutLine>* vtx = vtxs.begin; vtx!=vtxs.end; ++vtx){
			if (vtx->deleted) nDel ++;
		}
		DSTR << vtxs.size()-nDel << "lines are used." << std::endl;
		for(CDQHLine<CDCutLine>* vtx = vtxs.begin; vtx!=vtxs.end; ++vtx){
			if (vtx->deleted) continue;
			CDCutLine* line = vtx->vtx[0];
			Vec3d n(0, line->normal.x, line->normal.y);
			n = local.Ori() * n;
			DSTR << n << " d=" << line->dist << std::endl;
		}
	}else{
		DSTR << lines.size() << " lines" << std::endl;
		for(unsigned i=0; i<lines.size(); ++i){
			Vec3d n(0, lines[i].normal.x, lines[i].normal.y);
			n = local.Ori() * n;
			DSTR << n << " d=" << lines[i].dist << std::endl;
		}
	}
};

}
