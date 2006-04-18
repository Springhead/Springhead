#ifndef FIFILE_H
#define FIFILE_H

#include <SprFileIO.h>
#include <FileIO/FITypeDesc.h>
#include "FINodeHandler.h"

namespace Spr{;
class FILoadContext;
class FISaveContext;
class FIFile:public InheritObject<FIFileIf, Object>{
protected:
	///	ファイルにセーブ・ファイルからロードする型の情報(FITypeDesc)
	FITypeDescDb typeDb;
	///	ロード時に自動ロードとは別の処理が必要なノードのハンドラ
	FINodeHandlers handlers;
public:
	OBJECTDEFABST(FIFile);
	virtual bool Load(ObjectIfs& objs, const char* fn);
	virtual void Load(FILoadContext* fc);
	virtual bool Save(const ObjectIfs& objs, const char* fn);
	virtual void Save(const ObjectIfs& objs, FISaveContext* sc);
protected:
	///	ロードの実装
	virtual void LoadImp(FILoadContext* fc)=0;
	///
	void SaveNode(FISaveContext* sc, ObjectIf* obj);
	///
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
};


}
#endif
