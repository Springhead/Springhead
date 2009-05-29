#ifndef FW_APP_ADAPTER_H
#define FW_APP_ADAPTER_H

#include <conio.h>
//#include <GL/glut.h>
#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>
#include <Foundation/UTMMTimer.h>
#include <HumanInterface/SprHIDRUsb.h>
#include "FWAppImp.h"

using namespace Spr;

void MMTimerCallBack(void* arg);

class FWAppAdapter: public FWAppGLUT, public UTRefCount{
protected:
	UTRef<HISdkIf> hisdk;
	UTRef<HISpidarGIf>  spidarG6;
	vector<FWAppImp*> imps;
public:
	UTMMTimer timer;
	FWAppImp* imp;
	int impId;

	bool bDebug;
	bool bHaptic;

	FWAppAdapter();
	int GetNImp();
	int GetCurrentImpID();
	void AddImp(FWAppImp* i);
	void SetCallBackImp(FWAppImp* i);
	void SwitchImp(int i);
	void InitHapticInterface();
	void Init(int argc, char* argv[]);
	void InitCameraView();
	void Start();
	void Step();
	void Idle();
	void Display();
	void Keyboard(int key, int x, int y);
};

#endif