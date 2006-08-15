#ifndef SPR_FWSCENEIF_H
#define SPR_FWSCENEIF_H

#include <Springhead.h>

namespace Spr{

	struct PHSceneIf;
	struct GRSceneIf;

	struct FWSceneDesc{
	};

	struct FWSceneIf : SceneIf {
		IF_DEF(FWScene);
		virtual PHSceneIf* GetPHScene() = 0;
		virtual GRSceneIf* GetGRScene() = 0;
	};

	FWSceneIf* SPR_CDECL CreateFWScene();
	FWSceneIf* SPR_CDECL CreateFWScene(const void* desc);
	void SPR_CDECL FWRegisterSdk();

}

#endif
