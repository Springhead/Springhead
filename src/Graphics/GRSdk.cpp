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

namespace Spr{;
GRSdkIf* SPR_CDECL GRSdkIf::CreateSdk(){
	GRSdkIf* rv = DBG_NEW GRSdk;
	return rv;
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
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMaterial));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMaterial));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRTexture));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRTexture));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRLight));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRLight));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRFrame));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRCamera));
}

//----------------------------------------------------------------------------
//	GRSdk
IF_OBJECT_IMP(GRSdk, Sdk);
GRSdk::GRSdk(const GRSdkDesc& desc):GRSdkDesc(desc){
	RegisterSdk();
}
GRDebugRenderIf* GRSdk::CreateDebugRender(){
	GRDebugRender* rv = DBG_NEW GRDebugRender;
	objects.push_back(rv);
	return rv;
}
GRDeviceGLIf* GRSdk::CreateDeviceGL(int w){
	GRDeviceGL* rv = DBG_NEW GRDeviceGL(w);
	objects.push_back(rv);
	return rv;
}
GRSceneIf* GRSdk::CreateScene(){
	GRSceneIf* rv = (GRSceneIf*)CreateObject(GRSceneIf::GetIfInfoStatic(), &GRSdkDesc());
	AddChildObject(rv);
	return rv;
}
GRSceneIf* GRSdk::GetScene(size_t i){
	if (i<scenes.size()) return scenes[i];
	return NULL;
}
ObjectIf* GRSdk::GetChildObject(size_t i){
	if (i<scenes.size()) return scenes[i]->GetIf();
	i -= scenes.size();
	if (i<objects.size()) return objects[i]->GetIf();
	return NULL;
}
bool GRSdk::AddChildObject(ObjectIf* o){
	GRScene* s = DCAST(GRScene, o);
	if (s){
		GRScenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if (it == scenes.end()){
			scenes.push_back(s);
			return true;
		}
	}
	Object* obj = DCAST(Object, o);
	assert(obj);
	objects.push_back(obj);
	return true;
}

}
