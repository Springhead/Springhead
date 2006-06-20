/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
SGIF_OBJECT_IMP(CDPhysicalMaterial, SGObject);

void CDPhysicalMaterialData::InitData(){
	reflexSpring = reflexDamper = frictionSpring = frictionDamper = 1.0f;
	staticFriction = 0.6f;
	dynamicFriction = 0.3f;
}

//	Loader
class CDPhysicalMaterialLoader:public FIObjectLoader<CDPhysicalMaterial>{
	virtual bool LoadData(FILoadScene* ctx, CDPhysicalMaterial* pmat){
		ctx->docs.Top()->GetWholeData(pmat->pMatData);
		return true;
	}
};

//	Saver
class CDPhysicalMaterialSaver:public FIObjectSaver<CDPhysicalMaterial>{
public:
	virtual UTString GetType() const{ return "CDPhysicalMaterial"; }
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CDPhysicalMaterial* pmat){
		doc->SetWholeData(pmat->pMatData);
	}
};
DEF_REGISTER_BOTH(CDPhysicalMaterial);

}
