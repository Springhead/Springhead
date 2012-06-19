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
	/**	@brief シーンをファイルからロードする
		@param objs ロードするシーンの親となるオブジェクトのスタック
		@param fn	ファイル名
		@return		成功したらtrue，失敗したらfalse
	 */
	bool Load(ObjectIfs& objs, const char* fn);

	/** @brief シーンをファイルへセーブする
		@param objs	セーブするシーンの最上位オブジェクトを格納した配列
		@param fn	ファイル名
		@return		成功したらtrue, 失敗したらfalse
	 */
	bool Save(const ObjectIfs& objs, const char* fn);

	/** @brief	インポート情報の設定
		@param	インポート情報
		シーンをセーブする際に，一部を別ファイルへエクスポートしたい場合は
		Saveに先立ってSetImportを呼ぶ．
	 */
	void SetImport(ImportIf* import);

	/**	@brief  インポート情報の取得
		@return	インポート情報
		シーンのロード後にインポートを取得したい際に使用する．
	 */
	ImportIf* GetImport();

	///	情報のDSTRの出力の有無
	/// 汎用性が低いのでObjectIfにデバッグ出力レベルを設定するAPIをつけるべきか？tazz
	void SetDSTR(bool f);
};

///	Sprファイルのファイルローダとセーバ
struct FIFileSprIf: public FIFileIf{
	SPR_IFDEF(FIFileSpr);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};
struct FIFileSprDesc{
	SPR_DESCDEF(FIFileSpr);
};

///	DirectXファイルのファイルローダとセーバ
struct FIFileXIf: public FIFileIf{
	SPR_IFDEF(FIFileX);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};
struct FIFileXDesc{
	SPR_DESCDEF(FIFileX);
};

///	DirectXファイルのファイルローダとセーバ
struct FIFileVRMLIf: public FIFileIf{
	SPR_IFDEF(FIFileVRML);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};
struct FIFileVRMLDesc{
	SPR_DESCDEF(FIFileVRML);
};

///	COLLADAファイルのファイルローダとセーバ
struct FIFileCOLLADAIf: public FIFileIf{
	SPR_IFDEF(FIFileCOLLADA);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};
struct FIFileCOLLADADesc{
	SPR_DESCDEF(FIFileCOLLADA);
};

///	バイナリファイルのファイルローダとセーバ
struct FIFileBinaryIf: public FIFileIf{
	SPR_IFDEF(FIFileBinary);
	///	ロード
	bool Load(ObjectIfs& objs, const char* fn);
	///	セーブ
	bool Save(const ObjectIfs& objs, const char* fn);
};
struct FIFileBinaryDesc{
	SPR_DESCDEF(FIFileBinary);
};

}

#endif
