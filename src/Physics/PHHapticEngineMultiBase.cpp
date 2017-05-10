#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;
//----------------------------------------------------------------------------
// PHHapticLoopImp

void PHHapticLoopImp::UpdateInterface(){
	for(int i = 0; i < NHapticPointers(); i++){
		GetHapticPointer(i)->UpdatePointer();
	}
}
double PHHapticLoopImp::GetPhysicsTimeStep(){
	return engineImp->GetPhysicsTimeStep();
}
double PHHapticLoopImp::GetHapticTimeStep(){
	return engineImp->GetHapticTimeStep();
}
int PHHapticLoopImp::NHapticPointers(){
	return (int)hapticPointers.size();
}
int PHHapticLoopImp::NHapticSolids(){
	return (int)hapticSolids.size();
}
PHHapticPointer* PHHapticLoopImp::GetHapticPointer(int i){
	return hapticPointers[i];
}
PHSolidForHaptic* PHHapticLoopImp::GetHapticSolid(int i){
	return hapticSolids[i];
}
PHSolidPairForHaptic* PHHapticLoopImp::GetSolidPairForHaptic(int i, int j){
	return solidPairs.item(i, j);
}
PHHapticPointers* PHHapticLoopImp::GetHapticPointers(){
	return &hapticPointers;
}
PHSolidsForHaptic* PHHapticLoopImp::GetHapticSolids(){
	return &hapticSolids;
}
PHSolidPairsForHaptic* PHHapticLoopImp::GetSolidPairsForHaptic(){
	return &solidPairs;
}
PHHapticRender* PHHapticLoopImp::GetHapticRender(){
	return engineImp->GetHapticRender();
}

//----------------------------------------------------------------------------
// PHHapticEngineMultiBase
PHHapticEngineMultiBase::PHHapticEngineMultiBase(){
	bSync = false;
	bCalcPhys =  true;
	hapticCount = 1;
	cbBeforeStep = NULL;
	cbAfterStep = NULL;
}

void PHHapticEngineMultiBase::StepHapticLoop(){
	hapticLoop->Step();
	SyncThreads();
}

void PHHapticEngineMultiBase::SyncThreads(){
	if(bSync){ 
		/// 同期の実行
		SyncHapticPointers();			
		SyncHaptic2Physic();
		SyncArrays();
		SyncPhysic2Haptic();
		/// 同期終了処理
		hapticLoop->loopCount = 0;		// HapticLoopのカウント初期化
		bSync = false;					// 同期終了フラグ
	}
	/// 同期失敗時の処理
	#define COUNT_MAX 100
	if(hapticLoop->loopCount > COUNT_MAX) {
		DSTR << "Too Many Loops in HapticLoop!" << std::endl;
		DSTR << "Reset HapticLoop Count" << std::endl;
		hapticLoop->loopCount = 0;
		bSync = false;
	}
	hapticLoop->loopCount++;
	hapticCount++;
}

void PHHapticEngineMultiBase::SyncHapticPointers(){
	for(int i = 0; i < hapticLoop->NHapticPointers(); i++){
		PHHapticPointer* ppointer = GetHapticPointer(i);
		PHHapticPointer* hpointer = hapticLoop->GetHapticPointer(i);
		// haptic側のポインタの状態をphysics側のポインタへ反映
		// physics <-------- haptic
		// 1.ポインタの位置姿勢の同期
		// hapticからの入力をphysicsへ

		//DSTR << "sync ppointer" << ppointer->GetHapticForce() << std::endl;
		//DSTR << "sync hpointer" << hpointer->GetHapticForce() << std::endl;
		ppointer->hiSolidSt = hpointer->hiSolidSt;
		ppointer->UpdatePointer();
		
		// 2.情報の同期
		*(PHHapticPointerHapticSt*)ppointer = *(PHHapticPointerHapticSt*)hpointer;

		// physics側の変更をhaptic側へ反映
		// haptic <--------- physics
		//	TODO hase:Solidのコピーは重いので避けるべきだが、しないとPhysics側で設定したFlagが伝わらなくなる。
		*hpointer = *ppointer;
	}
}

//	TODO hase:	ここも大量のSolidのコピーがあり重そう。
void PHHapticEngineMultiBase::SyncArrays(){
	// haptic <------------- physics
	// Physicsで新しく追加されたオブジェクトをHaptic側にコピー
	// 1.力覚ポインタの増加分
	int hNpointers = hapticLoop->NHapticPointers();	// haptic側のポインタ数
	int pNpointers = NHapticPointers();				// physics側のポインタ数
	PHHapticPointers* hpointers = hapticLoop->GetHapticPointers();
	for(int i = hNpointers; i < pNpointers; i++){
		hpointers->push_back(DBG_NEW PHHapticPointer(*GetHapticPointer(i)));
	}
	//DSTR << "------------" << std::endl;
	//DSTR << pNpointers << hNpointers << std::endl;
	//DSTR << hNpointers << std::endl;

	// 2. Solidの増加分
	const int hNsolids = hapticLoop->NHapticSolids();
	const int pNsolids = NHapticSolids();
	if(hNsolids == pNsolids) return;
	PHSolidsForHaptic* hsolids = hapticLoop->GetHapticSolids();
	for(int i = hNsolids; i < (int)pNsolids; i++){
		hsolids->push_back(DBG_NEW PHSolidForHaptic(*GetHapticSolid(i)));
	}

	// 3. solidPair, shapePairの増加分
	// 3.1 力覚ポインタの増加分
	PHSolidPairsForHaptic* hsolidPairs = hapticLoop->GetSolidPairsForHaptic();
	hsolidPairs->resize(pNsolids, pNpointers);
	for(int i = 0; i < pNsolids; i++){
		for(int j = hNpointers; j < pNpointers; j++){
			PHSolidPairForHaptic* psolidPair = GetSolidPairForHaptic(i, j);
			hsolidPairs->item(i, j) = DBG_NEW PHSolidPairForHaptic(*psolidPair);
		}
	}
	// 3.2 solidの増加分
	for(int i = hNsolids; i < pNsolids; i++){
		for(int j = 0; j < pNpointers; j++){
			PHSolidPairForHaptic* psolidPair = GetSolidPairForHaptic(i, j);
			hsolidPairs->item(i, j) = DBG_NEW PHSolidPairForHaptic(*psolidPair);
		}
	}
	//DSTR << "--------------" << std::endl;
	//DSTR << hapticLoop->NHapticPointers() << std::endl;
	//DSTR << hapticLoop->NHapticSolids() << std::endl;
	//DSTR << hapticLoop->GetHapticPointer(0)->neighborSolidIDs.size() << std::endl;
}


// デバック用コード
// 旧版、PHScene::Step()が内蔵されているので
void PHHapticEngineMultiBase::StepPhysicsSimulation(){
	if (bSync) return;
	if (bCalcPhys){
		/// Before Step Callback
		if (cbBeforeStep) { cbBeforeStep(callbackArg); }

		/// シミュレーションの実行
		engine->GetScene()->Step();
		bCalcPhys = false;

		/// After Step Callback
		if (cbAfterStep) { cbAfterStep(callbackArg); }
	}
	double pdt = GetPhysicsTimeStep();
	double hdt = GetHapticTimeStep();
	if (hapticCount < pdt / hdt) return;
	hapticCount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;	
}

}