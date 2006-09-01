/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
};

class FISaveContext;
class FISdk;

/**	ファイルローダー・セーバー
*/
class FIFile:public InheritObject<FIFileIf, Object>{
public:
	///	ファイルにセーブ・ファイルからロードする型の情報(UTTypeDesc)
	UTTypeDescDb typeDb;
	///	ロード時に自動ロードとは別の処理が必要なノードのハンドラ
	UTLoadHandlers handlers;
public:
	FISdk* sdk;
	OBJECT_DEF_ABST(FIFile);
	virtual bool Load(ObjectIfs& objs, const char* fn);
	virtual void Load(FILoadContext* fc);
	virtual bool Save(const ObjectIfs& objs, const char* fn);
	virtual void Save(const ObjectIfs& objs, FISaveContext* sc);
	FISdk* GetSdk(){ return sdk; }
	/**	ノードのロード．
		ロードしたDescからオブジェクトを作成する．
		オブジェクトの作成は，親オブジェクトのCreateObject()，親の親のCreateObject()と
		先祖のCreateObject()を順に呼んで，作成できたところで止める．
		どのオブジェクトも作成できない場合は，SdkIf::CreateSdk()を呼び出す．	*/
	void LoadNode(FILoadContext* fc);
	///	ノードのロードの終了
	void LoadEndNode(FILoadContext* fc);
	///	ブロック(組み立て型)に入る
	void LoadEnterBlock(FILoadContext* fc);
	///	ブロック(組み立て型)から出る
	void LoadLeaveBlock(FILoadContext* fc);
	///	ロードの実装
	virtual void LoadImp(FILoadContext* fc)=0;
	///	LoadContextの設定
	virtual void SetLoaderContext(FILoadContext* fc){}
	/**	ロードするノードのグループを登録。グループ名をスペースで区切って指定。
		例：ResisterGroup("Foundation Physics Graphics Framework OldSpringhead");
	*/
	void RegisterGroup(const char* gp);

protected:
	
	///	ノードのセーブ
	void SaveNode(FISaveContext* sc, ObjectIf* obj);
	///	ブロックのセーブ
	void SaveBlock(FISaveContext* sc);

	///	保存処理のハンドラー
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

template <class intf, class base>
struct InheritFIFile:public InheritObject<intf, base>{
	virtual bool Load(ObjectIfs& objs, const char* fn){ return base::Load(objs, fn); }
	virtual bool Save(const ObjectIfs& objs, const char* fn){ return base::Save(objs, fn); }
	virtual void RegisterGroup(const char* gp){ return base::RegisterGroup(gp); }
};


}
#endif
