#ifndef PHHAPTICRENDER_IMPULSE_H
#define PHHAPTICRENDER_IMPULSE_H

#include <Physics/PHHapticRenderMultiBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopImpulse
class PHHapticLoopImpulse : public PHHapticLoopImp{
	SPR_OBJECTDEF_NOIF(PHHapticLoopImpulse);
public:
	virtual void Step();
	virtual void HapticRendering();
};

//----------------------------------------------------------------------------
// PHHapticRenderImpulse
class PHHapticRenderImpulse : public PHHapticRenderMultiBase{
	SPR_OBJECTDEF_NOIF(PHHapticRenderImpulse);
protected:
	PHHapticLoopImpulse hapticLoopImpulse;
public:
	PHHapticRenderImpulse();
	virtual void Step();
	virtual void SyncHaptic2Physic();	
	virtual void SyncPhysic2Haptic();	

};

}

#endif