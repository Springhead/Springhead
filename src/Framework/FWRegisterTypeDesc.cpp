/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/Foundation.h>
#include <Framework/Framework.h>
#include "TypeDescDumpForOldSpringheadHandler.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "TypeDescDumpFramework.h"


using namespace Spr;
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 
namespace SprOldSpringhead{
	void SPR_CDECL OldSprRegisterTypeDescs(){
		static bool bFirst=true;
		if (!bFirst) return;
		bFirst = false;

		UTRegisterTypeDescs();

		UTTypeDescDb* db = UTTypeDescDbPool::Get("OldSpringhead");
		#include "TypeDescDumpImpForOldSpringheadHandler.h"
		db->Link();
	}
}

namespace Spr{
    static UTTypeDesc* desc;
    static UTTypeDesc::Field* field;

	void SPR_CDECL FWRegisterTypeDescs(){
	    static bool bFirst = true;
		if(!bFirst) return;
		bFirst=false;
		//
		UTRegisterTypeDescs();
		UTTypeDescDb* db = UTTypeDescDbPool::Get("Framework");
		assert(db);
		#include "TypeDescDumpImpFramework.h"
		db->Link();
		SprOldSpringhead::OldSprRegisterTypeDescs();
    }
}
