#ifndef FWMULTIRATE_HAPTIC_APP_H
#define FWMULTIRATE_HAPTIC_APP_H

#include <Framework/FWHapticAppBase.h>
#include <Framework/FWHapticBase.h>
#include <Foundation/UTMMTimer.h>

namespace Spr{;

class MultiMediaTimer : public UTMMTimer{
public:
	MultiMediaTimer(unsigned int r, unsigned int i, MMTimerFunc* f){
		Init(r, i, f);
	}
	MultiMediaTimer(){};
	void Init(unsigned int r, unsigned int i, MMTimerFunc* f){
		Resolution(r);
		Interval(i);
		Set(f, NULL);
	}
};

class FWMultiRateHapticApp : public FWHapticAppBase{
protected:
	FWHapticBase hapticProcess;
	volatile int hapticcount;
	bool bSync;
	bool bCalcPhys;
public:
	MultiMediaTimer mTimer;
	FWMultiRateHapticApp();
	virtual void InitCameraView();
	virtual void Idle();
	virtual void SyncHapticProcess();

	FWHapticBase* GetHapticProcess();
};

}
#endif