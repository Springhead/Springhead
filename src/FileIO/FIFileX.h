#ifndef FIFILEX_H
#define FIFILEX_H

#include "FIFile.h"
#include "FISpirit.h"

namespace Spr{;

class FIFileX:public InheritFIFile<FIFileXIf, FIFile>{
public:
	OBJECTDEF(FIFileX);
protected:
	FIPhraseParser start, temp, uuid, define, defNormal, defArray, blockSkip, 
		defOpen, defRestrict, arraySuffix, data, block, exp, ref, id, 
		boolVal, iNum, rNum, str;
	FISkipParser cmt;

public:
	FIFileX();
	FITypeDescDb* GetDb(){return &typeDb;}
	void Init(FITypeDescDb* db=NULL, FINodeHandlers* h=NULL);
protected:
	void LoadImp(FIFileContext* fc);
	bool Save(const ObjectIfs& objs, const char* fn);
};


}
#endif
