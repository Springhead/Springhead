/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDCapsule_H
#define CDCapsule_H

#include <Foundation/Object.h>
#include <Collision/CDConvex.h>

namespace Spr{;
	
/// カプセル
class CDCapsule: public CDConvex{
public:
	SPR_OBJECTDEF(CDCapsule);
	SPR_DECLMEMBEROF_CDCapsuleDesc;

	CDCapsule(const CDCapsuleDesc& desc = CDCapsuleDesc());
	
	virtual bool IsInside(const Vec3f& p);
	virtual float CalcVolume();
	virtual Matrix3f CalcMomentOfInertia();
	
	///	サポートポイントを求める．
	virtual int Support(Vec3f& w, const Vec3f& v) const;
	///	切り口を求める．接触解析に使う．
	/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
	/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
	
	float	GetRadius(){ return radius; }
	void	SetRadius(float r){ radius = r; }
	float	GetLength(){ return length; }
	void	SetLength(float l){ length = l; }

	int LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset);
};



	
}	//	namespace Spr

#endif	// CDCapsule_H
