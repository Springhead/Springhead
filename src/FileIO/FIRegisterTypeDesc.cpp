/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/Foundation.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "TypeDescDump.h"
#include "TypeDescDumpForOldSpringheadHandler.h"


using namespace Spr;
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 

namespace SprOldSpringhead{
	void SPR_CDECL RegisterTypeDescs(){
		static bool bFirst=true;
		if (!bFirst) return;
		bFirst = false;

		UTRegisterTypeDescs();

		UTTypeDescDb* db = UTTypeDescDb::theTypeDescDb;
		#include "TypeDescDumpImpForOldSpringheadHandler.h"
		db->Link();
	}
}

namespace Spr{ 
void SPR_CDECL FIRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();

	UTTypeDescDb* db = UTTypeDescDb::theTypeDescDb;
	#include "TypeDescDumpImp.h"
	db->Link();
	SprOldSpringhead::RegisterTypeDescs();
}

}
