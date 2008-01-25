/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

PHSdkIf* SPR_CDECL PHSdkIf::CreateSdk(){
	PHSdk* rv = DBG_NEW PHSdk;
	return rv->Cast();
}


//----------------------------------------------------------------------------
//	PHSdkIf

void SPR_CDECL PHRegisterTypeDescs();
void SPR_CDECL PHRegisterLoadHandlers();
void SPR_CDECL CDRegisterTypeDescs();

///	PHSdkをファイルローダーなどに登録。一度だけ呼べばよい
void PHSdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;

	Sdk::RegisterFactory(new PHSdkFactory());
	PHSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHScene));

	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSolid));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHRootNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHGear));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHHingeJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSliderJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHBallJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHPathJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHPath));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSpring));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHHingeJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSliderJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHBallJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHPathJointNode));

	PHSolidIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHFrame));

	PHRegisterTypeDescs();
	PHRegisterLoadHandlers();
	CDRegisterTypeDescs();
}

//----------------------------------------------------------------------------
//	PHSdk
IF_OBJECT_IMP(PHSdk, Sdk);

PHSdk::PHSdk(const PHSdkDesc&){
	PHSdkIf::RegisterSdk();
}
PHSdk::~PHSdk(){
	Clear();
}
void PHSdk::Clear(){
	Sdk::Clear();
	shapes.clear();
	objects.clear();
	scenes.clear();
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
void PHSdk::MergeScene(PHSceneIf* scene0, PHSceneIf* scene1){
	Scenes::iterator it0, it1;
	it0 = find(scenes.begin(), scenes.end(), scene0);
	it1 = find(scenes.begin(), scenes.end(), scene1);
	if(it0 == scenes.end() || it1 == scenes.end())
		return;
	for(unsigned i = 0; i < scene1->NChildObject(); i++){
		scene0->AddChildObject(scene1->GetChildObject(i));
	}
	scenes.erase(it1);
}
	
CDShapeIf* PHSdk::CreateShape(const IfInfo* ii, const CDShapeDesc& desc){
	CDShape* s = NULL;
	if (ii == CDConvexMeshIf::GetIfInfoStatic()){
		s = DBG_NEW CDConvexMesh((const CDConvexMeshDesc&)desc);
	}else if (ii == CDSphereIf::GetIfInfoStatic()){
		s = DBG_NEW CDSphere((const CDSphereDesc&)desc);
	}else if (ii == CDCapsuleIf::GetIfInfoStatic()){
		s = DBG_NEW CDCapsule((const CDCapsuleDesc&)desc);
	}else if (ii == CDBoxIf::GetIfInfoStatic()){
		s = DBG_NEW CDBox((const CDBoxDesc&)desc);
	}
	if (s){
		AddChildObject(s->Cast());
	}else{
		DSTR << "Error: Unknown shape type " << ii->ClassName() << std::endl;
	}
	return s->Cast();
}
ObjectIf* PHSdk::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Object::CreateObject(info, desc);
	if (!rv){
		if (info->Inherit(CDShapeIf::GetIfInfoStatic())){
			rv = CreateShape(info, *(CDShapeDesc*)desc);
		}
	}
	return rv;
}
ObjectIf* PHSdk::GetChildObject(size_t i){		
	if(i < shapes.size()) return shapes[i]->Cast();
	return GetScene(i - shapes.size());
}

bool PHSdk::AddChildObject(ObjectIf* o){
	bool ok = false;
	PHSceneIf* scene = DCAST(PHSceneIf, o);
	if (scene){
		Scenes::iterator it = std::find(scenes.begin(), scenes.end(), scene);
		if (it == scenes.end()){
			scenes.push_back(scene);
			ok = true;
		}
	}
	CDShapeIf* shape = DCAST(CDShapeIf, o);
	if (shape){
		Shapes::iterator it = std::find(shapes.begin(), shapes.end(), shape);
		if (it == shapes.end()) {
			shapes.push_back(shape);
			ok = true;
		}
	}
	if(ok){
		NamedObject* no = DCAST(NamedObject, o);
		no->SetNameManager(Cast());
		if(strcmp(no->GetName(), "") == 0){
			char name[256];
			if(scene)
				sprintf(name, "scene%d", NScene()-1);
			if(shape)
				sprintf(name, "shape%d", NShape()-1);
			no->SetName(name);
		}
	}
	// その他のオブジェクト
	if (std::find(objects.begin(), objects.end(), (Object*)(o->Cast())) == objects.end()){
		objects.push_back(o->Cast());
		ok = true;
	}
	
	return false;
}
bool PHSdk::DelChildObject(ObjectIf* o){
	PHSceneIf* s = DCAST(PHSceneIf, o);
	if(s){
		Scenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if (it != scenes.end()){
			scenes.erase(it);
			return true;
		}
		return false;
	}
	CDShapeIf* shape = DCAST(CDShapeIf, o);
	if(shape){
		Shapes::iterator it = find(shapes.begin(), shapes.end(), shape);
		if(it != shapes.end()){
			shapes.erase(it);
			return true;
		}
		return false;
	}
	ObjectRefs::iterator it = std::find(objects.begin(), objects.end(), DCAST(Object, o));
	if (it != objects.end()){
		objects.erase(it);
		return true;
	}
	return false;
}


}
