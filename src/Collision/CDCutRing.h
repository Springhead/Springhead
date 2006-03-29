#ifndef CDCUTRING_H
#define CDCUTRING_H

#include <SprCollision.h>
#include <Collision/CDQuickHull2D.h>
#include <Foundation/Object.h>

namespace Spr{;

///	接触解析に使うクラス．接触部分の切り口の線分を表す．
class CDCutLine{
protected:
	Vec2d normal;
	double dist;
	double distInv;
public:
	CDCutLine(Vec2d n, double d):normal(n), dist(d){
		const double epsilon = 1e-10;
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
	///	切り口の中の1点と，切り口面の座標系
//	Vec3d center;
//	Matrix3d local, localInv;
	Posed local, localInv;
	///	切り口を構成する直線
	std::vector<CDCutLine> lines;
	///	双対変換 → QuickHull で一番内側の凸多角形の頂点を求める．
	static CDQHLines<CDCutLine> vtxs;	//	一番内側の凸多角形の頂点

	CDCutRing(Vec3d c, Matrix3d ccs){
		local.Ori().FromMatrix(ccs);
		local.Pos() = c;
		localInv = local.Inv();
	}
	void MakeRing();
	void Print(std::ostream& os);
};

}

#endif
