#ifndef PHSHAPE_H
#define PHSHAPE_H

#include <SprCollision.h>
#include <Foundation/Object.h>

namespace Spr{;
class CDShape : public CDShapeIf, public NamedObject{
protected:
	Posef pose;
public:
	PHMaterial material;

	OBJECTDEFABST(CDShape);
	BASEIMP_NAMEDOBJECT(NamedObject);
	Posef GetPose() const { return pose; }
	void SetPose(Posef p){ pose = p; }
	
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax)=0;
};
#define	BASEIMP_CDSHAPE(base)	BASEIMP_NAMEDOBJECT(base)

}	//	namespace Spr
#endif
