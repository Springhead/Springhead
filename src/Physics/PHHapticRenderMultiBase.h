#ifndef PHHAPTICRENDER_MULTIBASE_H
#define PHHAPTICRENDER_MULTIBASE_H

#include <Physics/PHHapticEngine.h>

namespace Spr{;


class PHHapticLoopImp : public SceneObject{
	SPR_OBJECTDEF_NOIF(PHHapticLoopImp);
public:
	PHHapticRenderImp* renderImp;
	int loopCount;
	PHHapticLoopImp(){}
	virtual void Step(){};

	double GetPhysicsTimeStep();
	double GetHapticTimeStep();
};


class PHHapticRenderMultiBase : public PHHapticRenderImp{
	SPR_OBJECTDEF_NOIF(PHHapticRenderMultiBase);
protected:
	volatile int	hapticCount;
	volatile bool	bSync;
	bool			bCalcPhys;

public:
	PHHapticLoopImp* hapticLoop;
	PHHapticRenderMultiBase();
	virtual void Step();
	virtual void StepSimulation();

	virtual void StepHapticLoop();
	void Sync();
	virtual void RunSync(){}
};

}

#endif