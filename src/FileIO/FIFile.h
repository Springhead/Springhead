#ifndef FIFILE_H
#define FIFILE_H

#include <SprFileIO.h>
#include <FileIO/FITypeDesc.h>
#include "FINodeHandler.h"

namespace Spr{;
class FIFileContext;
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
	virtual void Load(FIFileContext* fc);
	virtual bool Save(const ObjectIfs& objs, const char* fn);
	virtual void Save(const ObjectIfs& objs, FISaveContext* sc);
protected:
	///	ロードの実装
	virtual void LoadImp(FIFileContext* fc)=0;
	///
	void SaveNode(FISaveContext* sc, ObjectIf* obj);
	///
	void SaveBlock(FISaveContext* sc);

	///	保存処理のハンドラー
	//@{
	///	ノード開始時の処理
	virtual void OnSaveNodeStart(FISaveContext* sc){}
	///	ノード終了時の処理
	virtual void OnSaveNodeEnd(FISaveContext* sc){}
	///	ブロック開始時の処理
	virtual void OnSaveBlockStart(FISaveContext* sc){}
	///	ブロック終了時の処理
	virtual void OnSaveBlockEnd(FISaveContext* sc){}
	///	子オブジェクトの保存開始時の処理
	virtual void OnSaveChildStart(FISaveContext* sc){}
	///	子オブジェクトの保存終了時の処理
	virtual void OnSaveChildEnd(FISaveContext* sc){}
	///	bool値の保存
	virtual void OnSaveBool(FISaveContext* sc, bool val){}
	///	int値の保存
	virtual void OnSaveInt(FISaveContext* sc, int val){}
	///	real値の保存
	virtual void OnSaveReal(FISaveContext* sc, double val){}
	///	string値の保存
	virtual void OnSaveString(FISaveContext* sc, UTString val){}
	//@}

};

template <class intf, class base>
struct InheritFIFile:public InheritObject<intf, base>{
	virtual bool Load(ObjectIfs& objs, const char* fn){ return base::Load(objs, fn); }
	virtual bool Save(const ObjectIfs& objs, const char* fn){ return base::Save(objs, fn); }
};


}
#endif
