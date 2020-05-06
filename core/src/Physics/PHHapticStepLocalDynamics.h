#ifndef PH_HAPTICENGINE_LD_H
#define PH_HAPTICENGINE_LD_H

#include <Physics/PHHapticStepMulti.h>

namespace Spr {;
//----------------------------------------------------------------------------
// PHHapticStepLocalDynamics
class PHHapticStepLocalDynamics : public PHHapticStepMulti {
	SPR_OBJECTDEF_NOIF(PHHapticStepLocalDynamics);
protected:
	std::vector<SpatialVector> lastvels;
	UTRef< ObjectStatesIf > states;
	virtual bool IsInterporate() { return false; }

public:
	PHHapticStepLocalDynamics();
	~PHHapticStepLocalDynamics();
	virtual void Step1();
	virtual void Step2();
	virtual void PredictSimulation3D();
	virtual void SyncHaptic2Physic();
	virtual void SyncPhysic2Haptic();
	virtual void StepHapticLoop();
	virtual void LocalDynamics();
	virtual void ReleaseState(PHSceneIf* scene);
};
}
#endif