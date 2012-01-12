#include <Physics/PHHapticRenderMultiBase.h>

namespace Spr{;
//----------------------------------------------------------------------------
// PHHapticLoopImp

void PHHapticLoopImp::UpdateInterface(){
	for(int i = 0; i < NHapticPointers(); i++){
		GetHapticPointer(i)->UpdateInterface((float)GetHapticTimeStep());
		GetHapticPointer(i)->UpdateHapticPointer();
	}
}
double PHHapticLoopImp::GetPhysicsTimeStep(){
	return renderImp->GetPhysicsTimeStep();
}
double PHHapticLoopImp::GetHapticTimeStep(){
	return renderImp->GetHapticTimeStep();
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

//PHHapticPointers* PHHapticLoopImp::GetHapticPointers(){
//	return hapticPointers.empty() ? NULL : (PHHapticPointers*)&*hapticPointers.begin();
//}
//
//PHSolidForHaptic** PHHapticLoopImp::GetHapticSolids(){
//	return hapticSolids.empty() ? NULL : (PHSolidForHaptic**)&*hapticSolids.begin();
//}

//----------------------------------------------------------------------------
// PHHapticRenderMultiBase
PHHapticRenderMultiBase::PHHapticRenderMultiBase(){
	bSync = false;
	bCalcPhys =  true;
	hapticCount = 1;
}

void PHHapticRenderMultiBase::StepHapticLoop(){
	hapticLoop->Step();
	Sync();
}

void PHHapticRenderMultiBase::Sync(){
	if(bSync){ 
		/// 同期の実行
		UpdateHapticPointer();			
		SyncHaptic2Physic();
		UpdateArrays();
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

void PHHapticRenderMultiBase::UpdateHapticPointer(){
	for(int i = 0; i < hapticLoop->NHapticPointers(); i++){
		PHHapticPointer* ppointer = GetHapticPointer(i);
		PHHapticPointer* hpointer = hapticLoop->GetHapticPointer(i);
		// physics側の変更をhaptic側へ反映
		// haptic <--------- physics
		// 変数が増えてきたらまとめる
		hpointer->bDebugControl = ppointer->bDebugControl;
		hpointer->bForce = ppointer->bForce;
		hpointer->bVibration = ppointer->bVibration;
		// haptic側のポインタの状態をphysics側のポインタへ反映
		// physics <-------- haptic
		if(ppointer->bDebugControl){
			// physicsからの入力を反映
			*ppointer->hiSolid = *DCAST(PHSolid, ppointer);
			*hpointer->hiSolid = *ppointer->hiSolid;
		}else{
			// hapticからの入力をphysicsへ
			*ppointer->hiSolid = *hpointer->hiSolid;
		}
		ppointer->UpdateHapticPointer();
	}
}

void PHHapticRenderMultiBase::UpdateArrays(){
	// haptic <------------- physics
	// Physicsで新しく追加されたオブジェクトをHaptic側にコピー
	// 1.力覚ポインタの増加分
	int hNpointers = hapticLoop->NHapticPointers();	// haptic側のポインタ数
	int pNpointers = NHapticPointers();				// physics側のポインタ数
	PHHapticPointers* hpointers = hapticLoop->GetHapticPointers();
	for(int i = hNpointers; i < pNpointers; i++){
		hpointers->push_back(DBG_NEW PHHapticPointer());
		*hpointers->back() = *GetHapticPointer(i);
	}

	// 2. Solidの増加分
	const int hNsolids = hapticLoop->NHapticSolids();
	const int pNsolids = NHapticSolids();
	if(hNsolids == pNsolids) return;
	PHSolidsForHaptic* hsolids = hapticLoop->GetHapticSolids();
	for(int i = hNsolids; i < (int)pNsolids; i++){
		hsolids->push_back(DBG_NEW PHSolidForHaptic());
		*hsolids->back() = *GetHapticSolid(i);
	}

	// 3. solidPairの増加分
	UTCombination< UTRef<PHSolidPairForHaptic> >* hsolidPairs = &hapticLoop->solidPairs;
	hsolidPairs->resize(pNpointers, pNsolids);
	// 力覚ポインタ増加分
	for(int i = hNpointers; i < pNpointers; i++){
		for(int j = 0; j < pNsolids; j++){
			hsolidPairs->item(i, j) = DBG_NEW PHSolidPairForHaptic();
			*hsolidPairs->item(i, j) = *GetSolidPairForHaptic(i, j);	
		}
	}
	// solidの増加分
	for(int i = 0; i < pNpointers; i++){
		for(int j = hNsolids; j < pNsolids; j++){
			hsolidPairs->item(i, j) = DBG_NEW PHSolidPairForHaptic();
			*hsolidPairs->item(i, j) = *GetSolidPairForHaptic(i, j);	
		}
	}
	//DSTR << "--------------" << std::endl;
	//DSTR << hapticLoop->NHapticPointers() << std::endl;
	//DSTR << hapticLoop->NHapticSolids() << std::endl;
	//DSTR << hapticLoop->GetHapticPointer(0)->neighborSolidIDs.size() << std::endl;
}

// デバック用コード
// 旧版、PHScene::Step()が内蔵されているので
// ここで呼ぶわけにはいかない
void PHHapticRenderMultiBase::StepSimulation(){
	if (bSync) return;
	if (bCalcPhys){
		/// シミュレーションの実行
		engine->GetScene()->Step();
		bCalcPhys = false;
	}
	double pdt = GetPhysicsTimeStep();
	double hdt = GetHapticTimeStep();
	if (hapticCount < pdt / hdt) return;
	hapticCount -= (int)(pdt/hdt);
	bSync = true;
	bCalcPhys = true;	
}




}