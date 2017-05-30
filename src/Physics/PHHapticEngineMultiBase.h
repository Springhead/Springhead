#ifndef PH_HAPTICENGINE_MULTIBASE_H
#define PH_HAPTICENGINE_MULTIBASE_H

#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRender.h>

namespace Spr{;

struct PHHapticRenderingModel{
	PHHapticPointers hapticPointers;
	PHSolidsForHaptic hapticSolids;
	PHContactDetector::PHSolidPairs solidPairs;
	PHSolidPairForHaptic* GetSolidPair(int i, int j) { return (PHSolidPairForHaptic*)&*solidPairs.item(i, j); }
};
//----------------------------------------------------------------------------
// PHHapticEngineMultiBase
class PHHapticEngineMultiBase : public PHHapticEngineImp{
	SPR_OBJECTDEF_ABST_NOIF(PHHapticEngineMultiBase);
protected:
	PHHapticRenderingModel hapticModel;	//	力覚レンダリングが使うモデル
	volatile int	hapticCount;		//	hapticCount - physicsCount*倍率、同期の要求を出すために使う
	volatile int	loopCount;			//	現在の物理ステップに対して何回目の力覚レンダリングか
	volatile bool	bSync;				//	同期の要求
	bool			bCalcPhys;

	PHHapticEngineIf::Callback cbBeforeStep;
	PHHapticEngineIf::Callback cbAfterStep;
	void *callbackArg;
	virtual int GetLoopCount() { return loopCount; }

public:
	PHHapticEngineMultiBase();
	virtual void StepHapticLoop()=0;	// hapticloop
	virtual void StepHapticSync();		// hapticloop
	virtual void Step1(){}
	virtual void Step2(){}
	virtual void SyncHapticPointers(); // hapticスレッド側のhapticpointerの状態をphysicsへ同期
	virtual void SyncArrays();
	virtual void SyncHaptic2Physic(){}	// hapticスレッドからphysicsスレッドへの同期
	virtual void SyncPhysic2Haptic(){}	// physicsスレッドからhapticスレッドへの同期
	virtual void UpdateHapticPointer();
	virtual int NHapticSolidsHaptic() { return (int)hapticModel.hapticSolids.size(); }
	virtual int NHapticPointersHaptic() { return (int)hapticModel.hapticPointers.size(); }
	virtual PHHapticPointer* GetHapticPointerHaptic(int i) { return hapticModel.hapticPointers[i]; }
	virtual PHSolidForHaptic* GetHapticSolidHaptic(int i) { return hapticModel.hapticSolids[i]; }
	virtual PHSolidPairForHaptic* GetSolidPairInHaptic(int i, int j) { return (PHSolidPairForHaptic*)&*hapticModel.solidPairs.item(i, j); }
	virtual PHHapticPointers* GetHapticPointersInHaptic() { return &hapticModel.hapticPointers; }
	virtual PHSolidsForHaptic* GetHapticSolidsInHaptic() { return &hapticModel.hapticSolids; }
	virtual PHContactDetector::PHSolidPairs* GetSolidPairsInHaptic() { return &hapticModel.solidPairs; }
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