/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FISAVECONTEXT_H
#define FISAVECONTEXT_H

#include <Foundation/UTLoadContext.h>
#include <fstream>

namespace Spr{;

class Import;
class FISdk;

class FISaveContext: public UTFileContext{
public:
	///	セーブするファイルの名前
	UTString fileName;
	///	UTTypeDescのフィールドへのイタレータのスタック
	UTTypeDescFieldIts fieldIts;
	///	セーブするディスクリプタのスタック．ネストした組み立て型に備えてスタックになっている．
	UTStack< UTRef<UTLoadedData> > datas;
	///
	std::set<ObjectIf*> savedObjects;
	/// インポートスタック
	UTStack<Import*>	importStack;
	/// SDKへの参照
	FISdk*				sdk;
	
	FISaveContext();
	///	セーブ用にファイルを開く
	//bool Open(const char* fn, bool binary);
	/// ファイルを閉じる
	//void Close();

	virtual void PushFileMap(const UTString fn, bool binary);
	virtual void PopFileMap();
	std::ostream&	Stream();

	///	メッセージの出力
	void Message(const char* msg);
	///	エラーメッセージの出力
	void ErrorMessage(const char* msg);

	/**	セーブするノードのグループを登録。グループ名をスペースで区切って指定。
		例：RegisterGroupToDb("Foundation Physics Graphics Framework OldSpringhead");
	*/
	void RegisterGroupToDb(const char* gp);

	UTString GetNodeTypeName();
	UTString GetNodeName();
};

}

#endif
