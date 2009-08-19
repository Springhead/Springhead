#ifndef FWAPPLDHAPTIC_H
#define FWAPPLDHAPTIC_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWAppLDHaptic : public FWApp{
public:
	FWAppLDHaptic();									
	virtual void Init(int argc, char* argv[]);
	virtual void InitCameraView();	
	virtual void InitHumanInterface();
	virtual void IdleFunc();
	static void CallBackPhysicsLoop();
	static void CallBackHapticLoop(void* arg);	
	virtual void Display();									
	virtual void BuildScene()=0;																	
	virtual void Reset();			
	virtual void Keyboard(int key, int x, int y){};		
	bool bDrawInfo;									
	void DisplayContactPlane();
	void DisplayLineToNearestPoint();
}; 

#endif