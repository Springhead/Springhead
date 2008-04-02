/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWObject.h"
#include <Graphics/GRFrame.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWObject::FWObject(const FWObjectDesc& d/*=FWObjectDesc()*/)
: desc(d), phSolid(NULL), grFrame(NULL)
{
}

void FWObject::Sync(){
	if (phSolid && grFrame){
		Affinef af;
		phSolid->GetPose().ToAffine(af);
		DCAST(GRFrame, grFrame)->SetTransform(af);
	}else{
		//DSTR << "Warning: No solid or frame for " << GetName() << ":FWObject." << std::endl;
	}
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
