#ifndef FW_APP_ADAPTER_H
#define FW_APP_ADAPTER_H

#include <conio.h>
#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>
#include <Foundation/UTMMTimer.h>
#include <HumanInterface/SprHIDRUsb.h>
#include "FWAppImp.h"

using namespace Spr;

class FWAppAdapter;

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
	int GetNImp(){return (int)imps.size();}
	int GetCurrentImpID(){ return impId; }
	void AddScene(FWAppImp* scene);
	void SwitchImp(int i);
	void InitHapticDevice();
	void Init(int argc, char* argv[]);
	void InitCameraView();
	void Start();
	void Step();
	void Idle();
	void Display();
	void Keyboard(int key, int x, int y);
};

#endif