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

//----------------------------------------------------------------------------
//	CDRoundCone
CDRoundCone::CDRoundCone() {
}

CDRoundCone::CDRoundCone(const CDRoundConeDesc& desc):CDRoundConeDesc(desc){
}

bool CDRoundCone::IsInside(const Vec3f& p){
	return (p - Vec3f(0.0f, 0.0f, 0.5f * -length)).square() < radius[0] * radius[0] ||
		   (p - Vec3f(0.0f, 0.0f, 0.5f * +length)).square() < radius[1] * radius[1] ||
		   (p.x*p.x + p.y*p.y < radius * radius && -0.5f*length < p.z && p.z < 0.5f*length);
}
float CDRoundCone::CalcVolume(){
	return  2.0f/3.0f * (float)M_PI * radius[0] * radius[0] * radius[0] +
			2.0f/3.0f * (float)M_PI * radius[1] * radius[1] * radius[1] +
			(float)M_PI * (radius[0]*radius[0] + radius[0]*radius[1] + radius[1]*radius[1]) * length / 3.0;
}
	
// サポートポイントを求める
Vec3f CDRoundCone::Support(const Vec3f& p) const {
	float dr = (radius[1]-radius[0]);
	Vec3d d = Vec3d(-dr, 0, sqrt(length*length-(dr*dr))); d = d / d.norm();
	float n = p.norm();
	Vec3d q = p / p.norm();
	Vec3f off;
	if(n < 1.0e-10f){
		off = Vec3f();
	}else{
		off = n * p;
	}
	if (q.Z() >= d.Z()){
		off *= radius[0];
		off.Z() += length*0.5f;
	}else{
		off *= radius[1];
		off.Z() -= length*0.5f;
	}
	return off;
}


// 切り口を求める. 接触解析を行う.
bool CDRoundCone::FindCutRing(CDCutRing& ring, const Posed& toW) {
	//	切り口(ring.local)系での カプセルの向き
	Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0,0,1);
	if (dir.X() < 0) dir = -dir;
	
	if (dir.X() < 0.3f){		//	カプセルが接触面に大体平行な場合
		float shrink = sqrt(1-dir.X()*dir.X());	//	傾いているために距離が縮む割合
		float start = -0.5f*length*shrink;
		float end = 0.5f*length*shrink;
		if (dir.X() > 1e-4){	//	完全に平行でない場合
			Vec3f center = ring.localInv * toW.Pos();
			float is = -(center.X()-radius[0]/shrink) / dir.X() * shrink;	//	接触面と中心線を半径ずらした線との交点
			if (is < end) end = is;

			if (end+0.001 < start){
				DSTR << "CDRoundCone::FindCutRing() may have a problem" << std::endl;
			}
//			assert(end + 0.001 >= start);

			if (end <= start) return false;
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

Vec2f CDRoundCone::GetRadius() {
	return radius;
}
float CDRoundCone::GetLength() {
	return length;
}

}	//	namespace Spr
