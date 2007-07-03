/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */


// ïKóvÇ…Ç»Ç¡ÇΩÇÁÇøÇ·ÇÒÇ∆èëÇ≠ 07/07/03 mitake
/*
#include <Foundation/Foundation.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "TypeDescDump.h"

namespace Spr{ 
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 

void CRRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();

	UTTypeDescDb* db = UTTypeDescDb::GetDb("Physics");
	assert(db);
	#include "TypeDescDumpImp.h"
	db->Link();
}
}
*/

