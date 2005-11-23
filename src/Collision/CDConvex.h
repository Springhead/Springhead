#ifndef CDCONVEX_H
#define CDCONVEX_H

#include <Springhead.h>
#include <Foundation/Object.h>
#include "CDShape.h"

namespace Spr{;
class CDConvex : public CDShape{
public:
	OBJECTDEFABST(CDConvex);
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const =0;
};

}	//	namespace Spr
#endif
