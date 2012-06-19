/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Foundation/UTTypeDesc.h>

namespace Spr{ 
extern void RegisterTypeDescCreature(UTTypeDescDb* db);
void SPR_CDECL CRRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();

	UTTypeDescDb* db = UTTypeDescDbPool::Get("Creature");
	assert(db);
	RegisterTypeDescCreature(db);
	db->Link();
}
}

