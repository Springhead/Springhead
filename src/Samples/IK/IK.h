#ifndef IK_H
#define IK_H

#include <vector>

#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>
#include <Physics/PHIK.h>

using namespace Spr;
using namespace PTM;
using namespace std;

class IK : public FWAppGLUT, public UTRefCount{
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
	void Init(int argc, char* argv[]);
	void Reset(int sceneNum=0);
	void InitCameraView();
	void BuildScene(int sceneNum=0);
	void Step();
	void Display();		
	void Keyboard(int key, int x, int y);

	void OneStep();
}; 

extern IK app;

#endif
