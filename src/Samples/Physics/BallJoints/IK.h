#ifndef IK_H
#define IK_H

#include <vector>

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Physics/SprPHIK.h>

using namespace Spr;
using namespace PTM;
using namespace std;

class IK : public FWApp{
public:
	FWWin* window;

	Vec3d gravity;

	PHIKPosCtlIf *ikPosCtl1, *ikPosCtl2;
	PHSolidIf *soPosCtl1, *soPosCtl2;

	//デバック表示に使う変数
	bool bGravity;
	bool bDebug;
	bool bStep;
	bool bOneStep;
	bool bIK;

	IK();

	virtual void Init(int argc, char* argv[]);
	virtual void TimerFunc(int id);
	virtual void Step();

	void Reset(int sceneNum=0);
	void InitCameraView();
	void BuildScene(int sceneNum=0);
	void Display();		
	void Keyboard(int key, int x, int y);

	void OneStep();
}; 

extern IK app;

#endif
