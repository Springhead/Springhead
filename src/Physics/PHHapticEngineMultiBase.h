#ifndef PH_HAPTICENGINE_MULTIBASE_H
#define PH_HAPTICENGINE_MULTIBASE_H

#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRender.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopImp
class PHHapticLoopImp : public SceneObject{
	SPR_OBJECTDEF_NOIF(PHHapticLoopImp);
protected:
	PHHapticPointers hapticPointers;
	PHSolidsForHaptic hapticSolids;
	PHSolidPairsForHaptic solidPairs;
public:
	PHHapticEngineImp* engineImp;
	int loopCount;

	PHHapticLoopImp(){}
	virtual void Step(){};
	virtual void UpdateInterface();
	virtual void HapticRendering(){};

	double GetPhysicsTimeStep();
	double GetHapticTimeStep();
	int NHapticPointers();
	int NHapticSolids();
	PHHapticPointer* GetHapticPointer(int i);
	PHSolidForHaptic* GetHapticSolid(int i);
	PHSolidPairForHaptic* GetSolidPairForHaptic(int i, int j);
	PHHapticPointers* GetHapticPointers();
	PHSolidsForHaptic* GetHapticSolids();
	PHSolidPairsForHaptic* GetSolidPairsForHaptic();
	PHHapticRender* GetHapticRender();
};

//----------------------------------------------------------------------------
// PHHapticEngineMultiBase
class PHHapticEngineMultiBase : public PHHapticEngineImp{
	SPR_OBJECTDEF_NOIF(PHHapticEngineMultiBase);
protected:
	volatile int	hapticCount;
	volatile bool	bSync;
	bool			bCalcPhys;

	PHHapticEngineIf::Callback cbBeforeStep;
	PHHapticEngineIf::Callback cbAfterStep;
	void *callbackArg;

public:
	PHHapticEngineMultiBase();
	virtual void StepHapticLoop();	// hapticloop
	virtual void StepHapticSync();	// hapticloop
	virtual void Step1(){}
	virtual void Step2(){}
	virtual void SyncThreads();
	virtual void SyncHapticPointers(); // hapticスレッド側のhapticpointerの状態をphysicsへ同期
	virtual void SyncArrays();
	virtual void SyncHaptic2Physic(){}	// hapticスレッドからphysicsスレッドへの同期
	virtual void SyncPhysic2Haptic(){}	// physicsスレッドからhapticスレッドへの同期

	//デバック用コード
	virtual void StepPhysicsSimulation();

	virtual bool SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg) {
		cbBeforeStep = f;
		callbackArg  = arg;
		return true;
	}

	virtual bool SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg) {
		cbAfterStep = f;
		callbackArg = arg;
		return true;
	}
};

}

#endif