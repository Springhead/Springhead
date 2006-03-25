#ifndef CDCUTRING_H
#define CDCUTRING_H

#include <SprCollision.h>
#include <Foundation/Object.h>

namespace Spr{;

///	接触解析に使うクラス．接触部分の切り口の線分を表す．
class CDCutLine{
public:
	Vec2d normal;
	double dist;
};
///	接触解析に使うクラス．接触部分の切り口
class CDCutRing{
public:
	///	切り口の中の1点と，切り口面の座標系
	Vec3d center;
	Matrix3d local, local_inv;
	///	切り口を構成する直線
	std::vector<CDCutLine> lines;
	CDCutRing(Vec3d c, Matrix3d ccs): center(c), local(ccs){
		local_inv = local.inv();
	}
	void MakeRing();
};

}

#endif
