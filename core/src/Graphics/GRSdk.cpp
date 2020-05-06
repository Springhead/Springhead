/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRSdk.h>
#include <Graphics/GRMesh.h>
#include <Graphics/GRBlendMesh.h>
#include <Graphics/GRSphere.h>
#include <Graphics/GRDeviceGL.h>

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
	Sdk::RegisterFactory(DBG_NEW GRSdkFactory());
	GRRegisterTypeDescs();

	GRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRScene));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRMesh));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(GRMesh));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRBlendMesh));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(GRBlendMesh));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRSphere));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(GRSphere));	
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRMaterial));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(GRMaterial));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRLight));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(GRLight));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRFrame));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRDummyFrame));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRAnimationController));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRAnimationSet));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRAnimation));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRCamera));
}

//----------------------------------------------------------------------------
//	GRSdk
GRSdk::GRSdk(const GRSdkDesc& desc):GRSdkDesc(desc){
	GRSdkIf::RegisterSdk();
}
GRSdk::~GRSdk(){
}

GRRenderIf* GRSdk::CreateRender(){
	GRRender* rv = DBG_NEW GRRender;
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
				sprintf(name, "scene%d", (int)NScene()-1);
				s->SetName(name);
			}
			return true;
		}
		return false;
	}
	Object* obj = DCAST(Object, o);
	assert(obj);
	objects.push_back(obj);
	return true;
}
bool GRSdk::DelChildObject(ObjectIf* o){
	GRScene* s = DCAST(GRScene, o);
	if (s){
		Scenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if (it != scenes.end()){
			scenes.erase(it);
			return true;
		}
		return false;
	}
	Object* obj = DCAST(Object, o);
	assert(obj);
	Objects::iterator it = std::find(objects.begin(), objects.end(), obj);
	if(it != objects.end()){
		objects.erase(it);
		return true;
	}
	return false;
}
void GRSdk::Clear(){
	Sdk::Clear();
	objects.clear();
	scenes.clear();
}

}
