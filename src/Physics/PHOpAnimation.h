#ifndef PHOPANIMATION_H
#define PHOPANIMATION_H

//#include "Physics/PHOpEngine.h"
//#include <Physics/SprEngine.h>
#include <Physics/SprPHOpObj.h>
#include <Foundation/Object.h>
namespace Spr
{;
class PHOpAnimation : public Object, public PHOpAnimationDesc
	{
	
	public :
		SPR_OBJECTDEF(PHOpAnimation);
		ACCESS_DESC(PHOpAnimation);
		//PHOpEngine * opEngine;
		

		std::vector<OpAnimationStruct> animatedPs;

		//std::vector<int> animatedPindex;
		//std::vector<int> animatedObjindex;
		//std::vector<float> animatedHoldtimes;
		//
		////Two Types
		//std::vector<int> animatedType;

		////Animated Force
		//std::vector<Vec3f> animatedForces;

		////local Position of animation
		//std::vector<Vec3f> animatedPStarts;
		//std::vector<Vec3f> animatedPEnds;

		//Vec3f* 
		/*void intial(PHOpEngine * engine)
		{
			opEngine = engine;
			opEngine->opObjs[0]->params.timeStep;
		}*/
		void AddAnimationP(int objIndex, int pIndex, Vec3f start, Vec3f end, int interval);
		void AddAnimationP(int objIndex, int pIndex, Vec3f force, float holdCount);
		//OpAnimationStruct* GetAnimeInfo(int animeindex);
		Vec3f GetAnimatedPStart(int animeindex);
		Vec3f GetAnimatedPEnd(int animeindex);
		Vec3f GetAnimatedForce(int animeindex);
		float GetAnimatedTime(int animeindex);

		void AnimationStep(void* opEngine);
		void AnimationIntergration(void* opEngine);

	};
}

#endif
