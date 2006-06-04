#ifndef PH_FORCEFIELD_H
#define PH_FORCEFIELD_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class PHGravityEngine : public PHEngine{
	OBJECT_DEF_NOIF(PHGravityEngine);
public:
	PHSolids solids;
	Vec3f	accel;

	int GetPriority() const {return SGBP_GRAVITYENGINE;}
	void Step();
	void Clear(){solids.clear();}
	bool AddChildObject(Object* o);
};

}

#endif
