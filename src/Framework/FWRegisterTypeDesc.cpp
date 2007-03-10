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
