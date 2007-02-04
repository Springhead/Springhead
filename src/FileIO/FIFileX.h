/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIFILEX_H
#define FIFILEX_H

#include "FIFile.h"
#include "FISpirit.h"

namespace Spr{;

class FIFileX:public FIFile, public FIFileXIfInit{
public:
	OBJECTDEF(FIFileX, FIFile);
protected:
	FIPhraseParser start, temp, uuid, define, defNormal, defArray, blockSkip, 
		defOpen, defRestrict, arraySuffix, data, block, exp, ref, id, 
		boolVal, iNum, rNum, str;
	FISkipParser cmt;

public:
	FIFileX();
	UTTypeDescDb* GetTypeDb(){return &typeDb;}
	void LoadImp(FILoadContext* fc);
	virtual void PushLoaderContext(FILoadContext* fc);
	virtual void PopLoaderContext();

protected:
	void Init();
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
