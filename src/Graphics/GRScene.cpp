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
	frames.push_back(world);
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
	GRFrame* fr = DBG_NEW GRFrame(desc);
	frames.push_back(fr);
	return fr;
}
ObjectIf* GRScene::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Scene::CreateObject(info, desc);
	if (rv) return rv;
	if (info == GRFrameIf::GetIfInfoStatic()){
		rv = CreateFrame(*(GRFrameDesc*)desc);
	}
	return rv;
}
bool GRScene::AddChildObject(ObjectIf* o){
	return world->AddChildObject(o);
}
size_t GRScene::NChildObject() const{
	return world->NChildObject();
}
ObjectIf* GRScene::GetChildObject(size_t pos){
	return world->GetChildObject(pos);
}
void GRScene::Render(GRRenderIf* r){
	world->Render(r);
}


}
