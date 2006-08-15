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
		bool AddChildObject(ObjectIf* o);
		PHSolidIf* GetPHSolid();
		GRFrameIf* GetGRFrame();
		ObjectIf* GetChildObject(size_t pos){
			if (pos==0) return phSolid ? (ObjectIf*)phSolid : (ObjectIf*)grFrame;
			if (pos==1) return phSolid ? (ObjectIf*)grFrame : NULL;
		}
		size_t NChildObject() const {
			return phSolid ? (grFrame ? 2 : 1) : (grFrame ? 1 : 0);
		}

		void Sync();
    };

}

#endif
