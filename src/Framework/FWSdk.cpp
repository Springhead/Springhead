/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Framework.h"
#include "Framework/FWSdk.h"
#include "Framework/FWOldSpringheadNode.h"
#include "Physics/PHSdk.h"
#include "Physics/PHScene.h"
#include "Graphics/GRSdk.h"
#include "Graphics/GRScene.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <stdlib.h>

namespace Spr{;

void SPR_CDECL FWRegisterTypeDescs();
void SPR_CDECL FWSdkIf::Register(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	FWRegisterTypeDescs();
	FWRegisterOldSpringheadNode();

	FWSdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(FWScene));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(FWObject));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSdk));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHScene));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRSdk));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(GRScene));
}

FWSdkIf* SPR_CDECL FWSdkIf::CreateSdk(){
	FWSdkIf* rv = DBG_NEW FWSdk;
	return rv;
}

//----------------------------------------------------------------------------
//	FWSdk
IF_OBJECT_IMP(FWSdk, Object);

FWSdk::FWSdk(){
}
FWSdk::~FWSdk(){
}
FWSceneIf* FWSdk::CreateScene(const FWSceneDesc& desc){
	FWSceneIf* rv = DCAST(FWSceneIf, CreateObject(FWSceneIf::GetIfInfoStatic(), &desc));
	AddChildObject(rv); 
	return rv;
}
int FWSdk::NScene() const{
	return (int)scenes.size();
}
FWSceneIf* FWSdk::GetScene(size_t i){
	if (i < scenes.size()) return scenes[i];
	return NULL;
}
bool FWSdk::AddChildObject(ObjectIf* o){
	FWScene* s = DCAST(FWScene, o);
	if (s){
		if (std::find(scenes.begin(), scenes.end(), s) == scenes.end()){
			scenes.push_back(s);
			return true;
		}
	}
	return false;
}

}
