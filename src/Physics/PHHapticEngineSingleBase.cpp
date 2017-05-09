#include <Physics/PHHapticEngineSingleBase.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticEngineSingleBase
PHHapticEngineSingleBase::PHHapticEngineSingleBase(){}

void PHHapticEngineSingleBase::Step1(){
	////DSTR << "step" << std::endl;
	engine->StartDetection();
	UpdateHapticPointer();
	PHHapticRenderInfo info;
	info.pointers = GetHapticPointers();
	info.hsolids = GetHapticSolids();
	info.sps = GetSolidPairsForHaptic();
	info.hdt = GetPhysicsTimeStep();
	info.pdt = GetPhysicsTimeStep();
	info.loopCount = 1.0;
	info.bInterpolatePose = false;
	GetHapticRender()->HapticRendering(info);
	//レンダリングした力をシーンに反映
	for(int i = 0; i < (int)NHapticSolids(); i++){
		PHSolidForHaptic* hsolid = GetHapticSolid(i);
		if(hsolid->bPointer) continue;
		PHSolid* sceneSolid = hsolid->sceneSolid;
		sceneSolid->AddForce(hsolid->force);
		sceneSolid->AddTorque(hsolid->torque);
		hsolid->force.clear();
		hsolid->torque.clear();
	}
}

void PHHapticEngineSingleBase::Step2(){
}

void PHHapticEngineSingleBase::UpdateHapticPointer(){
	for(int i = 0; i < NHapticPointers(); i++){
		GetHapticPointer(i)->UpdatePointer();
	}
}


// デバック用コード
// 旧版、PHScene::Step()が内蔵されているので
// ここで呼ぶわけにはいかない
void PHHapticEngineSingleBase::StepSimulation(){
	/// シミュレーションの実行
	engine->GetScene()->Step();
}




}