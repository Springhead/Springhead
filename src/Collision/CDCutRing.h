/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDCUTRING_H
#define CDCUTRING_H

#include "CDQuickHull2DImp.h"

namespace Spr{;

///	接触解析に使うクラス．接触部分の切り口の線分を表す．
class CDCutLine{
public:
	Vec2d normal;
	double dist;
	double distInv;
public:
	CDCutLine(Vec2d n, double d):normal(n), dist(d){
		const double epsilon = 1e-10;
		assert(finite(n.x));
		assert(finite(n.y));
		assert(finite(d));
		if (dist < epsilon) dist = epsilon;
		distInv = 1/dist;
	}
	Vec2d GetPos() const { 
		return normal*distInv; 
	}
	friend class CDCutRing;
};
///	接触解析に使うクラス．接触部分の切り口
class CDCutRing{
public:
	///	切り口の中の1点と，切り口面の座標系．ex が法線
	Posed local, localInv;
	///	切り口を構成する直線
	std::vector<CDCutLine> lines;
	///	双対変換 → QuickHull で一番内側の凸多角形の頂点を求める．
	static CDQHLines<CDCutLine> vtxs;	//	一番内側の凸多角形の頂点

	CDCutRing(Vec3d c, Matrix3d l){
		local.Ori().FromMatrix(l);
		local.Pos() = c;
		localInv = local.Inv();
	}
	void MakeRing();
	void Print(std::ostream& os);
};

}

#endif
