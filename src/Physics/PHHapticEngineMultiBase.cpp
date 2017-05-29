#include <Physics/PHHapticEngineMultiBase.h>

namespace Spr{;
//----------------------------------------------------------------------------
// PHHapticLoopImp
#if 0
void PHHapticLoopImp::UpdateInterface(){	//=> MultiBase::UpdateHapticPointer()
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
#endif
//----------------------------------------------------------------------------
// PHHapticEngineMultiBase
PHHapticEngineMultiBase::PHHapticEngineMultiBase(){
	bSync = false;
	bCalcPhys =  true;
	hapticCount = 1;
	loopCount = 0;
	cbBeforeStep = NULL;
	cbAfterStep = NULL;
}
#if 0
void PHHapticEngineMultiBase::StepHapticLoop(){
	hapticLoop->Step();
}
#endif
void PHHapticEngineMultiBase::StepHapticSync() {
	if (bSync) {
		/// 同期の実行
		SyncHapticPointers();
		SyncHaptic2Physic();
		SyncArrays();
		SyncPhysic2Haptic();
		/// 同期終了処理
		loopCount = 0;			// HapticLoopのカウント初期化
		bSync = false;			// 同期終了フラグ
	}
	/// 同期失敗時の処理
#define COUNT_MAX 100
	if (loopCount > COUNT_MAX) {
		DSTR << "Too Many Loops in HapticLoop!" << std::endl;
		DSTR << "Reset HapticLoop Count" << std::endl;
		loopCount = 0;
		bSync = false;
	}
	loopCount++;
	hapticCount++;
}

void PHHapticEngineMultiBase::SyncHapticPointers(){
	for (size_t i = 0; i < hapticModel.hapticPointers.size(); i++) {
		PHHapticPointer* pPointer = GetHapticPointer(i);
		PHHapticPointer* hPointer = hapticModel.hapticPointers[i];
		// haptic側のポインタの状態をphysics側のポインタへ反映
		// physics <-------- haptic
		// 1.ポインタの位置姿勢の同期
		// hapticからの入力をphysicsへ

		//DSTR << "sync ppointer" << ppointer->GetHapticForce() << std::endl;
		//DSTR << "sync hpointer" << hpointer->GetHapticForce() << std::endl;
		pPointer->hiSolidSt = hPointer->hiSolidSt;
		pPointer->UpdatePointer();
		
		// 2.情報の同期
		*(PHHapticPointerHapticSt*)pPointer = *(PHHapticPointerHapticSt*)hPointer;

		// physics側の変更をhaptic側へ反映
		// haptic <--------- physics
		//	TODO hase:Solidのコピーは重いので避けるべきだが、しないとPhysics側で設定したFlagが伝わらなくなる。
		*hPointer = *pPointer;
	}
}

//	TODO hase:	ここも大量のSolidのコピーがあり重そう。
void PHHapticEngineMultiBase::SyncArrays(){
	// haptic <------------- physics
	// Physicsで新しく追加されたオブジェクトをHaptic側にコピー
	// 1.力覚ポインタの増加分
	int hNpointers = hapticModel.hapticPointers.size();	// haptic側のポインタ数
	int pNpointers = NHapticPointers();				// physics側のポインタ数
	for(int i = hNpointers; i < pNpointers; i++){
		hapticModel.hapticPointers.push_back(DBG_NEW PHHapticPointer(*GetHapticPointer(i)));
	}
	//DSTR << "------------" << std::endl;
	//DSTR << pNpointers << hNpointers << std::endl;
	//DSTR << hNpointers << std::endl;

	// 2. Solidの増加分
	const int hNsolids = hapticModel.hapticSolids.size();
	const int pNsolids = NHapticSolids();
	if(hNsolids == pNsolids) return;
	for(int i = hNsolids; i < (int)pNsolids; i++){
		hapticModel.hapticSolids.push_back(DBG_NEW PHSolidForHaptic(*GetHapticSolid(i)));
	}

	// 3. solidPair, shapePairの増加分
	// 3.1 力覚ポインタの増加分
	hapticModel.solidPairs.resize(pNsolids, pNpointers);
	for(int i = 0; i < pNsolids; i++){
		for(int j = hNpointers; j < pNpointers; j++){
			PHSolidPairForHaptic* psolidPair = (PHSolidPairForHaptic*)engine->GetSolidPair(i, j);
			hapticModel.solidPairs.item(i, j) = DBG_NEW PHSolidPairForHaptic(*psolidPair);
		}
	}
	// 3.2 solidの増加分
	for(int i = hNsolids; i < pNsolids; i++){
		for(int j = 0; j < pNpointers; j++){
			PHSolidPairForHaptic* psolidPair = (PHSolidPairForHaptic*)engine->GetSolidPair(i, j);
			hapticModel.solidPairs.item(i, j) = DBG_NEW PHSolidPairForHaptic(*psolidPair);
		}
	}
	//DSTR << "--------------" << std::endl;
	//DSTR << hapticLoop->NHapticPointers() << std::endl;
	//DSTR << hapticLoop->NHapticSolids() << std::endl;
	//DSTR << hapticLoop->GetHapticPointer(0)->neighborSolidIDs.size() << std::endl;
}
void PHHapticEngineMultiBase::UpdateHapticPointer() {
	for (size_t i = 0; i < hapticModel.hapticPointers.size(); ++i) {
		hapticModel.hapticPointers[i]->UpdatePointer();
	}
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