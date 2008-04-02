/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIFILE_H
#define FIFILE_H

#include <SprFileIO.h>
#include <Foundation/UTTypeDesc.h>
#include <Foundation/UTLoadContext.h>
#include <Foundation/UTLoadHandler.h>

namespace Spr{;
///	FILoadContext
class FILoadContext: public UTLoadContext{
public:
	void PushFileMap(const UTString fn);
	virtual ~FILoadContext(){}
};

class FISaveContext;
class FISdk;

/**	ファイルローダー・セーバー
*/
class FIFile:public Object{
public:
	FISdk* sdk;
	SPR_OBJECTDEF_ABST(FIFile);
	FISdk* GetSdk(){ return sdk; }
	virtual bool Load(ObjectIfs& objs, const char* fn);
	virtual void Load(FILoadContext* fc);
	virtual bool Save(const ObjectIfs& objs, const char* fn);
	virtual void Save(const ObjectIfs& objs, FISaveContext* sc);
	/**	ノードのロード．
		TODO: 新仕様にあわせる

		ロードしたDescからオブジェクトを作成する．
		オブジェクトの作成は，親オブジェクトのCreateObject()，親の親のCreateObject()と
		先祖のCreateObject()を順に呼んで，作成できたところで止める．
		どのオブジェクトも作成できない場合は，SdkIf::CreateSdk()を呼び出す．
	void LoadNode();

	///	ノードのロードの終了
	void LoadEndNode(FILoadContext* fc);
	*/
	///	ロードの実装
	virtual void LoadImp(FILoadContext* fc)=0;
	
	//@name ローダーを書くためのユーティリティ関数
	//@{	
	///	ブロック(組み立て型)に入る
	virtual void LBlockStart(FILoadContext* fc);
	///	ブロック(組み立て型)から出る
	virtual void LBlockEnd(FILoadContext* fc);
	//@}

protected:	
	///	ノードのセーブ
	void SaveNode(FISaveContext* sc, ObjectIf* obj);
	///	ブロックのセーブ
	void SaveBlock(FISaveContext* sc);

	//@name 保存処理のハンドラー
	//@{
	///	ファイル開始時の処理
	virtual void OnSaveFileStart(FISaveContext* sc){}
	///	ファイル終了時の処理
	virtual void OnSaveFileEnd(FISaveContext* sc){}
	///	ノード開始時の処理
	virtual void OnSaveNodeStart(FISaveContext* sc){}
	///	ノード終了時の処理
	virtual void OnSaveNodeEnd(FISaveContext* sc){}
	///	データ開始時の処理
	virtual void OnSaveDataStart(FISaveContext* sc){}
	///	データ終了時の処理
	virtual void OnSaveDataEnd(FISaveContext* sc){}
	///	子オブジェクトの保存開始時の処理
	virtual void OnSaveChildStart(FISaveContext* sc){}
	///	子オブジェクトの保存終了時の処理
	virtual void OnSaveChildEnd(FISaveContext* sc){}

	///	ブロック開始時の処理
	virtual void OnSaveBlockStart(FISaveContext* sc){}
	///	ブロック終了時の処理
	virtual void OnSaveBlockEnd(FISaveContext* sc){}
	///	フィールド開始時の処理
	virtual void OnSaveFieldStart(FISaveContext* sc, int nElements){}
	///	フィールド終了時の処理
	virtual void OnSaveFieldEnd(FISaveContext* sc, int nElements){}
	///	要素開始時の処理
	virtual void OnSaveElementStart(FISaveContext* sc, int pos, bool last){}
	///	要素終了時の処理
	virtual void OnSaveElementEnd(FISaveContext* sc, int pos, bool last){}
	///	bool値の保存
	virtual void OnSaveBool(FISaveContext* sc, bool val){}
	///	int値の保存
	virtual void OnSaveInt(FISaveContext* sc, int val){}
	///	real値の保存
	virtual void OnSaveReal(FISaveContext* sc, double val){}
	///	string値の保存
	virtual void OnSaveString(FISaveContext* sc, UTString val){}
	virtual void OnSaveRef(FISaveContext* sc){}
	//@}

};


}
#endif
