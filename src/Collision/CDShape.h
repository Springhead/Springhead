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
	void SetPose(const Posef& p){ pose = p; }
	
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax)=0;
};
template <class intf, class base>
struct InheritCDShape:public InheritNamedObject<intf, base>{
	void SetPose(const Posef& p){ base::SetPose(p); }
	Posef GetPose() const { return base::GetPose(); }
};

}	//	namespace Spr
#endif
