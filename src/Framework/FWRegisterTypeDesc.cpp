#include <Foundation/Foundation.h>
#include <Framework/Framework.h>
#include "TypeDescDumpForOldSpringheadHandler.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "TypeDescDump.h"


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
    static UTTypeDesc* desc;
    static UTTypeDesc::Field* field;

	void SPR_CDECL FWRegisterTypeDescs(){
	    static bool bFirst = true;
		if(!bFirst) return;
		bFirst=false;
		//
		UTRegisterTypeDescs();
		UTTypeDescDb* db = UTTypeDescDb::theTypeDescDb;
		assert(db);
		#include "TypeDescDumpImp.h"
		db->Link();
		SprOldSpringhead::RegisterTypeDescs();
    }

	void SPR_CDECL FWRegisterFactories(){
		static bool bFirst = true;
		if (!bFirst) return;
		bFirst=false;

		FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(FWObject));
		std::cout << "Framework Factory Registered." << std::endl;
	}

}
