#ifndef ROBOT2_H
#define ROBOT2_H

#include <Springhead.h>		//	Springheadのインタフェース

using namespace Spr;

class Robot2{
public:
	PHSolidIf*	soBody;
	CDBoxIf*	boxBody;

	class Leg{
	public:
		CDBoxIf			*boxCrank, *boxFoot, *boxGuide;
		PHSolidIf		*soCrank, *soFoot[2], *soGuide[2];
		PHHingeJointIf	*jntCrank, *jntFoot[2], *jntFootGuide[2], *jntGuideBody[2];
		void Build(PHSolidIf* body, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk);
	};

	Leg	leg[4];

	void Build(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk);
	void Forward();
	void Backward();
	void TurnLeft();
	void TurnRight();
};

#endif