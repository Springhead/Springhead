/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDSPHERE_H
#define CDSPHERE_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDConvex.h"

namespace Spr{;

	
/// 球体
class CDSphere: public CDConvex, CDSphereIfInit, public CDSphereDesc{
public:
	OBJECT_DEF(CDSphere);
	
	CDSphere();
	//CDSphere(float r = 0.0f, Vec3f c = Vec3f()):radius(r), center(c){}
	//CDSphere(Vec3f c = Vec3f(), float r = 0.0f):center(c), radius(r){}
	CDSphere(const CDSphereDesc& desc);
	
	/// ShapeType
	virtual int ShapeType(){ return CDShapeDesc::SPHERE; }
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
	///	切り口を求める．接触解析に使う．
	/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
	/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
	
	/// 球体の半径を取得
	float GetRadius();
};

	
}	//	namespace Spr

#endif	// CDSPHERE_H
