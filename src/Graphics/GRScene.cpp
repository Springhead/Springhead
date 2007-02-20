/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "GRScene.h"
#include "GRMesh.h"
#include <sstream>

namespace Spr{;

//----------------------------------------------------------------------------
//	GRScene
IF_OBJECT_IMP(GRScene, Scene);

GRScene::GRScene(const GRSceneDesc& desc):GRSceneDesc(desc){
	Init();
}
void GRScene::Init(){
	world = DBG_NEW GRFrame;
	world->SetNameManager(this);
}

GRSdkIf* GRScene::GetSdk(){
	GRSdk* sdk = DCAST(GRSdk, GetNameManager());
	return sdk->Cast();
}

void GRScene::Clear(){
	world = NULL;
	Init();
}
GRVisualIf* GRScene::CreateVisual(const GRVisualDesc& desc, GRFrameIf* parent){
	GRVisual* v = NULL;
	switch(desc.type){
	case GRVisualDesc::FRAME:	 v = DBG_NEW GRFrame((const GRFrameDesc&)desc); break;
	case GRVisualDesc::MATERIAL: v = DBG_NEW GRMaterial((const GRMaterialDesc&)desc); break;
	case GRVisualDesc::LIGHT:	 v = DBG_NEW GRLight((const GRLightDesc&)desc); break;
	case GRVisualDesc::MESH:	 v = DBG_NEW GRMesh((const GRMeshDesc&)desc); break;
	}
	if(v){
		if(parent) parent->AddChildObject(v->Cast());
		else GetWorld()->AddChildObject(v->Cast());
	}
	return v->Cast();
}
bool GRScene::AddChildObject(ObjectIf* o){
	bool ok = false;
	GRCamera* c = DCAST(GRCamera, o);
	if(c){
		camera = c;
		ok = true;
	}else{
        ok = world->AddChildObject(o);
	}
	if(ok)
		DCAST(NamedObject, o)->SetNameManager(Cast());
	return ok;
}
size_t GRScene::NChildObject() const{
	//return (camera ? 1:0) + (world ? 1:0);
	return world->NChildObject() + (camera ? 1 : 0);
}
ObjectIf* GRScene::GetChildObject(size_t pos){
	//return camera ? (pos == 0 ? (ObjectIf*)camera->Cast() : (ObjectIf*)world->Cast()) : (ObjectIf*)world->Cast();
	if(pos < world->NChildObject())
		return world->GetChildObject(pos);
	return camera->Cast();
}
void GRScene::Render(GRRenderIf* r){
	if (camera) camera->Render(r);
	world->Render(r);
	world->Rendered(r);
	if (camera) camera->Rendered(r);
}


}
