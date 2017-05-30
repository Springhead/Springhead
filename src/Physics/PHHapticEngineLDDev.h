#ifndef PH_HAPTICENGINE_LDDEV_H
#define PH_HAPTICENGINE_LDDEV_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticEngineLD
class PHHapticEngineLDDev : public PHHapticEngineMultiBase{
	SPR_OBJECTDEF_NOIF(PHHapticEngineLDDev);
protected:
	UTRef<ObjectStatesIf> states;
	std::vector<SpatialVector> lastvels;
	virtual bool IsInterporate() { return false; }

public:
	PHHapticEngineLDDev();
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