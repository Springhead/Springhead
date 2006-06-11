#include <Foundation/Foundation.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "TypeDescDump.h"
#include "TypeDescDumpForHandler.h"

namespace Spr{ 
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 
using namespace SprOldSpringehead;

void FIRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTRegisterTypeDescs();

	UTTypeDescDb* db = UTTypeDescDb::theTypeDescDb;
	#include "TypeDescDumpImp.h"
	#include "TypeDescDumpImpForHandler.h"
	db->Link();
}
}
