#ifndef PH_HAPTICENGINE_LD_H
#define PH_HAPTICENGINE_LD_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr {;
//----------------------------------------------------------------------------
// PHHapticEngineLD
class PHHapticEngineLD : public PHHapticEngineMultiBase {
	SPR_OBJECTDEF_NOIF(PHHapticEngineLD);
protected:
	std::vector<SpatialVector> lastvels;
	UTRef< ObjectStatesIf > states;
	virtual bool IsInterporate() { return false; }

public:
	PHHapticEngineLD();
	~PHHapticEngineLD();
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