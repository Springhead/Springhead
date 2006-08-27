/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <stdlib.h>

namespace Spr{;
void SPR_CDECL PHRegisterTypeDescs();
void SPR_CDECL CDRegisterTypeDescs();

struct Sdks{
	typedef std::vector< PHSdkIf* > Cont;
	Cont* cont;

	Sdks(){
#if defined _DEBUG && _MSC_VER			
		// メモリリークチェッカ
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif		
		cont = DBG_NEW Cont;
	}
	~Sdks(){
		while(cont->size()){
			PHSdkIf* sdk = cont->back();
			cont->erase(cont->end()-1);
			sdk->DelRef();
			if (sdk->RefCount()==0){
				delete sdk;
			}
		}
		delete cont;
	}
};
static Sdks sdks;
PHSdkIf* SPR_CDECL CreatePHSdk(){
	PHSdkIf* rv = DBG_NEW PHSdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}

//----------------------------------------------------------------------------
//	PHSdk
IF_OBJECT_IMP(PHSdk, NameManager);

void SPR_CDECL PHRegisterSdk(){
	RegisterSdk(new PHSdkFactory());
}

void SPR_CDECL PHRegisterFactories(){
	bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	PHSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHScene));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSolid));
}

UTRef<UTTypeDescDb> PHSdk::typeDb;
PHSdk::PHSdk(){
	PHRegisterTypeDescs();
	CDRegisterTypeDescs();
	PHRegisterFactories();
	SetNameManager(NameManager::GetRoot());
}
PHSdk::~PHSdk(){
	for(Sdks::Cont::iterator it = sdks.cont->begin(); it != sdks.cont->end(); ++it){
		if (*it == this){
			sdks.cont->erase(it);
			DelRef();
			break;
		}
	}
}
UTTypeDescDb* PHSdk::GetTypeDb(){
	if (!typeDb) typeDb = new UTTypeDescDb;
	return typeDb;
}
PHSceneIf* PHSdk::CreateScene(const PHSceneDesc& desc){
	PHSceneIf* rv = DCAST(PHSceneIf, CreateObject(PHSceneIf::GetIfInfoStatic(), &desc));
	AddChildObject(rv); 
	return rv;
}
PHSceneIf* PHSdk::CreateScene(){
	return CreateScene(PHSceneDesc());
}
int PHSdk::NScene(){
	return scenes.size();
}
PHSceneIf* PHSdk::GetScene(size_t i){
	if (i < scenes.size()) return scenes[i];
	return NULL;
}

CDShapeIf* PHSdk::CreateShape(const CDShapeDesc& desc){
	CDShape* s = NULL;
	if (desc.type == CDShapeDesc::CONVEXMESH){
		s = DBG_NEW CDConvexMesh((const CDConvexMeshDesc&)desc);
	}else if (desc.type == CDShapeDesc::SPHERE){
		s = DBG_NEW CDSphere((const CDSphereDesc&)desc);
	}else if (desc.type == CDShapeDesc::CAPSULE){
		s = DBG_NEW CDCapsule((const CDCapsuleDesc&)desc);
	}else if (desc.type == CDShapeDesc::BOX){
		s = DBG_NEW CDBox((const CDBoxDesc&)desc);
	}
	if (s){
		s->SetNameManager(this);
		shapes.push_back(s);
	}else{
		DSTR << "Error: Unknown shape type " << desc.type << std::endl;
	}
	return s;
}
int PHSdk::NShape(){
	return shapes.size();
}
CDShapeIf* PHSdk::GetShape(int i){
	return shapes[i];
}
ObjectIf* PHSdk::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Object::CreateObject(info, desc);
	if (!rv){
		if (info->Inherit(CDShapeIf::GetIfInfoStatic())){
			rv = CreateShape(*(CDShapeDesc*)desc);
		}
	}
	return rv;
}
ObjectIf* PHSdk::GetChildObject(size_t i){		
	if (i<shapes.size()) return shapes[i];
	return scenes[i-shapes.size()];
}

bool PHSdk::AddChildObject(ObjectIf* o){
	PHScene* s = DCAST(PHScene, o);
	if (s){
		if (std::find(scenes.begin(), scenes.end(), s) == scenes.end()){
			scenes.push_back(s);
			return true;
		}
		return false;
	}

	CDShape* h = DCAST(CDShape, o);
	if (h){
		if (std::find(shapes.begin(), shapes.end(), h) == shapes.end()){
			shapes.push_back(h);
			return true;
		}
		return false;
	}

	if (std::find(objects.begin(), objects.end(), DCAST(Object, o)) == objects.end()){
		objects.push_back(DCAST(Object, o));
		return true;
	}
	return false;
}


}
