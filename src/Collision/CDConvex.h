#ifndef CDCONVEX_H
#define CDCONVEX_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDShape.h"

namespace Spr{;
class CDConvex : public CDShape{
public:
	OBJECTDEFABST(CDConvex);
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const =0;
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax);
};
#define		BASEIMP_CDCONVEX(base)	BASEIMP_CDSHAPE(base)

}	//	namespace Spr
#endif
