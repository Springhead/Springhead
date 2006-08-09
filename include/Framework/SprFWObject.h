#ifndef SPR_FWOBJECTIF_H
#define SPR_FWOBJECTIF_H

#include <Springhead.h>

namespace Spr{

	struct FWSceneIf;

	struct FWObjectDesc{
		std::string phSolidName;
		std::string grFrameName;
	};

	struct FWObjectIf : NamedObjectIf {
		IF_DEF(FWObject);
		virtual void Link(PHSceneIf* phScene, GRSceneIf* grScene) = 0;
		virtual PHSolidIf* GetPHSolid() = 0;
		virtual GRFrameIf* GetGRFrame() = 0;
	};

}

#endif
