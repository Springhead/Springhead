/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef ROBOT_H
#define ROBOT_H

#include <Springhead.h>		//	Springheadのインタフェース

using namespace Spr;

class Robot{
public:
	PHSolidIf*	soBody;
	CDBoxIf*	boxBody;
	CDBoxIf			*boxCrank, *boxFoot, *boxGuide;

	struct Leg{
		PHSolidIf		*soCrank, *soFoot[2], *soGuide[2];
		PHHingeJointIf	*jntCrank, *jntFoot[2], *jntFootGuide[2], *jntGuideBody[2];
		void Build(PHSolidIf* body, PHRootNodeIf* root, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk);
	};

	Leg	leg[4];

	void Build(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk);
	void Stop();
	void Forward();
	void Backward();
	void TurnLeft();
	void TurnRight();
};

#endif
