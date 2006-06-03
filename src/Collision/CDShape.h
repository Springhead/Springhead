#ifndef PHSHAPE_H
#define PHSHAPE_H

#include <SprCollision.h>
#include <Foundation/Object.h>

namespace Spr{;

/// 形状
/// 形状自体は位置情報を持たない．位置情報は形状を参照する各剛体が持つ．
class CDShape : public InheritNamedObject<CDShapeIf, NamedObject>{
protected:
	//Posef pose;
public:
	PHMaterial material;
	static double epsilon;

	OBJECTDEFABST(CDShape);
	//Posef GetPose() const { return pose; }
	//void SetPose(const Posef& p){ pose = p; }
	
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose)=0;
};

/// 形状への参照と剛体に対する相対位置
struct CDShapeRefWithPose{
	CDShape* shape;
	Posed pose;
};

template <class intf, class base>
struct InheritCDShape:public InheritNamedObject<intf, base>{
	//void SetPose(const Posef& p){ base::SetPose(p); }
	//Posef GetPose() const { return base::GetPose(); }
};

}	//	namespace Spr
#endif
