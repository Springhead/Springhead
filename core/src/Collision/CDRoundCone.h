/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDRoundCone_H
#define CDRoundCone_H

#include <Collision/CDConvex.h>

namespace Spr{;
	
/// カプセル
class CDRoundCone: public CDConvex{
public:
	SPR_OBJECTDEF(CDRoundCone);
	SPR_DECLMEMBEROF_CDRoundConeDesc;

	CDRoundCone();
	CDRoundCone(const CDRoundConeDesc& desc);
	
	virtual bool     IsInside(const Vec3f& p);
	virtual float    CalcVolume();
	virtual Vec3f    CalcCenterOfMass();
	virtual Matrix3f CalcMomentOfInertia();

	///	サポートポイントを求める．
	virtual int Support(Vec3f&w, const Vec3f& v) const;
	///	切り口を求める．接触解析に使う．
	/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
	/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
	/// 表面上の点pにおける曲率半径を求める
	virtual double CurvatureRadius(Vec3d p);
	///< 表面上の点pにおける法線
	virtual Vec3d Normal(Vec3d p);

	virtual int GetVtxCount() const { return INT32_MAX; };

	Vec2f GetRadius();
	float GetLength();
	void SetRadius(Vec2f r);
	void SetLength(float l);
	//lengthを変えないでカプセルの太さだけ変更する
	void SetWidth(Vec2f r);

	int LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset);
};

	
}	//	namespace Spr

#endif	// CDRoundCone_H
