/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_HAPTIC_STEP_BASE_H
#define PH_HAPTIC_STEP_BASE_H
#include <Physics/PHHapticEngine.h>
#include <Physics/PHHapticRender.h>

class PHSolidForHaptic;

namespace Spr{;

class PHHapticEngine;
//----------------------------------------------------------------------------
// PHHapticStepBase
class PHHapticStepBase : public SceneObject{
public:
	SPR_OBJECTDEF_ABST_NOIF(PHHapticStepBase);
	PHHapticEngine* engine;
	PHHapticStepBase(){}
	///	物理シミュレーションのdt
	double GetPhysicsTimeStep();
	///	力覚レンダリングのdt
	double GetHapticTimeStep();
	///	
	virtual void Step1(){};
	///	
	virtual void Step2(){};
	virtual void StepHapticLoop() = 0;
	virtual void StepHapticSync() = 0;
	virtual void UpdateHapticPointer() = 0;

	int NHapticPointers();
	int NHapticSolids();
	PHHapticPointer*       GetHapticPointer(int i);
	PHSolidForHaptic*      GetHapticSolid(int i);

	virtual int NPointersInHaptic()=0;
	virtual int NSolidsInHaptic()=0;
	virtual PHHapticPointer* GetPointerInHaptic(int i)=0;
	virtual PHSolidForHaptic* GetSolidInHaptic(int i)=0;

	virtual PHSolidPairForHaptic* GetSolidPairInHaptic(int i, int j)=0;
	virtual void ReleaseState(PHSceneIf* scene) {}

	///< 剛体と力覚ポインタのペアを取得する（i:剛体、j:力覚ポインタ）
	// iには力覚ポインタも含まれる。
	PHHapticRender*        GetHapticRender();

	///< デバック用シミュレーション実行
	virtual void StepPhysicsSimulation();
	/// シミュレーションを実行する直前に実行されるコールバックを登録する
	virtual bool SetCallbackBeforeStep(PHHapticEngineIf::Callback f, void* arg);
	/// シミュレーションを実行した直後に実行されるコールバックを登録する
	virtual bool SetCallbackAfterStep(PHHapticEngineIf::Callback f, void* arg);
	///	物理ステップの中の何度目のHapticStepかを返す
	virtual int GetLoopCount() = 0;
	///	中間表現を補間する場合 true
	virtual bool IsInterporate() = 0;
};

}
#endif
