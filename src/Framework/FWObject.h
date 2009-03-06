/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWOBJECT_H
#define FWOBJECT_H

#include <Springhead.h>
#include <Framework/SprFWObject.h>
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

namespace Spr{;

	class FWObject : public SceneObject, public FWObjectDesc {
    public:
		SPR_OBJECTDEF(FWObject);
		ACCESS_DESC(FWObject);
		FWObjectDesc desc;
		FWObject(const FWObjectDesc& d=FWObjectDesc()); // コンストラクタ
		//
		UTRef< PHSolidIf > phSolid;
		UTRef< GRFrameIf > grFrame;
		UTRef< PHSolidIf > phParentSolid;
		UTRef< PHSolidIf > phAncestorSolid;
		UTRef< PHSolidIf > phAncestorSolid2;
		Affinef			   grParentAffine;
		//
		bool		AddChildObject(ObjectIf* o);
		PHSolidIf*	GetPHSolid(){ return phSolid; }
		void		SetPHSolid(PHSolidIf* s){ phSolid = s; }
		GRFrameIf*	GetGRFrame(){ return grFrame; }
		void		SetGRFrame(GRFrameIf* f){ grFrame = f; }
		PHSolidIf*	GetPHParentSolid(){ return phParentSolid; }
		void		SetPHParentSolid(PHSolidIf* ps){ phParentSolid = ps; }
		PHSolidIf*	GetPHAncestorSolid(){ return phAncestorSolid; }
		void		SetPHAncestorSolid(PHSolidIf* ps){ phAncestorSolid = ps; }
		PHSolidIf*	GetPHAncestorSolid2(){ return phAncestorSolid2; }
		void		SetPHAncestorSolid2(PHSolidIf* ps){ phAncestorSolid2 = ps; }
		Affinef		GetGRParentAffine(){ return grParentAffine; }
		void		SetGRParentAffine(Affinef af){ grParentAffine = af; }


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
