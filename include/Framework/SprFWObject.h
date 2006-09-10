#ifndef SPR_FWOBJECTIF_H
#define SPR_FWOBJECTIF_H

#include <Foundation/SprScene.h>
#include <Physics/SprPHSolid.h>
#include <Graphics/SprGRFrame.h>

namespace Spr{

	struct FWSceneIf;

	struct FWObjectDesc{
	};

	struct FWObjectIf : NamedObjectIf {
		IF_DEF(FWObject);
		virtual PHSolidIf* GetPHSolid() = 0;
		virtual GRFrameIf* GetGRFrame() = 0;
	};

}

#endif
