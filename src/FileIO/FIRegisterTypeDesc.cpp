/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/Foundation.h>
#ifdef USE_HDRSTOP
#include <FileIO/FISdk.h>
#pragma hdrstop
#endif

using namespace Spr;
//static UTTypeDesc* desc; 
//static UTTypeDesc::Field* field; 


namespace Spr{ 
extern void RegisterTypeDescFileIO(UTTypeDescDb* db);

void SPR_CDECL FIRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();
	UTTypeDescDb* db = UTTypeDescDbPool::Get("FileIO");
	RegisterTypeDescFileIO(db);
	db->Link();
}

}
