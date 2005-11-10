#ifndef PHSPRING_H
#define PHSPRING_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PHSolid.h"
#include <SceneGraph/SGScene.h>

namespace Spr{;
class PHSpring: public SGBehaviorEngine{
public:
	Vec3f pos[2];
	Quaternionf ori[2];
	Vec3f kt, bt;
	Vec3f kr, br;
	UTRef<PHSolid> solid[2];
	SGOBJECTDEF(PHSpring);
	PHSpring();
	bool AddChildObject(SGObject* o, SGScene* s);
	virtual void Step(SGScene* s);
};

}
#endif
