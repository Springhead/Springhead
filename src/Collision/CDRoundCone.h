/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDRoundCone_H
#define CDRoundCone_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDConvex.h"

namespace Spr{;
	
/// カプセル
class CDRoundCone: public CDConvex, public CDRoundConeDesc{
public:
	SPR_OBJECTDEF(CDRoundCone);

	CDRoundCone();
	//CDRoundCone(float r = 0.0f, Vec3f c = Vec3f()):radius(r), center(c){}
	//CDRoundCone(Vec3f c = Vec3f(), float r = 0.0f):center(c), radius(r){}
	CDRoundCone(const CDRoundConeDesc& desc);
	
	virtual bool IsInside(const Vec3f& p);
	virtual float CalcVolume();
	
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
	///	切り口を求める．接触解析に使う．
	/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
	/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
	/// 表面上の点pにおける曲率半径を求める
	virtual double CurvatureRadius(Vec3d p);
	
	Vec2f GetRadius();
	float GetLength();
};

	
}	//	namespace Spr

#endif	// CDRoundCone_H
