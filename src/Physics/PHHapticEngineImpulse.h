#ifndef PHHAPTICENGINE_IMPULSE_H
#define PHHAPTICENGINE_IMPULSE_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticEngineImpulse
class PHHapticEngineImpulse : public PHHapticEngineMultiBase{
protected:
	virtual bool IsInterporate() { return true; }
public:
	SPR_OBJECTDEF_NOIF(PHHapticEngineImpulse);
	PHHapticEngineImpulse();
	virtual void Step1();
	virtual void Step2();
	virtual void StepHapticLoop();
	virtual void SyncHaptic2Physic();
	virtual void SyncPhysic2Haptic();
};

}

#endif