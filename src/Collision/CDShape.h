#ifndef PHSHAPE_H
#define PHSHAPE_H

#include <SprCollision.h>
#include <Foundation/Object.h>

namespace Spr{;
class CDShape : public InheritNamedObject<CDShapeIf, NamedObject>{
protected:
	Posef pose;
public:
	PHMaterial material;

	OBJECTDEFABST(CDShape);
	Posef GetPose() const { return pose; }
	void SetPose(Posef p){ pose = p; }
	
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax)=0;
};

}	//	namespace Spr
#endif
