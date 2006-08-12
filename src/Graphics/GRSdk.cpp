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
void SPR_CDECL GRRegisterTypeDescs();

struct GRSdks{
	typedef std::vector< GRSdkIf* > Cont;
	Cont* cont;

	GRSdks(){
#if defined _MSC_VER && _DEBUG
		// ‚±‚ÌŠÖ”‚ðŒÄ‚ñ‚¾Žž“_‚ÅŠJ•ú‚³‚ê‚Ä‚¢‚È‚¢ƒƒ‚ƒŠ‚Ìî•ñ‚ð•\Ž¦‚·‚é
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif		
		cont = DBG_NEW Cont;
	}
	~GRSdks(){
		while(cont->size()){
			GRSdkIf* sdk = cont->back();
			cont->erase(cont->end()-1);
			sdk->DelRef();
			if (sdk->RefCount()==0){
				delete sdk;
			}
		}
		delete cont;
	}
};
static GRSdks sdks;
GRSdkIf* SPR_CDECL CreateGRSdk(){
	GRSdkIf* rv = DBG_NEW GRSdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}

void SPR_CDECL GRRegisterFactories(){
	bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRScene));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMesh));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMesh));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMaterial));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRMaterial));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRLight));
	GRSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRLight));
	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRFrame));

	GRSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRCamera));
}

void SPR_CDECL GRRegisterSdk(){
	RegisterSdk(new GRSdkFactory());
}

//----------------------------------------------------------------------------
//	GRSdk
IF_OBJECT_IMP(GRSdk, NameManager);
GRSdk::GRSdk(const GRSdkDesc& desc):GRSdkDesc(desc){
	GRRegisterTypeDescs();
	GRRegisterFactories();
	SetNameManager(NameManager::GetRoot());
}
GRSdk::~GRSdk(){
	for(GRSdks::Cont::iterator it = sdks.cont->begin(); it != sdks.cont->end(); ++it){
		if (*it == this){
			sdks.cont->erase(it);
			DelRef();
			break;
		}
	}
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
	return (GRSceneIf*)CreateObject(GRSceneIf::GetIfInfoStatic(), &GRSdkDesc());
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
