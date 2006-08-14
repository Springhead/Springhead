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

	FWScene::FWScene(const FWSceneDesc& d/*=FWSceneDesc()*/){
		SetNameManager(NameManager::GetRoot());
	}

	FWScene::~FWScene(){
		for (size_t i=0; i<NChildObject(); i++) {
			delete objs[i];
		}
	}

	void FWScene::Link(PHSceneIf* phScene, GRSceneIf* grScene){
		for (size_t i=0; i<NChildObject(); i++) {
			FWObjectIf* obj = DCAST(FWObjectIf, GetChildObject(i));
			if (obj) { obj->Link(phScene, grScene); }
		}
	}

	bool FWScene::AddChildObject(ObjectIf* o){
		objs.push_back(o);
		return true;
	}

	size_t FWScene::NChildObject() const{
		return objs.size();
	}

	ObjectIf* FWScene::GetChildObject(size_t pos){
		return objs[pos];
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
