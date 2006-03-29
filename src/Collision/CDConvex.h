#ifndef CDCONVEX_H
#define CDCONVEX_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDShape.h"
#include "CDCutRing.h"

namespace Spr{;
class CDConvex : public CDShape{
public:
	OBJECTDEFABST(CDConvex);
	///	サポートポイント(方向ベクトルpとの内積が最大の頂点)を求める．
	virtual Vec3f Support(const Vec3f& p) const =0;
	///	切り口を求める．接触解析に使う．
	virtual void FindCutRing(CDCutRing& r, const Posed& toW) =0;
	///	バウンディングボックスを求める．
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax);
};

}	//	namespace Spr
#endif
