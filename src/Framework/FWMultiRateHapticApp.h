#ifndef FWMULTIRATE_HAPTIC_APP_H
#define FWMULTIRATE_HAPTIC_APP_H

#include <Framework/FWHapticAppBase.h>

namespace Spr{;
class FWMultiRateHapticApp : public FWHapticAppBase{
public:
	FWMultiRateHapticApp();
	virtual void InitCameraView();
	virtual void Idle();
	virtual void SyncHapticProcess();
	virtual void ExpandPHSolidInfo();
	virtual void FindNearestObjectFromHapticPointer();
};

}
#endif