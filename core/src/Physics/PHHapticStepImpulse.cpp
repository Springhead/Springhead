#include <Physics/PHHapticStepImpulse.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopImpulse

#if 0
// 1/7はここから
void PHHapticLoopImpulse::Step(){
	UpdateInterface();
	HapticRendering();
}

void PHHapticLoopImpulse::HapticRendering(){
	PHHapticRenderInfo info;
	info.pointers = GetHapticPointers();
	info.hsolids = GetHapticSolids();
	info.sps = GetSolidPairsForHaptic();
	info.hdt = GetHapticTimeStep();
	info.pdt = GetPhysicsTimeStep();
	info.loopCount = loopCount;
	info.bInterpolatePose = true;
	GetHapticRender()->HapticRendering(info);
}
#endif
//----------------------------------------------------------------------------
// PHHapticStepImpulse
PHHapticStepImpulse::PHHapticStepImpulse(){ 
}

void PHHapticStepImpulse::Step1(){};
void PHHapticStepImpulse::Step2(){
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidIf* solid = GetHapticSolid(i)->GetSceneSolid();
		//if(i == 1) //CSVOUT << solid->GetVelocity().y << "," << solid->GetFramePosition().y  << std::endl;
	}
	engine->StartDetection();
}
void PHHapticStepImpulse::StepHapticLoop() {
	UpdateHapticPointer();
	engine->HapticRendering(this->Cast());
}

void PHHapticStepImpulse::SyncHaptic2Physic(){
	// physics <------ haptic
	// PHSolidForHapticの同期
	// PHSolidPairForHaptic(力覚ポインタと近傍の物体)の各種情報の同期
	for(size_t i = 0; i < hapticModel.hapticPointers.size(); i++){
		PHHapticPointer* hpointer = hapticModel.hapticPointers[i];
		int hpointerID = hpointer->GetPointerID();
		int nNeighbors = (int)hpointer->neighborSolidIDs.size();
		// 近傍物体であるペアだけ同期
		for(int j = 0; j < nNeighbors; j++){
			int solidID = hpointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticModel.GetSolidPair(solidID, hpointerID);
			PHSolidPairForHaptic* ppair = (PHSolidPairForHaptic*)engine->GetSolidPair(solidID, hpointerID);
			ppair->CopyFromHaptics(hpair);
		}
	}
	// レンダリングした力をシーンに反映
	double forceScale = GetHapticTimeStep() / GetPhysicsTimeStep();
	for(int i = 0; i < (int)hapticModel.hapticSolids.size(); i++){
		PHSolidForHaptic* hsolid = hapticModel.hapticSolids[i];
		if(hsolid->bPointer) continue;
		/*//	For debug to watch feedback force to each dynamic solid
		if (hsolid->force != Vec3d(0, 0, 0)) {
			DSTR << "HapticStep i=" << i << " f:" << hsolid->force * forceScale << std::endl;
		}
		//	*/

		PHSolid* sceneSolid = hsolid->sceneSolid;
		sceneSolid->AddForce(hsolid->force * forceScale);
		sceneSolid->AddTorque(hsolid->torque * forceScale);
		hsolid->force.clear();
		hsolid->torque.clear();
	}
}

void PHHapticStepImpulse::SyncPhysic2Haptic(){
	// haptic <------ physics
	// PHSolidForHapticの同期
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidForHaptic* psolid = GetHapticSolid(i)->Cast();
		PHSolidForHaptic* hsolid = hapticModel.hapticSolids[i];
		*psolid->GetLocalSolid() = *psolid->GetSceneSolid();	//	impulseの場合は常時sceneで管理されているsolidと同期				
		*hsolid = PHSolidForHaptic(*psolid);			// LocalDynamicsの場合はdosimによって同期情報をかえる必要がある
	}
	// solidpair, shapepairの同期
	// 近傍物体のみ同期させる
	for(int i = 0; i < NHapticPointers(); i++){
		PHHapticPointer* ppointer = GetHapticPointer(i)->Cast();
		const int ppointerID = ppointer->GetPointerID();
		for(size_t j = 0; j < ppointer->neighborSolidIDs.size(); j++){
			const int solidID = ppointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticModel.GetSolidPair(solidID, ppointerID);
			PHSolidPairForHaptic* ppair = engine->GetSolidPair(solidID, ppointerID)->Cast();
			hpair->CopyFromPhysics(ppair);
		}
	}
}

}
