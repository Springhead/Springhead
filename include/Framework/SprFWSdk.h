#ifndef SPR_FWSDKIF_H
#define SPR_FWSDKIF_H

#include <Framework/SprFWScene.h>
#include <Physics/SprPHSdk.h>
#include <Graphics/SprGRSdk.h>
#include <FileIO/SprFISdk.h>

namespace Spr{

	struct FWSdkDesc{
	};
	
	struct FWSdkIf : ObjectIf {
		IF_DEF(FWSdk);
		virtual FWSceneIf* CreateScene(const FWSceneDesc& desc)=0;
		virtual int NScene() const=0;
		virtual FWSceneIf* GetScene(size_t i)=0;
		virtual PHSdkIf* GetPHSdk()=0;
		virtual GRSdkIf* GetGRSdk()=0;
		virtual FISdkIf* GetFISdk()=0;

		static void SPR_CDECL RegisterSdk();
		static FWSdkIf* SPR_CDECL CreateSdk();
	};
}

#endif
