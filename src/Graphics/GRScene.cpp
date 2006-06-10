#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "GRScene.h"
#include <sstream>

namespace Spr{;

//----------------------------------------------------------------------------
//	GRScene
IF_OBJECT_IMP(GRScene, Scene);

GRScene::GRScene(const GRSceneDesc& desc):GRSceneDesc(desc){
	Init();
}
void GRScene::Init(){
	world = DBG_NEW GRFrame;
	world->SetNameManager(this);
}

GRSdkIf* GRScene::GetSdk(){
	GRSdk* sdk = DCAST(GRSdk, GetNameManager());
	return sdk;
}

void GRScene::Clear(){
	world = NULL;
	Init();
}
GRFrameIf* GRScene::CreateFrame(const GRFrameDesc& desc){
	ObjectIf* o = CreateObject(GRFrameIf::GetIfInfoStatic(), &desc);
	return (GRFrameIf*)o;
}
bool GRScene::AddChildObject(ObjectIf* o){
	GRCamera* c = DCAST(GRCamera, o);
	if (c){
		camera = c;
		return true;
	}
	return world->AddChildObject(o);
}
size_t GRScene::NChildObject() const{
	return world->NChildObject() + (camera ? 1 : 0);
}
ObjectIf* GRScene::GetChildObject(size_t pos){
	if (camera){
		if (pos == 0) return camera->GetIf();
		return world->GetChildObject(pos-1);
	}
	return world->GetChildObject(pos);
}
void GRScene::Render(GRRenderIf* r){
	if (camera){
		if (camera->frame){
			Affinef af = camera->frame->GetWorldTransform();
			r->SetViewMatrix(af.inv());
		}
		r->SetCamera(*camera);
	}
	world->Render(r);
	world->Rendered(r);
}


}
