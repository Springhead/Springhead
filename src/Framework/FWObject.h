#ifndef FWOBJECT_H
#define FWOBJECT_H

#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

namespace Spr{

	class FWObject : public InheritNamedObject<FWObjectIf, NamedObject>, public FWObjectDesc {
    public:
		OBJECT_DEF(FWObject);
		ACCESS_DESC(FWObject);
		FWObjectDesc desc;
		FWObject(const FWObjectDesc& d=FWObjectDesc()); // コンストラクタ
		//
		PHSolidIf* phSolid;
		GRFrameIf* grFrame;
		//
		void Link(PHSceneIf* phScene, GRSceneIf* grScene);
		PHSolidIf* GetPHSolid();
		GRFrameIf* GetGRFrame();
    };

}

#endif
