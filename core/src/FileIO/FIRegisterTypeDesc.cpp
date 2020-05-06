/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTTypeDesc.h>
#include <FileIO/FISdk.h>

using namespace Spr;
//static UTTypeDesc* desc; 
//static UTTypeDesc::Field* field; 


namespace Spr{ 
extern void RegisterTypeDescFileIO(UTTypeDescDbIf* db);

void SPR_CDECL FIRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();
	UTTypeDescDbIf* db = UTTypeDescDbPool::Get("FileIO");
	RegisterTypeDescFileIO(db);
	db->Link();
	db->LinkCheck();
}

}
