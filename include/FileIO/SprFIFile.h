/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_IFDEF(FIFile);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};

///	DirectXファイルのファイルローダとセーバ
struct FIFileXIf: public FIFileIf{
	SPR_IFDEF(FIFileX);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};

///	COLLADAファイルのファイルローダとセーバ
struct FIFileCOLLADAIf: public FIFileIf{
	SPR_IFDEF(FIFileCOLLADA);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};

}

#endif
