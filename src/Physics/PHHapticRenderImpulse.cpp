#include <Physics/PHHapticRenderImpulse.h>

namespace Spr{;

void PHHapticRenderImpulse::Step(){
	engine->StartDetection();
	PHHapticPointer** ps = GetHapticPointers();
	int id = ps[0]->GetID();
	int N = ps[0]->neighborSolidIDs.size();
	for(int i = 0; i < N; i++){
		int a = id;
		int b = i;
		if(a > b) std::swap(a, b);
		PHSolidPairForHaptic* sh = GetSolidPairForHaptic(a, b);
		PHShapePairForHaptic* sp = sh->shapePairs.item(0, 0);
		//DSTR << sp->normal << std::endl;
		//DSTR << "------------" << std::endl;
	}
}

}