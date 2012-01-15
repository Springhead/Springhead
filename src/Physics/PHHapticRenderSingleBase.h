#ifndef PH_HAPTICRENDER_SINGLEBASE_H
#define PH_HAPTICRENDER_SINGLEBASE_H

#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticRenderSingleBase
class PHHapticRenderSingleBase : public PHHapticRenderImp{
	SPR_OBJECTDEF_NOIF(PHHapticRenderSingleBase);
public:
	PHHapticRenderSingleBase();
	virtual void Step();		// phengine->phhapticengineから呼ばれる
	virtual void UpdateInterface();
	virtual void UpdateHapticPointer();
	virtual void HapticRendering();

	//デバック用コード
	virtual void StepSimulation();
};

}

#endif