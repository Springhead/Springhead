#ifndef FWOBJECT_H
#define FWOBJECT_H

#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>
#include "IfStubFramework.h"

namespace Spr{;

	class FWObject : public NamedObject, public FWObjectIfInit, public FWObjectDesc {
    public:
		OBJECTDEF(FWObject, NamedObject);
		ACCESS_DESC(FWObject);
		FWObjectDesc desc;
		FWObject(const FWObjectDesc& d=FWObjectDesc()); // コンストラクタ
		//
		PHSolidIf* phSolid;
		GRFrameIf* grFrame;
		//
		bool AddChildObject(ObjectIf* o);
		PHSolidIf* GetPHSolid(){ return phSolid; }
		void SetPHSolid(PHSolidIf* s){ phSolid = s; }
		GRFrameIf* GetGRFrame(){ return grFrame; }
		void SetGRFrame(GRFrameIf* f){ grFrame = f; }

		ObjectIf* GetChildObject(size_t pos){
			if (pos==0) if (phSolid) return phSolid; else return grFrame;
			if (pos==1) if (phSolid) return grFrame; else return NULL;
			return NULL;
		}
		size_t NChildObject() const {
			return phSolid ? (grFrame ? 2 : 1) : (grFrame ? 1 : 0);
		}

		void Sync();
    };

}

#endif
