#ifndef CRSIMPLE_H
#define CRSIMPLE_H

#include <SprCreature.h>
#include <Foundation/Object.h>

namespace Spr{;
class CRSimple : public InheritSceneObject<CRSimpleIf, SceneObject>, public CRSimpleDesc{
	OBJECT_DEF(CRSimple);
	CRSimple(const CRSimpleDesc& desc=CRSimpleDesc(), SceneIf* s=NULL){}
	void Hoge();
	void Step();
};
}	//	namespace Spr

#endif