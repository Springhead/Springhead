#ifndef PH_HAPTICENGINE_LDDEV_H
#define PH_HAPTICENGINE_LDDEV_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopLDDev
class PHHapticLoopLDDev : public PHHapticLoopImp{
	SPR_OBJECTDEF_NOIF(PHHapticLoopLDDev);
public:
	virtual void Step();
	virtual void HapticRendering();
	virtual void LocalDynamics6D();
};

//----------------------------------------------------------------------------
// PHHapticEngineLD
class PHHapticEngineLDDev : public PHHapticEngineMultiBase{
	SPR_OBJECTDEF_NOIF(PHHapticEngineLDDev);
protected:
	PHHapticLoopLDDev hapticLoopLD;
	UTRef<ObjectStatesIf> states;
	std::vector<SpatialVector> lastvels;

public:
	PHHapticEngineLDDev();
	virtual void Step1();
	virtual void Step2();
	virtual void PredictSimulation6D();
	virtual void SyncHaptic2Physic();	
	virtual void SyncPhysic2Haptic();	

};

}

#endif