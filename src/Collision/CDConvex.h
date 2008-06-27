/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDCONVEX_H
#define CDCONVEX_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDShape.h"
#include "CDCutRing.h"

namespace Spr{;

class CDConvex : public CDShape{
protected:
	bool		valid;
	struct Boxel{
		Vec3f		pos;
		float		volume;
		Matrix3f	moment;
	};
	std::vector<Boxel> boxels;

	float		volume;
	Vec3f		center;
	Matrix3f	moment;

	/// 
	bool	IsBoxInside(const Vec3f& bbmin, const Vec3f& bbmax);
	/// 幾何情報の無効化
	void	Invalidate(){valid = false;}
	/// 幾何解析
	void	Analyze();
	void	AccumulateBoxels(const Vec3f& bbmin, const Vec3f& bbmax, float eps);
public:
	SPR_OBJECTDEF_ABST(CDConvex);

	/// 偏心によって生じるイナーシャ
	static void OffsetInertia(const Vec3f& c, Matrix3f& I);

	///	サポートポイント(方向ベクトルpとの内積が最大の頂点)を求める．
	virtual Vec3f Support(const Vec3f& p) const =0;
	///	切り口を求める．接触解析に使う．
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW) =0;
	///	バウンディングボックスを求める．
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose);

	virtual float CalcVolume(){
		if(!valid)Analyze();
		return volume;
	}
	virtual Vec3f	CalcCenterOfMass(){
		if(!valid)Analyze();
		return center;
	}
	virtual Matrix3f CalcMomentOfInertia(){
		if(!valid)Analyze();
		return moment;
	}

	CDConvex():valid(false){}
};

}	//	namespace Spr
#endif
