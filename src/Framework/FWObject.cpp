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

	void FWObject::Link(PHSceneIf* phScene, GRSceneIf* grScene){
		/*
		phScene->FindObject(phSolid, desc.phSolidName);
		grScene->FindObject(grFrame, desc.grFrameName);
		DSTR << phSolid << " <=> " << grFrame << std::endl;
		*/
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
			phSolid = obj;
			rv = true;
		}
		if (!rv) {
			GRFrameIf* obj = DCAST(GRFrameIf, o);
			grFrame = obj;
			rv = true;
		}
		return rv;
	}

}
