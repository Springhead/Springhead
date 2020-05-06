#ifndef PH_HAPTICENGINE_SINGLEBASE_H
#define PH_HAPTICENGINE_SINGLEBASE_H

#include <Physics/PHHapticStepBase.h>

namespace Spr{;

/** PHHapticStepSingle
	シングルスレッド版のHapticEngineの実装、PhysicsのStep()をMultimedia timerでまわすと、
	Graphics描画処理Display()やKey入力処理Keyboard()に対して割り込むので危なっかしいが、
	Solid数は変わらないのでなんとかなる。
	ただし、SolidPairなどあたり判定関係の表示をすると落ちるので注意が必要。
*/	
class PHHapticStepSingle : public PHHapticStepBase{
	SPR_OBJECTDEF_NOIF(PHHapticStepSingle);
	virtual bool IsInterporate() { return false; }
	virtual int GetLoopCount() { return 1; }
public:
	PHHapticStepSingle();
	virtual void Step1();
	virtual void Step2();
	virtual void UpdateHapticPointer();
	virtual void StepHapticLoop() {}
	virtual void StepHapticSync() {}
	PHHapticRender* GetHapticRender(){
		return engine->hapticRender;
	}
	virtual int NSolidsInHaptic() { return NHapticSolids(); }
	virtual int NPointersInHaptic() { return NHapticPointers(); }
	virtual PHHapticPointer* GetPointerInHaptic(int i) { return GetHapticPointer(i); }
	virtual PHSolidForHaptic* GetSolidInHaptic(int i) { return GetHapticSolid(i); }
	virtual PHSolidPairForHaptic* GetSolidPairInHaptic(int i, int j) { return (PHSolidPairForHaptic*)engine->GetSolidPair(i, j); }

	//デバック用コード
	virtual void StepSimulation();
};

}

#endif