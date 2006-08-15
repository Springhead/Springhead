#include "FWScene.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
	IF_IMP(FWScene, Scene);
	OBJECT_IMP(FWScene, Scene);

	void SPR_CDECL FWRegisterSdk(){
		RegisterSdk(new FWSceneFactory());
	}

	FWScene::FWScene(const FWSceneDesc& d/*=FWSceneDesc()*/)
	: phScene(NULL), grScene(NULL)
	{
		SetNameManager(NameManager::GetRoot());
	}

	FWScene::~FWScene(){
		for (size_t i=0; i<NChildObject(); i++) {
			delete fwObjects[i];
		}
	}

	bool FWScene::AddChildObject(ObjectIf* o){
		bool rv = false;
		if (!rv) {
			FWObjectIf* obj = DCAST(FWObjectIf, o);
			if (obj) {
				fwObjects.push_back(obj);
				rv = true;
			}
		}
		if (!rv) {
			PHSceneIf* obj = DCAST(PHSceneIf, o);
			if (obj) {
				phScene = obj;
				rv = true;
			}
		}
		if (!rv) {
			GRSceneIf* obj = DCAST(GRSceneIf, o);
			if (obj) {
				grScene = obj;
				rv = true;
			}
		}
		return rv;
	}

	size_t FWScene::NChildObject() const{
		return fwObjects.size();
	}

	ObjectIf* FWScene::GetChildObject(size_t pos){
		return fwObjects[pos];
	}

	FWSceneIf* SPR_CDECL CreateFWScene(){
		FWSceneIf* rv = DBG_NEW FWScene;
		return rv;
	}

	FWSceneIf* SPR_CDECL CreateFWScene(const void* desc){
		FWSceneIf* rv = DBG_NEW FWScene(*(FWSceneDesc*)desc);
		return rv;
	}

}
