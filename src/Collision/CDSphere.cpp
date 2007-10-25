/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Springhead.h>
#include <Foundation/Scene.h>
#include "Collision.h"
#include <set>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDQuickHull3DImp.h"

namespace Spr{;
	
//----------------------------------------------------------------------------
//	CDSphere
IF_OBJECT_IMP(CDSphere, CDConvex);	
	
CDSphere::CDSphere() {
}

CDSphere::CDSphere(const CDSphereDesc& desc) {
	radius = desc.radius;
}

// サポートポイントを求める
Vec3f CDSphere::Support(const Vec3f& p) const {	
	float n = p.norm();
	if(n < 1.0e-10f)
		return Vec3f();
	else return (radius / n) * p;
	/*
	Vec3f center = GetPose().Pos();		// 球体の中心座標を取得
	Vec3f p_ = p;
	p_ -= center;						// pからcenterへのベクトルを求める
	float s = p_.norm();				// pからcenterへのベクトルの長さ
	if (s > 1e-8f) {		
		float r = radius / s;	
		return r*p_ + center;			// 衝突検出に必要な球の最近傍点
	}else{
		return center;
	}
	*/
}

// 切り口を求める. 接触解析を行う.
bool CDSphere::FindCutRing(CDCutRing& r, const Posed& toW) {
	return false;
}

// 球体の半径を取得
float CDSphere::GetRadius() {
	return radius;
}
float CDSphere::CalcVolume(){
	return float(4.0/3.0*M_PI*radius*radius*radius);
}
Matrix3f CDSphere::CalcMomentOfInertia(){
	return Matrix3f();
}

}	//	namespace Spr
