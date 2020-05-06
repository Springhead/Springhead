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

struct FIFileIf;
struct FIFileXIf;
struct FIFileSprIf;
struct FIFileVRMLIf;
struct FIFileCOLLADAIf;
struct FIFileBinaryIf;
struct ImportIf;

///	ファイル入出力SDK
struct FISdkIf : public SdkIf{
	SPR_IFDEF(FISdk);

	/// Xファイルオブジェクトの作成
	FIFileXIf*			CreateFileX();
	/// Sprファイルオブジェクトの作成
	FIFileSprIf*		CreateFileSpr();
	/// VRMLファイルオブジェクトの作成
	FIFileVRMLIf*		CreateFileVRML();
	/// COLLADAファイルオブジェクトの作成
	FIFileCOLLADAIf*	CreateFileCOLLADA();
	/// バイナリファイルオブジェクトの作成
	FIFileBinaryIf*		CreateFileBinary();

	/** @brief IfInfoに対応するファイルオブジェクトを作成
		@param ii IfInfo
		未知のIfInfoの場合はNULLを返す．
	 */
	FIFileIf*			CreateFile(const IfInfo* ii);

	/** @brief 拡張子からファイルオブジェクトを作成
		@param ext	拡張子を含むファイルパス
		拡張子とファイルタイプの対応は以下の通り:
		.x		<=> Direct X
		.wrl	<=> VRML
		.dae	<=> Collada
		.dat	<=> Binary
		
		未知の拡張子の場合はNULLを返す．
	 */
	FIFileIf*		CreateFileFromExt(UTString filename);

	/// ルートインポートの作成
	ImportIf*		CreateImport();
	/// インポートの作成
	ImportIf*		CreateImport(ImportIf* parent, UTString path, ObjectIf* owner, const ObjectIfs& children);

	/// ファイルオブジェクトの削除
	bool DelChildObject(ObjectIf* o);
	/// 全ファイルオブジェクトの削除
	void Clear();

	static  FISdkIf* SPR_CDECL CreateSdk();
	static void SPR_CDECL RegisterSdk();
};

//@}

}	//	namespace Spr
#endif
