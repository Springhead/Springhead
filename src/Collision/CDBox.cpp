/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Springhead.h>
#include <Foundation/Object.h>
#include "Collision.h"
#include <set>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDQuickHull3DImp.h"

namespace Spr{;
const double sqEpsilon = 1e-4;
const double epsilon = 1e-8;
const double epsilon2 = epsilon*epsilon;

//----------------------------------------------------------------------------
//	CDBox
IF_OBJECT_IMP(CDBox, CDConvex);	

CDBox::CDBox() {
}

CDBox::CDBox(const CDBoxDesc& desc) {
	material = desc.material;

	boxsize = desc.boxsize;
	// ローカル座標系で、boxの位置を設定
	Vec3f halfsize = Vec3f(boxsize.x/2.0, boxsize.y/2.0, boxsize.z/2.0);
	base.push_back(Vec3f( halfsize.x,  halfsize.y, -halfsize.z));
	base.push_back(Vec3f( halfsize.x,  halfsize.y,  halfsize.z));
	base.push_back(Vec3f( halfsize.x, -halfsize.y,  halfsize.z));
	base.push_back(Vec3f( halfsize.x, -halfsize.y, -halfsize.z));
	base.push_back(Vec3f(-halfsize.x,  halfsize.y, -halfsize.z));
	base.push_back(Vec3f(-halfsize.x,  halfsize.y,  halfsize.z));
	base.push_back(Vec3f(-halfsize.x, -halfsize.y,  halfsize.z));
	base.push_back(Vec3f(-halfsize.x, -halfsize.y, -halfsize.z));

	for (int nface=0; nface<6; ++nface){	// 立方体は8面
		qfaces.push_back(CDQuadFace());
	}
	// boxの各面の法線を設定、boxの各面の頂点のインデックスを設定
	qfaces[0].normal  = Vec3f(1.0, 0.0, 0.0);
	qfaces[0].vtxs[0] = 0;		
	qfaces[0].vtxs[1] = 1;
	qfaces[0].vtxs[2] = 2;
	qfaces[0].vtxs[3] = 3;
	qfaces[1].normal  = Vec3f(0.0, 1.0, 0.0);
	qfaces[1].vtxs[0] = 0;		
	qfaces[1].vtxs[1] = 4;
	qfaces[1].vtxs[2] = 5;
	qfaces[1].vtxs[3] = 1;
	qfaces[2].normal  = Vec3f(-1.0, 0.0, 0.0);
	qfaces[2].vtxs[0] = 4;		
	qfaces[2].vtxs[1] = 7;
	qfaces[2].vtxs[2] = 6;
	qfaces[2].vtxs[3] = 5;
	qfaces[3].normal  = Vec3f(0.0, -1.0, 0.0);
	qfaces[3].vtxs[0] = 3;		
	qfaces[3].vtxs[1] = 2;
	qfaces[3].vtxs[2] = 6;
	qfaces[3].vtxs[3] = 7;
	qfaces[4].normal  = Vec3f(0.0, 0.0, 1.0);
	qfaces[4].vtxs[0] = 1;		
	qfaces[4].vtxs[1] = 5;
	qfaces[4].vtxs[2] = 6;
	qfaces[4].vtxs[3] = 2;
	qfaces[5].normal  = Vec3f(0.0, 0.0, -1.0);
	qfaces[5].vtxs[0] = 0;		
	qfaces[5].vtxs[1] = 3;
	qfaces[5].vtxs[2] = 7;
	qfaces[5].vtxs[3] = 4;
}

// サポートポイントを求める
Vec3f CDBox::Support(const Vec3f& p) const {
	// 与えられた方向pに一番遠い点（内積最大の点をサポートポイントとする）
	float d1=0.0, d2=0.0;
	for (unsigned int i=0; i<8; ++i){		// 8頂点
		d1 = base[i] * p;
		if (d1 > d2) { 
			d2 = d1;
			curPos = i;
		}
	}
	return base[curPos];
}

// 切り口を求める. 接触解析を行う.
bool CDBox::FindCutRing(CDCutRing& ring, const Posed& toW) {
	const int qfaces_size = 6;	// qfaces.size()=6面 
	const int base_size = 8;

	Posed toL	  = toW.Inv();
	// 頂点がどっち側にあるか調べる
	Vec3d planePosL = toL * ring.local.Pos();
	Vec3d planeNormalL = toL.Ori() * ring.local.Ori() * Vec3d(1, 0, 0);
	int sign[base_size];
	double d = planeNormalL * planePosL;
	for (int i=0; i<base_size; ++i){
		double vtxDist = planeNormalL * base[i];
		if (vtxDist > d + sqEpsilon) sign[i] = 1;
		else if (vtxDist < d - sqEpsilon) sign[i] = -1;
		else sign[i] = 0;
	}
	bool rv = false;
	//	またがっている面の場合，交線を求める
	for(int i=0; i<qfaces_size; ++i){		// face.size()=6面
		//　全頂点がplaneに対して同じ方向にある場合はパス
		if (sign[qfaces[i].vtxs[0]] == sign[qfaces[i].vtxs[1]] &&
			sign[qfaces[i].vtxs[0]] == sign[qfaces[i].vtxs[2]] &&
			sign[qfaces[i].vtxs[0]] == sign[qfaces[i].vtxs[3]]) continue;
		
		//	接触面(plane,面1)とboxの面(qface,面2)の交線を求める
		/*	直線をとおる1点を見つけるのは
						|面2
						|n2
						|d2
				   O	|
			-------+----+----面1 n1,d1=0
						|P
			P = a*n1 + b*n2;
				a = (d1 - d2*(n1*n2)) / (1-(n1*n2)^2)
				b = (d2 - d1*(n1*n2)) / (1-(n1*n2)^2)
			が面1(plane)と面2(qface)が作る直線を通る1点
			O:		planePosL
			n1,d1	planeの法線(planeNormalL)，Oからの距離=0
			n2,d2	qfaceの法線(qfaceNormal)，Oからの距離			
		*/
		Vec3d qfaceNormal = qfaces[i].normal;
		double qfaceDist = qfaceNormal * (base[qfaces[i].vtxs[0]] - planePosL);
		Vec3d lineDirection = (planeNormalL ^ qfaceNormal).unit();
		double ip = planeNormalL * qfaceNormal;
//		if ((ip < 1.0-epsilon2) && (ip > -1.0+epsilon2)){	//	平行な面は無視
			double a = -qfaceDist*ip / (1-(ip*ip));
			double b = qfaceDist / (1-(ip*ip));
			Vec3d lineOff = a*planeNormalL + b*qfaceNormal;
			Vec3d lineNormal = planeNormalL ^ lineDirection;
			double lineDist = lineNormal * lineOff;
			if (finite(lineDist)) {	
				// local -> world -> ring2次元系に変換
				Posed to2D = ring.localInv * toW;
				Vec2d lineNormal2D = (to2D.Ori() * lineNormal).sub_vector(1, Vec2d());
				//	線は内側を向かせたいので， normal, dist を反転して ring.lines に追加
				ring.lines.push_back(CDCutLine(-lineNormal2D, -lineDist));
				rv = true;
			} 
/*		}else{
			DSTR << "parallel plane" << std::endl;
			DSTR << "ip:" << ip << std::endl;
			DSTR << "pn:" << planeNormalL<<std::endl;
			DSTR << "fn:" << qfaceNormal<<std::endl;
			DSTR << "vtx:";
			for(int j=0; j<4; ++j){
				int id = qfaces[i].vtxs[j];
				double vtxDist = planeNormalL * base[id];
				DSTR << sign[id] << " " << vtxDist-d << " " << base[id];
			}
			DSTR << std::endl;
			exit(0);
		}
*/	}
	//bool 衝突の有無
	return rv;
}

// 直方体のサイズを取得
Vec3f CDBox::GetBoxSize() {
	return boxsize;
}

Vec3f* CDBox::GetVertices(){
	return &*base.begin();
}

CDFaceIf* CDBox::GetFace(size_t i){
	return &qfaces.at(i);
}

}	//	namespace Spr
