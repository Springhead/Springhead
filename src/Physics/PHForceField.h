#ifndef PH_FORCEFIELD_H
#define PH_FORCEFIELD_H

#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Physics/PHSolid.h>
#include <list>

namespace Spr{

	void RegisterPHGravityLoader(FILoadScene* loader);
	void RegisterPHGravitySaver (FISaveScene* saver );

	///////////////////////////////////////////////////////////
	//PHGravityEngine

	class PHGravityEngine : public SGBehaviorEngine
	{
	public:
		SGOBJECTDEF(PHGravityEngine);
		PHSolids solids;
		Vec3f	accel;

		int GetPriority() const {return SGBP_GRAVITYENGINE;}
		void Step(SGScene* s);
		void ApplyForce();
		void Clear(SGScene* s){solids.clear();}
		bool AddChildObject(SGObject* o, SGScene* s);
	};
}

#endif