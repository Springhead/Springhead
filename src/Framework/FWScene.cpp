#include "FWScene.h"
#include "FWObject.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
	IF_IMP(FWScene, Scene);
	OBJECT_IMP(FWScene, Scene);

	FWScene::FWScene(const FWSceneDesc& d/*=FWSceneDesc()*/)
	: phScene(NULL), grScene(NULL)
	{
	}

	ObjectIf* FWScene::CreateObject(const IfInfo* info, const void* desc){
		ObjectIf* rv = baseType::CreateObject(info, desc);
		if (!rv && phScene) rv = phScene->CreateObject(info, desc);
		if (!rv && grScene) rv = grScene->CreateObject(info, desc);
		return rv;
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
		if (!rv && phScene) {
			rv = phScene->AddChildObject(o);
		}
		if (!rv && grScene) {
			rv = grScene->AddChildObject(o);
		}
		return rv;
	}

	size_t FWScene::NChildObject() const{
		return fwObjects.size() + (grScene?1:0) + (phScene?1:0);
	}

	ObjectIf* FWScene::GetChildObject(size_t pos){
		if (pos < fwObjects.size()) return fwObjects[pos];
		if (pos - fwObjects.size() == 0) return phScene ? (ObjectIf*)phScene : (ObjectIf*)grScene;
		if (pos - fwObjects.size() == 1) return phScene ? grScene : NULL;
		return NULL;
	}

	FWSceneIf* SPR_CDECL CreateFWScene(){
		FWSceneIf* rv = DBG_NEW FWScene;
		return rv;
	}

	FWSceneIf* SPR_CDECL CreateFWScene(const void* desc){
		FWSceneIf* rv = DBG_NEW FWScene(*(FWSceneDesc*)desc);
		return rv;
	}
	void FWScene::Sync(){
		typedef std::vector< UTRef<FWObjectIf> > FWObjects;
		for(FWObjects::iterator it = fwObjects.begin(); it!=fwObjects.end(); ++it){
			DCAST(FWObject, *it)->Sync();
		}
	}
	void FWScene::Step(){
		phScene->Step();
	}
	void FWScene::Draw(GRRenderIf* grRender, bool debug/*=false*/){
		Sync();
		if (debug){
			// GRRenderGLIfを使うか、GRRenderIfにメソッド追加か
			// あとでなんとかする mitake
			/*
			GRCameraIf* cam = NULL;
			if (grScene) cam = grScene->GetCamera();
			if (cam) cam->Render(grRender);
			
			GRLightDesc ld;
			ld.diffuse = Vec4f(1,1,1,1) * 0.8;
			ld.specular = Vec4f(1,1,1,1) * 0.8;
			ld.ambient = Vec4f(1,1,1,1) * 0.4;
			ld.position = Vec4f(1,1,1,0);
			grRender->PushLight(ld);
			grRender->DrawScene(phScene);
			grRender->PopLight();
			if (cam) cam->Rendered(grRender);
			*/
		}else{
			grScene->Render(grRender);
		}
	}
}
