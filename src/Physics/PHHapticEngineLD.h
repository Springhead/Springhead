#ifndef PH_HAPTICENGINE_LD_H
#define PH_HAPTICENGINE_LD_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopLD
class PHHapticLoopLD : public PHHapticLoopImp{
	SPR_OBJECTDEF_NOIF(PHHapticLoopLD);
public:
	virtual void Step();
	virtual void HapticRendering();
	virtual void LocalDynamics();
};

//----------------------------------------------------------------------------
// PHHapticEngineLD
class PHHapticEngineLD : public PHHapticEngineMultiBase{
	SPR_OBJECTDEF_NOIF(PHHapticEngineLD);
protected:
	PHHapticLoopLD hapticLoopLD;

	std::vector<SpatialVector> lastvels;

public:
	PHHapticEngineLD();
	virtual void Step1();
	virtual void Step2();
	virtual void PredictSimulation3D();
	virtual void SyncHaptic2Physic();	
	virtual void SyncPhysic2Haptic();
};

}

#endif