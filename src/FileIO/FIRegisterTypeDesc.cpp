#include <Foundation/Foundation.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "TypeDescDump.h"


namespace Spr{ 
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 

void FIRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();

	UTTypeDescDb* db = UTTypeDescDb::theTypeDescDb;
	#include "TypeDescDumpImp.h"
	db->Link();
}
}
