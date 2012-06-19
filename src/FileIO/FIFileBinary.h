/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIFILEBINARY_H
#define FIFILEBINARY_H

#include <FileIO/FIFile.h>

namespace Spr{;

class FIFileBinary : public FIFile{
public:
	SPR_OBJECTDEF(FIFileBinary);
protected:
	FILoadContext* fileContext;
	bool skip;
	const char *ptr, *end;

	void LoadBlock();
	int LoadNode();
public:
	FIFileBinary(const FIFileBinaryDesc& desc = FIFileBinaryDesc());
	void LoadImp(FILoadContext* fc);
	//virtual void PushLoaderContext(FILoadContext* fc);
	//virtual void PopLoaderContext();

protected:
	void Init();

	virtual bool IsBinary(){ return true; }

	//	保存処理のハンドラー
	virtual void OnSaveFileStart(FISaveContext* sc);
	virtual void OnSaveNodeStart(FISaveContext* sc);
	virtual void OnSaveNodeEnd(FISaveContext* sc);
	virtual void OnSaveBlockStart(FISaveContext* sc);
	virtual void OnSaveRef(FISaveContext* sc);
	virtual void OnSaveDataEnd(FISaveContext* sc);
	virtual void OnSaveBool(FISaveContext* sc, bool val);
	virtual void OnSaveInt(FISaveContext* sc, int val);
	virtual void OnSaveReal(FISaveContext* sc, double val);
	virtual void OnSaveString(FISaveContext* sc, UTString val);
	virtual bool OnSaveFieldStart(FISaveContext* sc, int nElements);
	virtual void OnSaveFieldEnd(FISaveContext* sc, int nElements);
	virtual void OnSaveElementEnd(FISaveContext* sc, int pos, bool last);
};


}
#endif
