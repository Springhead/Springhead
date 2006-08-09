#ifndef SPR_FWSCENEIF_H
#define SPR_FWSCENEIF_H

#include <Springhead.h>

namespace Spr{

	struct PHSceneIf;
	struct GRSceneIf;

	struct FWSceneDesc{
		std::string grSceneName;
		std::string phSceneName;
	};

	struct FWSceneIf : SceneIf {
		IF_DEF(FWScene);
		virtual void Link(PHSceneIf* phScene, GRSceneIf* grScene) = 0;
	};

	FWSceneIf* SPR_CDECL CreateFWScene();

}

#endif
