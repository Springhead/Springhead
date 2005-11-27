#ifndef SPR_PHSDKIF_H
#define SPR_PHSDKIF_H
#include <Springhead.h>

namespace Spr{;

struct PHSceneIf;
///	ÉVÅ[Éì
struct PHSdkIf : public ObjectIf{
	virtual PHSceneIf* CreateScene() = NULL;
};
PHSdkIf* CreatePHSdk();

}	//	namespace Spr
#endif
