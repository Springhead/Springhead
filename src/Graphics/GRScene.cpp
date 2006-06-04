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

GRScene::GRScene(GRSdkIf* s, const GRSceneDesc& desc):GRSceneDesc(desc){
	Init();
	sdk = s;
}
GRScene::GRScene(){
	Init();
}
void GRScene::Init(){
	Scene::Clear();
}

GRSdkIf* GRScene::GetSdk(){
	return sdk;
}

void GRScene::Clear(){
	world = NULL;
	Init();
}
GRFrameIf* GRScene::CreateFrame(const GRFrameDesc& desc){
	GRFrame* fr = DBG_NEW GRFrame;
	frames.push_back(fr);
	return fr;
}
size_t GRScene::NChildObject() const{
	return frames.size();
}
ObjectIf* GRScene::GetChildObject(size_t pos){
	return (GRFrameIf*)frames[pos];
}


}
