#ifndef PH_HAPTICENGINE_SINGLEBASE_H
#define PH_HAPTICENGINE_SINGLEBASE_H

#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRenderBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticEngineSingleBase
class PHHapticEngineSingleBase : public PHHapticRenderBase, public PHHapticEngineImp{
	SPR_OBJECTDEF_NOIF(PHHapticEngineSingleBase);
public:
	PHHapticEngineSingleBase();
	virtual void Step1();
	virtual void Step2();
	virtual void UpdateInterface();
	virtual void UpdateHapticPointer();

	//デバック用コード
	virtual void StepSimulation();
};

}

#endif