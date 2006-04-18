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
	void LoadImp(FILoadContext* fc);

	//	ï€ë∂èàóùÇÃÉnÉìÉhÉâÅ[
	virtual void OnSaveFileStart(FISaveContext* sc);
	virtual void OnSaveNodeStart(FISaveContext* sc);
	virtual void OnSaveNodeEnd(FISaveContext* sc);
	virtual void OnSaveDataEnd(FISaveContext* sc);
	virtual void OnSaveBool(FISaveContext* sc, bool val);
	virtual void OnSaveInt(FISaveContext* sc, int val);
	virtual void OnSaveReal(FISaveContext* sc, double val);
	virtual void OnSaveRef(FISaveContext* sc);
	virtual void OnSaveString(FISaveContext* sc, UTString val);
	virtual void OnSaveFieldStart(FISaveContext* sc, int nElements);
	virtual void OnSaveFieldEnd(FISaveContext* sc, int nElements);
	virtual void OnSaveElementEnd(FISaveContext* sc, int pos, bool last);
};


}
#endif
