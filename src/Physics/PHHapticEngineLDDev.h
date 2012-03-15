#ifndef PH_HAPTICENGINE_LD_H
#define PH_HAPTICENGINE_LD_H

#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopLDDev
class PHHapticLoopLDDev : public PHHapticLoopImp{
	SPR_OBJECTDEF_NOIF(PHHapticLoopLDDev);
public:
	virtual void Step();
	virtual void HapticRendering();
	virtual void LocalDynamics();
};

//----------------------------------------------------------------------------
// PHHapticEngineLD
class PHHapticEngineLDDev : public PHHapticEngineMultiBase{
	SPR_OBJECTDEF_NOIF(PHHapticEngineLDDev);
protected:
	PHHapticLoopLDDev hapticLoopLD;
	UTRef<ObjectStatesIf> states, states2;

	Vec3d phvel;

public:
	PHHapticEngineLDDev();
	virtual void Step1();
	virtual void Step2();
	virtual void PredictSimulation3D();
	virtual void SyncHaptic2Physic();	
	virtual void SyncPhysic2Haptic();	

};

}

#endif