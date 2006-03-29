#ifndef CDCUTRING_H
#define CDCUTRING_H

#include <SprCollision.h>
#include <Foundation/Object.h>

namespace Spr{;

///	接触解析に使うクラス．接触部分の切り口の線分を表す．
class CDCutLine{
public:
	CDCutLine(Vec2d n, double d):normal(n), dist(d){}
	Vec2d normal;
	double dist;
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
