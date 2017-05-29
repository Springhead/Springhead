#ifndef PHHAPTICENGINE_IMPULSE_H
#define PHHAPTICENGINE_IMPULSE_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;

#if 0
//----------------------------------------------------------------------------
// PHHapticLoopImpulse
class PHHapticLoopImpulse : public PHHapticLoopImp{
	SPR_OBJECTDEF_NOIF(PHHapticLoopImpulse);
public:
	virtual void Step();
	virtual void HapticRendering();
};
#endif

//----------------------------------------------------------------------------
// PHHapticEngineImpulse
class PHHapticEngineImpulse : public PHHapticEngineMultiBase{
	SPR_OBJECTDEF_NOIF(PHHapticEngineImpulse);
public:
	PHHapticEngineImpulse();
	virtual void Step1();
	virtual void Step2();
	virtual void StepHapticLoop();
	virtual void SyncHaptic2Physic();
	virtual void SyncPhysic2Haptic();
};

}

#endif