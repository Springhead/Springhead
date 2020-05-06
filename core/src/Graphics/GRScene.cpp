/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRScene.h>
#include <Graphics/GRSdk.h>
#include <Graphics/GRMesh.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	GRScene
GRScene::GRScene(const GRSceneDesc& desc):GRSceneDesc(desc){
	Init();
}
void GRScene::Init(){
	world = DBG_NEW GRFrame;
	world->SetScene(Cast());
	world->SetName("world");
}

GRSdkIf* GRScene::GetSdk(){
	GRSdk* sdk = DCAST(GRSdk, GetNameManager());
	return sdk->Cast();
}

void GRScene::Clear(){
	world = NULL;
	Init();
}
GRVisualIf* GRScene::CreateVisual(const IfInfo* info, const GRVisualDesc& desc, GRFrameIf* parent){
	GRVisual* v = NULL;
	if (info==GRFrameIf::GetIfInfoStatic()) 
		v = DBG_NEW GRFrame((const GRFrameDesc&)desc);
	else if (info==GRMaterialIf::GetIfInfoStatic()) 
		v = DBG_NEW GRMaterial((const GRMaterialDesc&)desc);
	else if (info==GRLightIf::GetIfInfoStatic()) 
		v = DBG_NEW GRLight((const GRLightDesc&)desc);
	else if (info==GRMeshIf::GetIfInfoStatic()) 
		v = DBG_NEW GRMesh((const GRMeshDesc&)desc);
	if(v){
		v->SetScene(Cast());
		if(parent) parent->AddChildObject(v->Cast());
		else GetWorld()->AddChildObject(v->Cast());
		return v->Cast();
	}
	return NULL;
}
void GRScene::SetCamera(const GRCameraDesc& desc){
	if (camera) camera->SetDesc(&desc);
	else{
		camera = DBG_NEW GRCamera(desc);
		camera->SetNameManager(Cast());
	}
}
bool GRScene::AddChildObject(ObjectIf* o){
	bool ok = false;
	GRCamera* c = o->Cast();
	if(!ok && c){
		camera = c;
		ok = true;
	}
	GRAnimationController* ac = o->Cast();
	if(!ok && ac){
		animationController = ac;
	}
	GRAnimationSet* as = o->Cast();
	if(!ok && as){
		if (!animationController) 
			animationController = CreateObject(GRAnimationControllerIf::GetIfInfoStatic(), NULL)->Cast();
		animationController->AddChildObject(as->Cast());
		ok = true;
	}
	if(ok){
		SceneObject* so = DCAST(SceneObject, o);
		so->SetScene(Cast());
		if(strcmp(so->GetName(), "") == 0){
			char name[256];
			if(camera)
				sprintf(name, "camera");
			if(ac)
				sprintf(name, "animation_controller");
			if(as)
				sprintf(name, "animation_set%d", (int)animationController->NChildObject()-1);
			so->SetName(name);		
		}
	}
	else {
		ok = world->AddChildObject(o);
	}
	return ok;
}
size_t GRScene::NChildObject() const{
	return world->NChildObject() + (camera ? 1 : 0) + (animationController ? 1 : 0);
}
ObjectIf* GRScene::GetChildObject(size_t pos){
	if(pos < world->NChildObject())
		return world->GetChildObject(pos);
	if (camera && pos == world->NChildObject()) return camera->Cast();
	return animationController->Cast();
}
void GRScene::Render(GRRenderIf* r){
	if (camera) camera->Render(r);
	world->Render(r);
	world->Rendered(r);
	if (camera) camera->Rendered(r);
}
}
