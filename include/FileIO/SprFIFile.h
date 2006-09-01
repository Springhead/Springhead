/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FIFile_H
#define SPR_FIFile_H
#include <Foundation/SprObject.h>

/**	\defgroup gpFileIO	ファイル入出力SDK	*/
//@{
namespace Spr{;

class UTTypeDescDb;
///	ファイルローダ・セーバ
struct FIFileIf: public ObjectIf{
	IF_DEF(FIFile);
	///	ロード
	virtual bool Load(ObjectIfs& objs, const char* fn)=0;
	///	セーブ
	virtual bool Save(const ObjectIfs& objs, const char* fn)=0;
	///	ノード型の登録
	virtual void RegisterGroup(const char* gp)=0;
};

///	DirectXファイルのファイルローダとセーバ
struct FIFileXIf: public FIFileIf{
	IF_DEF(FIFileX);
	///	ロード
	virtual bool Load(ObjectIfs& objs, const char* fn)=0;
	///	セーブ
	virtual bool Save(const ObjectIfs& objs, const char* fn)=0;
};


}

#endif
