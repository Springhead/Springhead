/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <Foundation/Object.h>
#include <Foundation/Scene.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

void __cdecl main(){
	UTRef<NameManager> nameMan = DBG_NEW NameManager;
	UTRef<NamedObject> obj = DBG_NEW NamedObject;
	NamedObjectIf* objIf = obj->Cast();
	NameManagerIf* nameManIf = nameMan->Cast();
	obj->SetNameManager(nameManIf);
	objIf->SetName("firstObject");
	nameManIf->Print(std::cout);

	/*
	HIPose* poseObj = new HIPose;
	NamedObjectIf* nIf = poseObj->Cast();
	HIOrientationIf* oriIf = poseObj->Cast();
	HIPositionIf* posIf = NULL;
	posIf = oriIf->Cast();
	oriIf = posIf->Cast();
	HIPoseIf* poseIf = poseObj->Cast();
	DSTR << oriIf->GetOrientation() << std::endl;
	DSTR << posIf->GetPosition() << std::endl;
	DSTR << poseIf->GetOrientation() << std::endl;

	
	delete poseObj;
*/
}
