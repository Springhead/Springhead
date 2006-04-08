#include <FileIO/FITypeDesc.h>
#include "FISpirit.h"
#include "FIFileContext.h"
#include "FINodeHandler.h"

namespace Spr{;

class FINodeHandlers;
class FIFileX{
protected:
	FIPhraseParser start, temp, uuid, define, defNormal, defArray, blockSkip, 
		defOpen, defRestrict, arraySuffix, data, block, exp, ref, id, 
		boolVal, iNum, rNum, str;
	FISkipParser cmt;
	FITypeDescDb typeDb;
	FINodeHandlers handlers;
public:
	FITypeDescDb* GetDb(){return &typeDb;}
	void Init(FITypeDescDb* db=NULL, FINodeHandlers* h=NULL);
	void Load(FIFileContext* ctx);
};




}
