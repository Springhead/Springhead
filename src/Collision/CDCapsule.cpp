/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
const double epsilon = 1e-8;

//----------------------------------------------------------------------------
//	CDCapsule
IF_OBJECT_IMP(CDCapsule, CDConvex);	
	
CDCapsule::CDCapsule() {
}

CDCapsule::CDCapsule(const CDCapsuleDesc& desc):CDCapsuleDesc(desc){
}

// サポートポイントを求める
Vec3f CDCapsule::Support(const Vec3f& p) const {	
	float n = p.norm();
	Vec3f off;
	if(n < 1.0e-10f){
		off = Vec3f();
	}else{
		off = (radius / n) * p;
	}
	if (p.Z() >= 0){
		off.Z() += length*0.5f;
	}else{
		off.Z() -= length*0.5f;
	}
	return off;
}


// 切り口を求める. 接触解析を行う.
bool CDCapsule::FindCutRing(CDCutRing& ring, const Posed& toW) {
	//	切り口(ring.local)系での カプセルの向き
	Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0,0,1);
	if (dir.X() < 0) dir = -dir;
	
	if (dir.X() < 0.3f){		//	カプセルが接触面に大体平行な場合
		float shrink = sqrt(1-dir.X()*dir.X());	//	傾いているために距離が縮む割合
		float start = -0.5f*length*shrink;
		float end = 0.5f*length*shrink;
		if (dir.X() > epsilon){	//	完全に平行でない場合
			Vec3f center = ring.localInv * toW.Pos();
			float is = -(center.X()-radius) / dir.X() * shrink;	//	接触面と中心線を半径ずらした線との交点
			if (is < end) end = is;
			assert(end > start);
		}

		//	ringに線分を追加
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Y(), -dir.Z()), -start));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Y(), dir.Z()), end));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Z(), -dir.Y()), 0));
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Z(), dir.Y()), 0));
		return true;
	}else{
		return false;
	}
}

float CDCapsule::GetRadius() {
	return radius;
}
float CDCapsule::GetLength() {
	return length;
}

}	//	namespace Spr
