#include <Foundation/Foundation.h>
#include <Framework/Framework.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "TypeDescDump.h"

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
    }

	void SPR_CDECL FWRegisterFactories(){
		static bool bFirst = true;
		if (!bFirst) return;
		bFirst=false;

		FWSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(FWObject));
		std::cout << "Framework Factory Registered." << std::endl;
	}

}
