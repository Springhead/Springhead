/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/Foundation.h>
#include <Framework/Framework.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
extern void RegisterTypeDescFramework(UTTypeDescDb* db);
extern void RegisterTypeDescFWOldSpringhead(UTTypeDescDb* db);

static UTTypeDesc* desc;
static UTTypeDesc::Field* field;
void SPR_CDECL FWRegisterTypeDescs(){
    static bool bFirst = true;
	if(!bFirst) return;
	bFirst=false;
	//
	UTRegisterTypeDescs();
	UTTypeDescDb* db = UTTypeDescDbPool::Get("Framework");
	RegisterTypeDescFramework(db);
	db = UTTypeDescDbPool::Get("OldSpringhead");
	RegisterTypeDescFWOldSpringhead(db);
	db->Link();
}

}
