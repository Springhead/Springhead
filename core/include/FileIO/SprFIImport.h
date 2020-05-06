/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FIImport_H
#define SPR_FIImport_H

/**	\defgroup gpFileIO */
//@{
namespace Spr{;

/// Importノード
//  UTImportHandlerが扱う
/*struct Import{
	std::string		path;	
};*/

struct ImportDesc{
	std::string		path;	
};

/**
	ファイル間のインポート関係を管理するクラス．
	ツリー構造を持つ．
 */
struct ImportIf : ObjectIf{
	SPR_IFDEF(Import);

	/**	@brief ロード専用モードをOn/Offする
		@param on
		ロード専用モードの場合，保存時に子オブジェクトを別ファイルへ保存せず，
		本体ファイルにImportノードのみ書き込む．
		Import対象が外部アプリで作成されたメッシュデータなどのときに使用する．
	 */
	void SetLoadOnly(bool on = true);
	/*
	/// インポート対象のオブジェクトを追加
	void AddObject(ObjectIf* obj);

	/// インポート対象のオブジェクトを取得
	//ObjectIf* GetTarget();

	/// インポート/エクスポートする相対パスを設定
	void SetPath(UTString path);
	
	/// インポート/エクスポートする相対パスを取得
	UTString GetPath();

	/// 子インポートエントリを作成
	FIImportIf* CreateImport();
	*/
};

}

#endif
