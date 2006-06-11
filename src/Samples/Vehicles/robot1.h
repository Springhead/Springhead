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
	void Forward();
	void Backward();
	void TurnLeft();
	void TurnRight();
};

#endif