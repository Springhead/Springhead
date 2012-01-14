#include <Physics/PHHapticRenderImpulse.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopImpulse

// 1/7はここから
void PHHapticLoopImpulse::Step(){
	UpdateInterface();
	HapticRendering();
}

void PHHapticLoopImpulse::HapticRendering(){
	for(int i = 0; i < NHapticPointers(); i++){
		//GetHapticPointer(i)->HapticRendering(GetHapticSolids(), GetSolidPairsForHaptic(), loopCount);
		GetHapticPointer(i)->MultiPointRendering(GetHapticSolids(), GetSolidPairsForHaptic(), loopCount);
	}
}

//----------------------------------------------------------------------------
// PHHapticRenderImpulse
PHHapticRenderImpulse::PHHapticRenderImpulse(){ 
	hapticLoop = &hapticLoopImpulse;
	hapticLoop->renderImp = this;
}

void PHHapticRenderImpulse::Step(){
	//// デバックコード
	//PHHapticPointer* hp = GetHapticPointer(0);
	//int pointerID = hp->GetPointerID();
	//int Nsolids = NHapticSolids();
	//DSTR << pointerID << std::endl;
	//DSTR << "solidpairs" << " " << Nsolids << std::endl;
	//for(int i = 0; i < Nsolids; i++){
	//	PHSolidPairForHaptic* sh = GetSolidPairForHaptic(pointerID, i);
	//	DSTR << sh->solid[0] << " " << sh->solid[1] << std::endl;
	//	DSTR << "shapeparis" << std::endl;
	//	for(int j = 0; j < sh->solid[0]->NShape(); j++)for(int k = 0; k < sh->solid[1]->NShape(); k++){
	//		DSTR << j << " " << k << std::endl;
	//		PHShapePairForHaptic* sp = 	sh->shapePairs.item(j, k);
	//		DSTR << sp << std::endl;
	//	}
	//	//DSTR << hp->neighborSolidIDs[i] <<std::endl;
	//	//DSTR << "------------" << std::endl;
	//}
	//DSTR << "solids" << " " << NHapticSolids() << std::endl;
	//for(int i = 0; i < NHapticSolids(); i++){
	//	PHSolidForHaptic* sh = GetHapticSolid(i);
	//	if(sh->doSim == 1 || sh->doSim == 2) 
	//		DSTR << i << std::endl;
	//}

	engine->StartDetection();
}

void PHHapticRenderImpulse::SyncHaptic2Physic(){
	// physics <-- haptic
	// PHSolidPairForHaptic(力覚ポインタと近傍の物体)の各種情報の同期
	for(int i = 0; i < hapticLoop->NHapticPointers(); i++){
		PHHapticPointer* hpointer = hapticLoop->GetHapticPointer(i);
		int hpointerID = hpointer->GetPointerID();
		int nNeighbors = hpointer->neighborSolidIDs.size();
		// 近傍物体の数
		for(int j = 0; j < nNeighbors; j++){
			int solidID = hpointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticLoop->GetSolidPairForHaptic(solidID, hpointerID);
			PHSolidPairForHaptic* ppair = GetSolidPairForHaptic(hpointerID, solidID);

			// 力の同期（旧ToPhysic)
			// 同期するべき情報を構造体にしてまとめるべき
			ppair->test_force = hpair->test_force;
			ppair->test_torque = hpair->test_torque;
			ppair->impulse = hpair->impulse;
			ppair->impulsePoints = hpair->impulsePoints;

			// 接触点の数だけ力を剛体に加える
			for(int k = 0; k < (int)hpair->impulsePoints.size(); k++){
				ImpulsePoint ip = hpair->impulsePoints[k];
					ppair->solid[1]->AddForce(ip.impulse, ip.contactPointW);				
			}
			hpair->impulsePoints.clear();
		}
	}
}

void PHHapticRenderImpulse::SyncPhysic2Haptic(){
	// haptic <------ physics
	// PHSolidForHapticの同期
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidForHaptic* psolid = GetHapticSolid(i);
		PHSolidForHaptic* hsolid = hapticLoop->GetHapticSolid(i);
		*hsolid = *psolid;	// LocalDynamicsの場合はdosimによって同期情報をかえる必要がある
	}
	// solidpair, shapepairの同期
	// 近傍物体のみ同期させる
	for(int i = 0; i < NHapticPointers(); i++){
		PHHapticPointer* ppointer = GetHapticPointer(i);
		const int ppointerID = ppointer->GetPointerID();
		const int nNeighbors = ppointer->neighborSolidIDs.size();
		for(int j = 0; j < nNeighbors; j++){
			const int solidID = ppointer->neighborSolidIDs[j];
			PHSolidPairForHaptic* hpair = hapticLoop->GetSolidPairForHaptic(solidID, ppointerID);
			PHSolidPairForHaptic* ppair = GetSolidPairForHaptic(solidID, ppointerID);
			*hpair = *ppair;
		}
	}
}

}