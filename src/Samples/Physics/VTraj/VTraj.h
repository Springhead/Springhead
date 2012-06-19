#ifndef IK_H
#define IK_H

#include <vector>

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Physics/PHIK.h>

using namespace Spr;
using namespace PTM;
using namespace std;

class VTraj : public FWApp{
public:
	FWWin* window;

	Vec3d gravity;

	PHIKPosCtlIf *ikPosCtl1, *ikPosCtl2;
	PHSolidIf *soPosCtl1, *soPosCtl2;

	UTRef<ObjectStatesIf> states;

	struct BJOffset {
		PHBallJointIf* jo;
		Vec3d          offset;
		double         damper;
		Vec3d          vel;
		Quaterniond    goal;
	};

	//デバック表示に使う変数
	bool bGravity;
	bool bDebug;
	bool bStep;
	bool bOneStep;
	bool bIK;

	VTraj();

	virtual void Init(int argc, char* argv[]);
	virtual void Step();

	void Reset(int sceneNum=0);
	void InitCameraView();
	void BuildScene(int sceneNum=0);
	void TimerFunc(int id);
	void Display();		
	void Keyboard(int key, int x, int y);

	void OneStep();
	void CalcOffsetForce();
}; 

extern VTraj app;

#endif
