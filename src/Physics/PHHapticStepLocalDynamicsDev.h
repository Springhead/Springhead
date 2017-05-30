#ifndef PH_HAPTICENGINE_LDDEV_H
#define PH_HAPTICENGINE_LDDEV_H

#include <Physics/PHHapticStepMulti.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticStepLocalDynamics
class PHHapticStepLocalDynamicsDev : public PHHapticStepMulti{
	SPR_OBJECTDEF_NOIF(PHHapticStepLocalDynamicsDev);
protected:
	UTRef<ObjectStatesIf> states;
	std::vector<SpatialVector> lastvels;
	virtual bool IsInterporate() { return false; }

public:
	PHHapticStepLocalDynamicsDev();
	virtual void Step1();
	virtual void Step2();
	virtual void PredictSimulation6D();
	virtual void SyncHaptic2Physic();	
	virtual void SyncPhysic2Haptic();
	virtual void StepHapticLoop();
	virtual void LocalDynamics6D();
	virtual void ReleaseState(PHSceneIf* scene);
};

}

#endif