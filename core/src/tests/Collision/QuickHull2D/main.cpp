/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** 
 Springhead2/src/tests/Collision/QuickHull2D/main.cpp

【概要】
  ・QucickHull2D および QuickHull3D のテストプログラム
  
【終了基準】
  ・プログラムが正常終了したら0を返す。
 
【処理の流れ】
  ・QuickHullが使用する頂点を登録する。
  ・設定した頂点から凸包を作り、出来た面を出力する。
    また、QuickHull2Dの場合は、エクセルに出来た面を出力する。

 */
#include "Collision/CDQuickHull3DImp.h"
#include "Collision/CDQuickHull2DImp.h"
#include <iostream>
#include <fstream>

using namespace Spr;

//	2Dのテスト
int main2D(){
	std::vector<CDQHVtx2DSample> vtxBufs;
#if 0
	for(int i=0; i<6; ++i){
		vtxBufs.push_back(CDQHVtx2DSample());
		vtxBufs.back().id_ = i;
	}
	vtxBufs[0].pos = Vec2f( 1.0f,  1.0f);
	vtxBufs[1].pos = Vec2f( 1.0f, -1.0f);
	vtxBufs[2].pos = Vec2f(-1.0f,  1.0f);
	vtxBufs[3].pos = Vec2f(-1.0f, -1.0f);
	vtxBufs[4].pos = Vec2f( 2.0f,  1.0f);
	vtxBufs[5].pos = Vec2f( 1.0f, -2.0f);
#else
	for(int i=0; i<20; ++i){
//		vtxBufs.push_back(CDQHVtx2DSample());
//		vtxBufs.back().id_ = i;
//		vtxBufs.back().pos = Vec2f(rand()*10.0/RAND_MAX,  rand()*10.0/RAND_MAX);
		vtxBufs.push_back(CDQHVtx2DSample());
		vtxBufs.back().id_ = i*3;
		vtxBufs.back().pos = Vec2f(0,  1);
		vtxBufs.push_back(CDQHVtx2DSample());
		vtxBufs.back().id_ = i*3+1;
		vtxBufs.back().pos = Vec2f(1,  0);
		vtxBufs.push_back(CDQHVtx2DSample());
		vtxBufs.back().id_ = i*3+2;
		vtxBufs.back().pos = Vec2f(1,  1);
	}
#endif
	CDQHLines<CDQHVtx2DSample>::TVtxs vtxs;
	for(unsigned i=0; i<vtxBufs.size(); ++i){
		vtxs.push_back(&*vtxBufs.begin()+i);
	}

	CDQHLines<CDQHVtx2DSample> lines(50);
	lines.CreateConvexHull(&vtxs.front(), &vtxs.back()+1);
	//	できた面を表示
	std::cout << lines;

	//	できた面をエクセルでグラフにできるようにファイルに出力
	std::ofstream file("hull.xls");
	CDQHLine<CDQHVtx2DSample>* cur = lines.end-1;
	do {
		file << cur->vtx[0]->GetPos().X() << "\t";
		file << cur->vtx[0]->GetPos().Y() << "\n";
		cur = cur->neighbor[1];
	} while(cur != lines.end-1);
	file << "\n";
	for(unsigned i=0; i<vtxs.size(); ++i){
		file << vtxs[i]->pos.X() << "\t";
		file << vtxs[i]->pos.Y() << "\n";
	}
	return 0;
}

//	3Dのテスト
int main3D(){
	std::vector<CDQHVtx3DSample> vtxBufs;
	struct NormalDist{
		NormalDist(Vec3f n, double d):normal(n),dist(d){}
		Vec3f normal;
		double dist;
	};
	NormalDist inputs[] = {
		NormalDist(Vec3f(0.541109,0.000453501,0.840953),1e-006),
		NormalDist(Vec3f(-0.537583,0.00281859,-0.843206),1e-006),
		NormalDist(Vec3f(0.000162262,-1,0.000434865),0.218723),
		NormalDist(Vec3f(-0.00114141,-0.999996,-0.00261497),0.217835),
		NormalDist(Vec3f(-0.84321,-0.000443318,0.537584),0.595489),
		NormalDist(Vec3f(0.537583,-0.00281859,0.843206),1),
		NormalDist(Vec3f(-0.000162246,1,-0.00043484),0.781277),
		NormalDist(Vec3f(0.00114142,0.999996,0.00261497),0.782165),
		NormalDist(Vec3f(-0.840953,9.88755e-005,0.541109),0.873115),
		NormalDist(Vec3f(-0.541109,-0.000453501,-0.840953),1),
		NormalDist(Vec3f(0.840953,-9.8811e-005,-0.541109),1.12688),
		NormalDist(Vec3f(0.84321,0.000443305,-0.537584),1.40451),
	};
	for(int i=0; i<12; ++i){
		vtxBufs.push_back(CDQHVtx3DSample());
		vtxBufs.back().dir = inputs[i].normal;
		vtxBufs.back().dist = (float)inputs[i].dist;
		vtxBufs.back().id_ = i;
	}

	CDQHPlanes<CDQHVtx3DSample>::TVtxs vtxs;
	for(unsigned i=0; i<vtxBufs.size(); ++i){
		vtxs.push_back(&*vtxBufs.begin()+i);
	}

	CDQHPlanes<CDQHVtx3DSample> planes(50);
	planes.CreateConvexHull(&vtxs.front(), &vtxs.back()+1);
	//	できた面を表示
	std::cout << planes;
	return 0;
}
int SPR_CDECL main(){
	int rv = main2D();
	if (rv) return rv;
	return main3D();
}
