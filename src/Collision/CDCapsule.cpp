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


// 切り口を求める. 
bool CDCapsule::FindCutRing(CDCutRing& ring, const Posed& toW) {
	//	切り口(ring.local)系での カプセルの向き
	Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0,0,1);
	if (dir.X() < 0) dir = -dir;
	
	if (dir.X() < 1e-6){		//	カプセルが接触面に平行な場合だけは線分になる。
		float shrink = 1;
		float start = -0.5f*length*shrink;
		float end = 0.5f*length*shrink;
		//	ringに線分を追加
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Y(), -dir.Z()), -start));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Y(), dir.Z()), end));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Z(), -dir.Y()), 0));
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Z(), dir.Y()), 0));
		return true;
	}else{
		return false;			//	他は1点で接触
	}
}

float CDCapsule::GetRadius() {
	return radius;
}
float CDCapsule::GetLength() {
	return length;
}

}	//	namespace Spr
