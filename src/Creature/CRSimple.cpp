/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRSimple.h"
#include <Foundation/Scene.h>
#include <Foundation/Object.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	CRSimple
IF_IMP(CRSimple, PHEngine);
OBJECT_IMP(CRSimple, PHEngine);

void CRRegisterFactories(){
	bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;

	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(CRSimple));
	std::cout << "CRSimple Factory Registered." << std::endl;
}

void CRSimple::Step(){
	for (int i=0; i<desc.test; i++){
		std::cout << "Hoge!";
	}
	std::cout << std::endl;
}

}
