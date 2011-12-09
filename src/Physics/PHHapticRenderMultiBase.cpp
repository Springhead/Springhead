#include <Physics/PHHapticRenderMultiBase.h>

namespace Spr{;

double PHHapticLoopImp::GetPhysicsTimeStep(){
	return renderImp->GetPhysicsTimeStep();
}

double PHHapticLoopImp::GetHapticTimeStep(){
	return renderImp->GetHapticTimeStep();
}

PHHapticRenderMultiBase::PHHapticRenderMultiBase(){
	bSync = false;
	bCalcPhys =  true;
	hapticCount = 1;
}


void PHHapticRenderMultiBase::Step(){
}


void PHHapticRenderMultiBase::StepHapticLoop(){
	hapticLoop->Step();
	Sync();
}

void PHHapticRenderMultiBase::Sync(){
	if(bSync){ 
		/// 同期の実行
		RunSync();
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