/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include "PHOpAnimation.h"
#include <Physics/SprPHOpObj.h>
#include "PHOpEngine.h"

namespace Spr{
	;

	void PHOpAnimation::AnimationStep(void* opEngine)
	{
		timestep = ((PHOpEngine*)opEngine)->opObjs[0]->params.timeStep;
		//orig Pose base
		for (int ai = 0; ai < (int)animatedPs.size(); ai++)
		{
			if (animatedPs[ai].animatedHoldtimes == 0)
				continue;
			PHOpObj* pobj = ((PHOpEngine*)opEngine)->opObjs[animatedPs[ai].animatedObjindex];
			PHOpParticle* dp = &pobj->objPArr[animatedPs[ai].animatedPindex];
			if (animatedPs[ai].animatedType == 1)
			{
				dp->pNewCtr += animatedPs[ai].animatedForces * timestep * timestep / dp->pTotalMass;
				
			}
			dp->pOrigCtr += (dp->pNewCtr - dp->pCurrCtr);
			animatedPs[ai].animatedHoldtimes--;
		}


		////external force based
		//for (int ai = 0; ai < animatedPs.size(); ai++)
		//{
		//	if (animatedPs[ai].animatedHoldtimes == 0)
		//		continue;

		//	PHOpObj* pobj = opEngine->opObjs[animatedPs[ai].animatedObjindex];
		//	PHOpParticle* dp = &pobj->objPArr[animatedPs[ai].animatedPindex];

		//	if (animatedPs[ai].animatedType == 0)
		//	{

		//	}
		//	else if (animatedPs[ai].animatedType == 1)
		//	{
		//		dp->pExternalForce += animatedPs[ai].animatedForces;
		//	}
		//
		//	animatedPs[ai].animatedHoldtimes--;
		//}
	}

	void PHOpAnimation::AnimationIntergration(void* opEngine)
	{
		for (int ai = 0; ai < (int)animatedPs.size(); ai++)
		{
			PHOpObj* pobj = ((PHOpEngine*)opEngine)->opObjs[animatedPs[ai].animatedObjindex];
			PHOpParticle* dp = &pobj->objPArr[animatedPs[ai].animatedPindex];
			//dp->pOrigCtr += (dp->pNewCtr - dp->pCurrCtr);
			dp->pCurrCtr = dp->pNewCtr;
		}
	}
	void PHOpAnimation::AddAnimationP(int objIndex, int pIndex, Vec3f start, Vec3f end, int interval)
	{//Animated by Start And End
		OpAnimationStruct tmp;
		tmp.animatedObjindex = objIndex;
		tmp.animatedPindex = pIndex;
		tmp.animatedPStarts = start;
		tmp.animatedPEnds = end;
		tmp.animatedHoldtimes = interval;
		tmp.animatedType = 0;
		animatedPs.push_back(tmp);

	}
	void PHOpAnimation::AddAnimationP(int objIndex, int pIndex, Vec3f force, float holdCount)
	{//Animated by ExternalForce

		OpAnimationStruct tmp;
		tmp.animatedObjindex = objIndex;
		tmp.animatedPindex = pIndex;
		tmp.animatedForces = force;
		tmp.animatedHoldtimes = holdCount;
		tmp.animatedType = 1;
		animatedPs.push_back(tmp);
	}
	Vec3f PHOpAnimation::GetAnimatedPStart(int animeindex)
	{
		return animatedPs[animeindex].animatedPStarts;
	}

	Vec3f PHOpAnimation::GetAnimatedPEnd(int animeindex)
	{
		return animatedPs[animeindex].animatedPEnds;
	}
	Vec3f PHOpAnimation::GetAnimatedForce(int animeindex)
	{
		return animatedPs[animeindex].animatedForces;
	}
	float PHOpAnimation::GetAnimatedTime(int animeindex)
	{
		return animatedPs[animeindex].animatedHoldtimes;
	}
}