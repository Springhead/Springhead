/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef ROBOT1_H
#define ROBOT1_H

#include <Springhead.h>		//	Springheadのインタフェース

using namespace Spr;

class Robot1{
public:
	PHSolidIf*	soBody;
	PHSolidIf*	soWheel[4];
	PHHingeJointIf* jntWheel[4];

	CDBoxIf*		boxBody;
	CDConvexMeshIf*	meshWheel;

	void InitCylinder(CDConvexMeshDesc& md, float height, float radius, int ndiv);
	void Build(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk);
	void Stop();
	void Forward();
	void Backward();
	void TurnLeft();
	void TurnRight();
};

#endif
