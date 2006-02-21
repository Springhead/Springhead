#include <FileIO/FITypeDesc.h>
#include "FISpirit.h"
#include "FIFileContext.h"

namespace Spr{;

class FIFileX{
protected:
	FIPhraseParser start, temp, uuid, define, defNormal, defArray, blockSkip, 
		defOpen, defRestrict, arraySuffix, data, block, exp, ref, id, iNum, rNum, str;
	FISkipParser cmt;
	FITypeDescDb typeDb;
public:
	FITypeDescDb* GetDb(){return &typeDb;}
	void Init(FITypeDescDb* db);
	void Load(FIFileContext* ctx);
};




}
