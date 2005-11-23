#include "Collision.h"
#pragma hdrstop

namespace Spr{;
SGOBJECTIMP(CDPhysicalMaterial, SGObject);

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
