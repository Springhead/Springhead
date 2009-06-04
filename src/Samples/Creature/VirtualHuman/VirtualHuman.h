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

	// ユーザの指
	PHSolidIf* soCursor;
	double zP;

	// クリーチャ
	CRCreatureIf                  *creature, *userhand;
	CRBallHumanBodyDesc            descBody;
	CRBallHumanBodyIf             *body, *userhandModel;
	CRReachingControllersIf       *reaches;

	VirtualHuman();
	void Init(int argc, char* argv[]);
	void Reset(int sceneNum=0);
	void InitCameraView();
	void BuildScene(int sceneNum=0);
	void Step();
	void Display();		
	void Keyboard(int key, int x, int y);
	void MouseButton(int button, int state, int x, int y);
	void MouseMove(int x, int y);

	void OneStep();
	void UpdateCursor(int x, int y);
}; 

extern VirtualHuman app;

#endif
