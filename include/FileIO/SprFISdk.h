/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FISDKIF_H
#define SPR_FISDKIF_H
#include <Foundation/SprScene.h>

namespace Spr{;


/** \addtogroup gpFileIO	*/
//@{

struct FIFileXIf;
struct FIFileVRMLIf;
struct FIFileCOLLADAIf;
struct FIFileBinaryIf;

///	ファイル入出力SDK
struct FISdkIf : public SdkIf{
	SPR_IFDEF(FISdk);

	/// Xファイルオブジェクトの作成
	FIFileXIf*			CreateFileX();
	/// VRMLファイルオブジェクトの作成
	FIFileVRMLIf*		CreateFileVRML();
	/// COLLADAファイルオブジェクトの作成
	FIFileCOLLADAIf*	CreateFileCOLLADA();
	/// バイナリファイルオブジェクトの作成
	FIFileBinaryIf*		CreateFileBinary();

	/// ファイルオブジェクトの削除
	bool DelChildObject(ObjectIf* o);
	/// 全ファイルオブジェクトの削除
	void Clear();

	static  FISdkIf* SPR_CDECL CreateSdk();
};

//@}

}	//	namespace Spr
#endif
