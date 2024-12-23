#include <Physics/PHHapticStepSingle.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticStepSingle
PHHapticStepSingle::PHHapticStepSingle(){}

PHSolidPairForHaptic* PHHapticStepSingle::GetSolidPairInHapticImp(int i, int j){
	return engine->GetSolidPairImp(i, j);
}
	
void PHHapticStepSingle::Step1(){
	////DSTR << "step" << std::endl;
	engine->StartDetection();
	UpdateHapticPointer();
	engine->HapticRendering(this->Cast());
	//レンダリングした力をシーンに反映
	for(int i = 0; i < (int)NHapticSolids(); i++){
		PHSolidForHaptic* hsolid = (PHSolidForHaptic*)GetHapticSolid(i);
		if(hsolid->bPointer) continue;
		PHSolid* sceneSolid = hsolid->sceneSolid;
		sceneSolid->AddForce(hsolid->force);
		sceneSolid->AddTorque(hsolid->torque);
		hsolid->force.clear();
		hsolid->torque.clear();
	}
}

void PHHapticStepSingle::Step2(){
}

void PHHapticStepSingle::UpdateHapticPointer(){
	for(int i = 0; i < NHapticPointers(); i++){
		((PHHapticPointer* )GetHapticPointer(i))->UpdatePointer();
	}
}


// デバック用コード
// 旧版、PHScene::Step()が内蔵されているので
// ここで呼ぶわけにはいかない
void PHHapticStepSingle::StepSimulation(){
	/// シミュレーションの実行
	engine->GetScene()->Step();
}




}
