/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHSdk.h>
#include <Physics/PHScene.h>
#include <Physics/PHHingeJoint.h>
#include <Physics/PHSliderJoint.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PHPathJoint.h>
#include <Physics/PHSpring.h>
#include <Physics/PHJointLimit.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHGear.h>
#include <Physics/PHFemMesh.h>
#include <Physics/PHFemMeshThermo.h>
#include <Physics/PHFemMeshNew.h>
#include <Physics/PHFemVibration.h>
#include <Physics/PHFemThermo.h>
#include <Physics/PHFemPorousWOMove.h>
#include <Collision/CDBox.h>
#include <Collision/CDSphere.h>
#include <Collision/CDCapsule.h>
#include <Collision/CDRoundCone.h>
#include <Collision/CDConvexMesh.h>
#include <Collision/CDConvexMeshInterpolate.h>
#include <stdlib.h>

namespace Spr{;

PHSdkIf* SPR_CDECL PHSdkIf::CreateSdk(){
	return (DBG_NEW PHSdk)->Cast();
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

	Sdk::RegisterFactory(DBG_NEW PHSdkFactory());
	PHSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHScene));

	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHSolid));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHRootNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHGear));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHHingeJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHSliderJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHBallJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHPathJoint));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHPath));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHSpring));

	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PH1DJointLimit));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHBallJointConeLimit));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHBallJointSplineLimit));
	
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHHingeJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHSliderJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHBallJointNode));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHPathJointNode));

	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHIKBallActuator));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHIKHingeActuator));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHIKEndEffector));

	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFemMesh));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFemMeshThermo));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFemMeshNew));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFemVibration));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFemThermo));
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFemPorousWOMove));

	PHSolidIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHFrame));

	PHRegisterTypeDescs();
	PHRegisterLoadHandlers();
	CDRegisterTypeDescs();
}

//----------------------------------------------------------------------------
//	PHSdk
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
int PHSdk::NScene(){
	return (int)scenes.size();
}
PHSceneIf* PHSdk::GetScene(int i){
	if (i < (int)scenes.size()) return scenes[i];
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
	if (ii == CDConvexMeshInterpolateIf::GetIfInfoStatic()){
		s = DBG_NEW CDConvexMeshInterpolate((const CDConvexMeshInterpolateDesc&)desc);
	}else if (ii == CDConvexMeshIf::GetIfInfoStatic()){
		s = DBG_NEW CDConvexMesh((const CDConvexMeshDesc&)desc);
	}else if (ii == CDSphereIf::GetIfInfoStatic()){
		s = DBG_NEW CDSphere((const CDSphereDesc&)desc);
	}else if (ii == CDCapsuleIf::GetIfInfoStatic()){
		s = DBG_NEW CDCapsule((const CDCapsuleDesc&)desc);
	}else if (ii == CDRoundConeIf::GetIfInfoStatic()){
		s = DBG_NEW CDRoundCone((const CDRoundConeDesc&)desc);
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
	return GetScene((int)(i - shapes.size()));
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
