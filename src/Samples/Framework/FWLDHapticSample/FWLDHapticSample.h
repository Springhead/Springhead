#ifndef FWLDHAPTIC_SAMPLE_H
#define FWLDHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWLDHapticSample : public FWApp{
public:
	FWLDHapticSample();									
	virtual void Init(int argc, char* argv[]);
	void InitCameraView();	
	void InitHumanInterface();
	void IdleFunc();
	static void CallBackPhysicsLoop();
	static void CallBackHapticLoop(void* arg);	
	void Display();									
	void BuildScene();																	
	void Reset();			
	void Keyboard(int key, int x, int y);		
	bool bDrawInfo;									
	void DisplayContactPlane();
	void DisplayLineToNearestPoint();
}; 

#endif