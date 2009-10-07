#ifndef FWMULTIRATE_HAPTIC_H
#define FWMULTIRATE_HAPTIC_H

#include <Springhead.h>
#include <Framework/FWInteractAdaptee.h>

namespace Spr{;

class FWMultiRateHaptic : public FWInteractAdaptee{
protected:
	volatile int hapticcount;
	volatile bool bSync;
	bool bCalcPhys;
	FWHapticLoopBase* hapticLoop;
	virtual void CreateHapticLoop(){}
public:
	FWMultiRateHaptic();
	FWHapticLoopBase* GetHapticLoop(){ return hapticLoop; }

	virtual void UpdatePointer(){}
	void Sync();
	void SyncPointer();
	virtual void Clear();

	virtual void Init(){
		FWInteractAdaptee::Init();
	}
	virtual void Step(){}

};
}
#endif