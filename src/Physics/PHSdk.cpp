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
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSpring));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHHingeJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSliderJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHBallJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHPathJointNode));
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
	it0 = find(scenes.begin(), scenes.end(), XCAST(scene0));
	it1 = find(scenes.begin(), scenes.end(), XCAST(scene1));
	if(it0 == scenes.end() || it1 == scenes.end())
		return;
	for(int i = 0; i < scene1->NChildObject(); i++){
		scene0->AddChildObject(scene1->GetChildObject(i));
	}
	scenes.erase(it1);
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
		s->SetNameManager(Cast());
		shapes.push_back(s->Cast());
	}else{
		DSTR << "Error: Unknown shape type " << desc.type << std::endl;
	}
	return s->Cast();
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
	if(i < shapes.size()) return shapes[i]->Cast();
	return GetScene(i - shapes.size());
}

bool PHSdk::AddChildObject(ObjectIf* o){
	PHSceneIf* s = DCAST(PHSceneIf, o);
	if (s){
		Scenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if (it == scenes.end()){
			scenes.push_back(s);
			return true;
		}
		return false;
	}
	CDShape* h = DCAST(CDShape, o);
	if (h){
		UTRef<CDShapeIf> shapeif = h->Cast();
		Shapes::iterator it = std::find(shapes.begin(), shapes.end(), shapeif);
		if (it == shapes.end()) {
			shapes.push_back(h->Cast());
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
	Objects::iterator it = std::find(objects.begin(), objects.end(), DCAST(Object, o));
	if (it != objects.end()){
		objects.erase(it);
		return true;
	}
	return false;
}


}
