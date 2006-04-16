#ifndef FIFILEX_H
#define FIFILEX_H

#include <SprFileIO.h>
#include <FileIO/FITypeDesc.h>
#include "FISpirit.h"
#include "FIFileContext.h"
#include "FINodeHandler.h"

namespace Spr{;

class FIFile:public InheritObject<FIFileIf, Object>{
public:
	OBJECTDEFABST(FIFile);
	virtual bool Load(ObjectIfs& objs, const char* fn){ return false; }
	virtual bool Save(const ObjectIfs& objs, const char* fn){ return false; }
};

template <class intf, class base>
struct InheritFIFile:public InheritObject<intf, base>{
	virtual bool Load(ObjectIfs& objs, const char* fn){ return base::Load(objs, fn); }
	virtual bool Save(const ObjectIfs& objs, const char* fn){ return base::Save(objs, fn); }
};

class FINodeHandlers;
class FIFileX:public InheritFIFile<FIFileXIf, FIFile>{
public:
	OBJECTDEF(FIFileX);
protected:
	FIPhraseParser start, temp, uuid, define, defNormal, defArray, blockSkip, 
		defOpen, defRestrict, arraySuffix, data, block, exp, ref, id, 
		boolVal, iNum, rNum, str;
	FISkipParser cmt;
	FITypeDescDb typeDb;
	FINodeHandlers handlers;
public:
	FIFileX();
	FITypeDescDb* GetDb(){return &typeDb;}
	void Init(FITypeDescDb* db=NULL, FINodeHandlers* h=NULL);
	void Load(FIFileContext* ctx);
	bool Load(ObjectIfs& objs, const char* fn);
	bool Save(const ObjectIfs& objs, const char* fn);
};




}
#endif
