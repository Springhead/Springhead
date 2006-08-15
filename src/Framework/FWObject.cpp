#include "FWObject.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
	IF_IMP(FWObject, NamedObject);
	OBJECT_IMP(FWObject, NamedObject);

	FWObject::FWObject(const FWObjectDesc& d/*=FWObjectDesc()*/)
	: desc(d), phSolid(NULL), grFrame(NULL)
	{
	}

	PHSolidIf* FWObject::GetPHSolid(){
		return phSolid;
	}

	GRFrameIf* FWObject::GetGRFrame(){
		return grFrame;
	}

	bool FWObject::AddChildObject(ObjectIf* o){
		bool rv = false;
		if (!rv) {
			PHSolidIf* obj = DCAST(PHSolidIf, o);
			if (obj) {
				phSolid = obj;
				rv = true;
			}
		}
		if (!rv) {
			GRFrameIf* obj = DCAST(GRFrameIf, o);
			if (obj) {
				grFrame = obj;
				rv = true;
			}
		}
		return rv;
	}

}
