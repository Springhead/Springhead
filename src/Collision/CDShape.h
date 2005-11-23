#ifndef PHSHAPE_H
#define PHSHAPE_H

#include <Springhead.h>
#include <Foundation/Object.h>

namespace Spr{;
class CDShape : public CDShapeIf, public Object{
public:
	OBJECTDEFABST(CDShape);
	BASEIMP_OBJECT(Object);
	Posef pose;
	
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax)=0;
};
class CDShapes:public std::vector< UTRef<CDShape> >{
};

}	//	namespace Spr
#endif
