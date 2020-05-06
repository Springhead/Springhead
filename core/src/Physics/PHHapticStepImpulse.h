#ifndef PHHAPTICENGINE_IMPULSE_H
#define PHHAPTICENGINE_IMPULSE_H

#include <Physics/PHHapticStepMulti.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticStepImpulse
class PHHapticStepImpulse : public PHHapticStepMulti{
protected:
	virtual bool IsInterporate() { return true; }
public:
	SPR_OBJECTDEF_NOIF(PHHapticStepImpulse);
	PHHapticStepImpulse();
	virtual void Step1();
	virtual void Step2();
	virtual void StepHapticLoop();
	virtual void SyncHaptic2Physic();
	virtual void SyncPhysic2Haptic();
};

}

#endif