/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef ROBOT_H
#define ROBOT_H

#include <windows.h>
#include <GL/gl.h>		// OpenGL
#include <GL/glu.h>
#include <Springhead.h>		//	Springheadのインタフェース

using namespace Spr;

class Robot{
public:
	PHSolidIf*	soBody;
	CDConvexMeshIf*   boxBody;
	//CDBoxIf*	boxBody;

	class Leg{
	public:
		CDBoxIf			*boxFoot, *boxDX1, *boxDX2; //足先のアルミ板、ダイナミクセル１、２、３
		CDSphereIf		*Sphere;					//足先の球
		//CDBoxIf			*boxCrank, *boxFoot, *boxGuide;
		PHSolidIf		*soFoot, *soDX1, *soDX2, *soSphere;
		//PHSolidIf		*soCrank, *soFoot[2], *soGuide[2];
		PHHingeJointIf	*jntFoot, *jntDX1, *jntDX2, *jntSphere;
		//PHHingeJointIf	*jntCrank, *jntFoot[2], *jntFootGuide[2], *jntGuideBody[2];
		PHRootNodeIf	*DX2;

		void Build(PHSolidIf* body, PHRootNodeIf* root, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk);
		//void Build(PHSolidIf* body, PHHingeJointIf* root, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk);
	};

	Leg	leg[3];

	void Build(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk);
	void Stop();
	void Forward();
	void Backward();
	//void TurnLeft();
	//void TurnRight();
};

#endif
