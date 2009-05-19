#ifndef VIRTUALHUMAN_H
#define VIRTUALHUMAN_H

#include <vector>

#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>
#include <Physics/PHIK.h>
#include <Creature/CRCreature.h>

using namespace Spr;
using namespace PTM;
using namespace std;

class VirtualHuman : public FWAppGLUT, public UTRefCount{
public:
	FWWin* window;

	Vec3d gravity;

	//デバック表示に使う変数
	bool bGravity;
	bool bDebug;
	bool bStep;
	bool bOneStep;
	bool bIK;

	// クリーチャ
	CRCreatureIf                  *creature, *userhand;
	CRBallHumanBodyDesc            descBody;
	CRBallHumanBodyIf             *body, *userhandModel;
	CREyeControllerIf             *eyeCtrl;
	CRNeckControllerIf            *neckCtrl;
	CRTouchSensorIf               *touchSensor;
	CRReachingControllersIf       *reaches;

	VirtualHuman();
	void Init(int argc, char* argv[]);
	void Reset(int sceneNum=0);
	void InitCameraView();
	void BuildScene(int sceneNum=0);
	void Step();
	void Display();		
	void Keyboard(int key, int x, int y);

	void OneStep();
}; 

extern VirtualHuman app;

#endif
