/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef CDCYLINDER_H
#define CDCYLINDER_H

#include <Foundation/Object.h>
#include <Collision/CDConvex.h>

namespace Spr {
	;

	/// 円筒
	class CDCylinder : public CDConvex {
	public:
		SPR_OBJECTDEF(CDCylinder);
		SPR_DECLMEMBEROF_CDCylinderDesc;

		CDCylinder(const CDCylinderDesc& desc = CDCylinderDesc());

		virtual bool IsInside(const Vec3f& p);
		virtual float CalcVolume();
		virtual Matrix3f CalcMomentOfInertia();

		///	サポートポイントを求める．
		virtual int Support(Vec3f& w, const Vec3f& v) const;
		/// サポートポイントがどこにあるかを求める．
		virtual int SupportTag(const Vec3f& v, std::vector<Vec3d>& c_vec) const ;

		///	切り口を求める．接触解析に使う．
		/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
		/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
		virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
		virtual int GetVtxCount() const { return INT32_MAX; };

		float	GetRadius() { return radius; }
		void	SetRadius(float r) { radius = r; bboxReady = false; }
		float	GetLength() { return length; }
		void	SetLength(float l) { length = l; bboxReady = false; }

		int LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset);
		Vec3d GetPointA(); // from orientation
		Vec3d GetPointB(); // from orientation

	};




}	//	namespace Spr

#endif	// CDCylinder_H