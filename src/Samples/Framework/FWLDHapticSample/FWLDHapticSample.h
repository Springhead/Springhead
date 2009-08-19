#ifndef FWLDHAPTIC_SAMPLE_H
#define FWLDHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWAppLDHaptic.h>

using namespace Spr;

class FWLDHapticSample : public FWAppLDHaptic{
public:
	FWLDHapticSample();									
	void InitCameraView();									
	void BuildScene();																			
	void Keyboard(int key, int x, int y);		
}; 

#endif