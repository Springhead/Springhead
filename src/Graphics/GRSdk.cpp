/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/Graphics.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "GRScene.h"
#include "GRMesh.h"
#include "GRBlendMesh.h"
#include "GRSphere.h"
#include "GRFrame.h"

namespace Spr{;
GRSdkIf* SPR_CDECL GRSdkIf::CreateSdk(){
	GRSdk* rv = DBG_NEW GRSdk;
	return rv->Cast();
}

void SPR_CDECL GRRegisterTypeDescs();
void SPR_CDECL GRSdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	Sdk::RegisterFactory(new GRSdkFactory());
	GRRegisterTypeDescs();

	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRScene));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMesh));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMesh));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRBlendMesh));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRBlendMesh));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRSphere));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRSphere));	
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMaterial));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMaterial));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRLight));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRLight));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRFrame));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRAnimationController));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRAnimationSet));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRAnimation));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRCamera));
}

//----------------------------------------------------------------------------
//	GRSdk
IF_OBJECT_IMP(GRSdk, Sdk);
GRSdk::GRSdk(const GRSdkDesc& desc):GRSdkDesc(desc){
	GRSdkIf::RegisterSdk();
}
GRDebugRenderIf* GRSdk::CreateDebugRender(){
	GRDebugRender* rv = DBG_NEW GRDebugRender;
	objects.push_back(rv);
	return rv->Cast();
}
GRDeviceGLIf* GRSdk::CreateDeviceGL(){
	GRDeviceGL* rv = DBG_NEW GRDeviceGL();
	objects.push_back(rv);
	return rv->Cast();
}
GRSceneIf* GRSdk::CreateScene(const GRSceneDesc& desc){
	GRSceneIf* rv = (GRSceneIf*)CreateObject(GRSceneIf::GetIfInfoStatic(), &desc);
	AddChildObject(rv);
	return rv;
}
GRSceneIf* GRSdk::GetScene(size_t i){
	if (i<scenes.size()) return scenes[i]->Cast();
	return NULL;
}
void GRSdk::MergeScene(GRSceneIf* scene0, GRSceneIf* scene1){
	Scenes::iterator it0, it1;
	it0 = find(scenes.begin(), scenes.end(), (GRScene*)(scene0->Cast()));
	it1 = find(scenes.begin(), scenes.end(), (GRScene*)(scene1->Cast()));
	if(it0 == scenes.end() || it1 == scenes.end())
		return;
	for(int i = 0; i < (int)scene1->NChildObject(); i++){
		scene0->AddChildObject(scene1->GetChildObject(i));
	}
	scenes.erase(it1);
}

ObjectIf* GRSdk::GetChildObject(size_t i){
	if (i<scenes.size()) return scenes[i]->Cast();
	i -= scenes.size();
	if (i<objects.size()) return objects[i]->Cast();
	return NULL;
}
bool GRSdk::AddChildObject(ObjectIf* o){
	GRScene* s = DCAST(GRScene, o);
	if (s){
		Scenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if (it == scenes.end()){
			scenes.push_back(s);
			// デフォルトネーム
			if(strcmp(s->GetName(), "") == 0){
				char name[256];
				sprintf(name, "scene%d", NScene()-1);
				s->SetName(name);
			}
			return true;
		}
	}
	Object* obj = DCAST(Object, o);
	assert(obj);
	objects.push_back(obj);
	return true;
}
void GRSdk::Clear(){
	Sdk::Clear();
	objects.clear();
	scenes.clear();
}

}
