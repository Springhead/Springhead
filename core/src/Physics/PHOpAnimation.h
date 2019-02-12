/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef PHOPANIMATION_H
#define PHOPANIMATION_H

#include <Physics/SprPHOpObj.h>
#include <Foundation/Object.h>
namespace Spr
{;
class PHOpAnimation : public Object, public PHOpAnimationDesc
	{
	
	public :
		SPR_OBJECTDEF(PHOpAnimation);
		ACCESS_DESC(PHOpAnimation);
		

		std::vector<OpAnimationStruct> animatedPs;

		void AddAnimationP(int objIndex, int pIndex, Vec3f start, Vec3f end, int interval);
		void AddAnimationP(int objIndex, int pIndex, Vec3f force, float holdCount);
		Vec3f GetAnimatedPStart(int animeindex);
		Vec3f GetAnimatedPEnd(int animeindex);
		Vec3f GetAnimatedForce(int animeindex);
		float GetAnimatedTime(int animeindex);

		void AnimationStep(void* opEngine);
		void AnimationIntergration(void* opEngine);

	};
}

#endif
