/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTTypeDesc.h>
#include <Graphics/GRSdk.h>

namespace Spr{ 
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 

extern void RegisterTypeDescGraphics(UTTypeDescDb* db);
void SPR_CDECL GRRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();

	UTTypeDescDb* fdb = UTTypeDescDbPool::Get("Foundation");
	UTTypeDescDb* db = UTTypeDescDbPool::Get("Graphics");
	RegisterTypeDescGraphics(db);
	db->Link();
	db->Link(fdb);
	db->LinkCheck();
}
}
