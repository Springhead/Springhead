#ifndef PH_HAPTICENGINE_SINGLEBASE_H
#define PH_HAPTICENGINE_SINGLEBASE_H

#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRender.h>
#include <Physics/PHContactDetector.h>

namespace Spr{;

/** PHHapticEngineSingleBase
	シングルスレッド版のHapticEngineの実装、PhysicsのStep()をMultimedia timerでまわすと、
	Graphics描画処理Display()やKey入力処理Keyboard()に対して割り込むので危なっかしいが、
	Solid数は変わらないのでなんとかなる。
	ただし、SolidPairなどあたり判定関係の表示をすると落ちるので注意が必要。
*/	
class PHHapticEngineSingleBase : public PHHapticEngineImp{
	SPR_OBJECTDEF_NOIF(PHHapticEngineSingleBase);
	virtual bool IsInterporate() { return false; }
	virtual int GetLoopCount() { return 1; }
public:
	PHHapticEngineSingleBase();
	virtual void Step1();
	virtual void Step2();
	virtual void UpdateHapticPointer();
	virtual void StepHapticLoop() {}
	virtual void StepHapticSync() {}
	PHHapticRender* GetHapticRender(){
		return engine->hapticRender;
	}
	virtual int NHapticSolidsHaptic() { return NHapticSolids(); }
	virtual int NHapticPointersHaptic() { return NHapticPointers(); }
	virtual PHHapticPointer* GetHapticPointerHaptic(int i) { return GetHapticPointer(i); }
	virtual PHSolidForHaptic* GetHapticSolidHaptic(int i) { return GetHapticSolid(i); }
	virtual PHSolidPairForHaptic* GetSolidPairInHaptic(int i, int j) { return (PHSolidPairForHaptic*)engine->GetSolidPair(i, j); }
	virtual PHHapticPointers* GetHapticPointersInHaptic() { return &engine->hapticPointers; }
	virtual PHSolidsForHaptic* GetHapticSolidsInHaptic() { return &engine->hapticSolids; }
	virtual PHContactDetector::PHSolidPairs* GetSolidPairsInHaptic() { return &engine->solidPairs;  }

	//デバック用コード
	virtual void StepSimulation();
};

}

#endif