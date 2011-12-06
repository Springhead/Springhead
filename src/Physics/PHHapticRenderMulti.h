#ifndef PHHAPTICRENDER_MULTI_IMP
#define PHHAPTICRENDER_MULTI_IMP

#include <Physics/PHHapticEngine.h>

namespace Spr{;

class PHHapticRenderMulti : public PHHapticRenderImp{
public:
	virtual void Step(){
		DSTR << "hogeshi" << std::endl;
	};
};

}

#endif