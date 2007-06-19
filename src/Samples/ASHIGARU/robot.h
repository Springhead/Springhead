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
#include <vector>

using namespace Spr;

class Robot{
public:
	PHSolidIf*	soBody;
	CDConvexMeshIf*   boxBody;
	PHRootNodeIf* root;
	PHHingeJointIf* jntConnect;

	class Leg{
	public:
		CDBoxIf			*boxFoot, *boxDX1, *boxDX2; //足先のアルミ板、ダイナミクセル１、２、３
		CDSphereIf		*Sphere;					//足先の球
		PHSolidIf		*soFoot, *soDX1, *soDX2, *soSphere;
		PHHingeJointIf	*jntFoot, *jntDX1, *jntDX2, *jntSphere, *jntConnect[10];

		void Build_root(PHSolidIf* body, PHRootNodeIf* root, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk);
		//void Build_Tree(PHSolidIf* body, PHTreeNodeIf* node_body, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk);

	};

	Leg	leg[3];

	void Build_root(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk);
	void Build_Tree(PHSolidIf* pre_body, PHRootNodeIf* root_node, const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk);
	void Stop();
	void Forward();
	void Backward();
	//void TurnLeft();
	//void TurnRight();
};

#endif
