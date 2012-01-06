#include <Physics/PHHapticRenderImpulse.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticLoopImpulse

// 1/7はここから
void PHHapticLoopImpulse::Step(){}



//----------------------------------------------------------------------------
// PHHapticRenderImpulse
void PHHapticRenderImpulse::Step(){
	engine->StartDetection();

	// デバックコード
	//PHHapticPointers* hp = GetHapticPointers();
	//int id = hp[0]->GetID();
	//int N = hp[0]->neighborSolidIDs.size();
	//DSTR << "shapepair" << std::endl;
	//for(int i = 0; i < N; i++){
	//	int a = id;
	//	int b = hp[0]->neighborSolidIDs[i];
	//	if(a > b) std::swap(a, b);
	//	PHSolidPairForHaptic* sh = GetSolidPairForHaptic(a, b);
	//	PHShapePairForHaptic* sp = sh->shapePairs.item(0, 0);
	//	//DSTR << sp->normal << std::endl;
	//	DSTR << hp[0]->neighborSolidIDs[i] <<std::endl;
	//	//DSTR << "------------" << std::endl;
	//}
	//DSTR << "solids" << std::endl;
	//PHSolidForHaptic** sh = GetHapticSolids();
	//for(int i = 0; i < NHapticSolids(); i++){
	//	if(sh[i]->doSim == 1 || sh[i]->doSim == 2) 
	//		DSTR << i << std::endl;
	//}
}

void PHHapticRenderImpulse::SyncHaptic2Physic(){
	// physics <-- haptic
	// 力覚ポインタと近傍の物体について各種情報の同期
	for(int i = 0; i < hapticLoop->NHapticPointers(); i++){
		PHHapticPointer* hpointer = hapticLoop->GetHapticPointer(i);
		int hpointerID = hpointer->GetID();
		int nNeighbors = hpointer->neighborSolidIDs.size();
		// 近傍物体の数
		for(int j = 0; j < nNeighbors; j++){
			int solidID = hpointer->neighborSolidIDs[j];
			int a = hpointerID;
			int b = solidID;
			int bSwapped = false;
			if(a > b){
				std::swap(a, b);
				bSwapped = true;
			}
			PHSolidPairForHaptic* hpair = hapticLoop->GetSolidPairForHaptic(a, b);
			PHSolidPairForHaptic* ppair = GetSolidPairForHaptic(a, b);

			// 力の同期（旧ToPhysic)
			// 同期するべき情報を構造体にしてまとめるべき
			ppair->test_force = hpair->test_force;
			ppair->test_torque = hpair->test_torque;
			ppair->impulse = hpair->impulse;
			ppair->impulsePoints = hpair->impulsePoints;

			// 接触点の数だけ力を剛体に加える
			for(int k = 0; k < hpair->impulsePoints.size(); k++){
				ImpulsePoint ip = hpair->impulsePoints[k];
				if(bSwapped){
					ppair->solid[0]->AddForce(ip.impulse, ip.contactPointW);
				}else{
					ppair->solid[1]->AddForce(ip.impulse, ip.contactPointW);				
				}
			}
			hpair->impulsePoints.clear();
		}
	}
}

void PHHapticRenderImpulse::SyncPhysic2Haptic(){
	// haptic <------ physics
	// solidの同期
	for(int i = 0; i < NHapticSolids(); i++){
		PHSolidForHaptic* psolid = GetHapticSolid(i);
		PHSolidForHaptic* hsolid = hapticLoop->GetHapticSolid(i);
		*hsolid = *psolid; 
	}
	// hapticpointerの同期
	for(int i = 0; i < hapticLoop->NHapticPointers(); i++){
		PHHapticPointer* ppointer = GetHapticPointer(i);
		PHHapticPointer* hpointer = hapticLoop->GetHapticPointer(i);
		const int ppointerID = ppointer->GetID();
		const int nNeighbors = ppointer->neighborSolidIDs.size();
		// solidpair, shapepairの同期
		for(int j = 0; j < nNeighbors; j++){
			const int solidID = hpointer->neighborSolidIDs[j];
			int a = ppointerID;
			int b = solidID;
			int bSwapped = false;
			if(a > b){
				std::swap(a, b);
				bSwapped = true;
			}
			PHSolidPairForHaptic* hpair = hapticLoop->GetSolidPairForHaptic(a, b);
			PHSolidPairForHaptic* ppair = GetSolidPairForHaptic(a, b);
			*hpair = *ppair;
		}
	}
}

}