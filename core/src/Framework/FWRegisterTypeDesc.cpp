/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTTypeDesc.h>
#include <Framework/FWSdk.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
extern void RegisterTypeDescFramework(UTTypeDescDbIf* db);
extern void RegisterTypeDescFWOldSpringhead(UTTypeDescDbIf* db);

static UTTypeDesc* desc;
static UTTypeDesc::Field* field;
void SPR_CDECL FWRegisterTypeDescs(){
    static bool bFirst = true;
	if(!bFirst) return;
	bFirst=false;
	//
	UTRegisterTypeDescs();
	UTTypeDescDbIf* fdb = UTTypeDescDbPool::Get("Foundation");
	UTTypeDescDbIf* pdb = UTTypeDescDbPool::Get("Physics");
	UTTypeDescDbIf* gdb = UTTypeDescDbPool::Get("Graphics");
	UTTypeDescDbIf* cdb = UTTypeDescDbPool::Get("Creature");
	UTTypeDescDbIf* db = UTTypeDescDbPool::Get("Framework");
	RegisterTypeDescFramework(db);
	db->Link();
	db->Link(fdb);
	db->Link(pdb);
	db->Link(gdb);
	db->Link(cdb);

	db = UTTypeDescDbPool::Get("OldSpringhead");
	RegisterTypeDescFWOldSpringhead(db);
	db->Link();
	db->Link(fdb);
	db->Link(pdb);
	db->Link(gdb);
	db->Link(cdb);
	db->LinkCheck();
}

}
