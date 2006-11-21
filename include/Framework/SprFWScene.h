#ifndef SPR_FWSCENEIF_H
#define SPR_FWSCENEIF_H

#include <Foundation/SprScene.h>
#include <Physics/SprPHScene.h>
#include <Graphics/SprGRScene.h>
#include <Graphics/SprGRRender.h>

namespace Spr{

	struct FWSceneDesc{
	};

	class HIForceDevice6D;

	struct FWSceneIf : SceneIf {
		IF_DEF(FWScene);
		virtual PHSceneIf* GetPHScene() = 0;
		virtual GRSceneIf* GetGRScene() = 0;
		virtual void Sync() = 0;
		virtual void Step() = 0;
		virtual void Draw(GRRenderIf* grRender, bool debug=false) = 0;
		virtual void AddHumanInterface(HIForceDevice6D* d)=0;
	};

	FWSceneIf* SPR_CDECL CreateFWScene();
	FWSceneIf* SPR_CDECL CreateFWScene(const void* desc);
}

#endif
