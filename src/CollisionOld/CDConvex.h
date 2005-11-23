#ifndef CDCONVEX_H
#define CDCONVEX_H
#include "CDGeometry.h"
#include "CDPhysicalMaterial.h"

namespace Spr {;

class CDConvex:public CDGeometry{
public:
	SGOBJECTDEFABST(CDConvex);
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const =0;
};

}
#endif
