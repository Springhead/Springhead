/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "FIFile.h"
#include "FILoadContext.h"

namespace Spr{;

IF_OBJECT_IMP_ABST(FIFile, Object);

//#define PDEBUG_EVAL(x)	x
#define PDEBUG_EVAL(x)

bool FIFile::Load(ObjectIfs& objs, const char* fn){
	printf("Loading %s ....", fn);
	FILoadContext fc;
	fc.objects.insert(fc.objects.end(), objs.begin(), objs.end());
	fc.fileInfo.Push();
	fc.fileInfo.back().Map(fn);
	Load(&fc);
	if (fc.rootObjects.size()){
		objs.insert(objs.end(), fc.rootObjects.begin(), fc.rootObjects.end());
		return true;
	}
	return false;
}
void FIFile::Load(FILoadContext* fc){
	if (fc->IsGood()){
		fc->typeDb = &typeDb;
		LoadImp(fc);
	}
	fc->Link();
	fc->PostTask();
}
void FIFile::LoadNode(FILoadContext* fc){
	if (fc->datas.Top()->type->GetIfInfo()){	
		//	インタフェースが登録されている場合，
		//	ロードしたデータからオブジェクトを作る．
		fc->PushCreateNode(fc->datas.Top()->type->GetIfInfo(), fc->datas.Top()->data);
	}
	//	ロード用のハンドラがあれば，呼び出す．
	//	ハンドラは，衝突判定の無効ペアの設定や重力の設定など，ノードを作る以外の仕事をする．
	static FINodeHandler key;
	key.AddRef();
	key.type = fc->datas.Top()->type->GetTypeName();
	FINodeHandlers::iterator it = handlers.lower_bound(&key);
	FINodeHandlers::iterator end = handlers.upper_bound(&key);
	for(; it != end; ++it){
		(*it)->Load(fc);
	}
	key.DelRef();
}
void FIFile::LoadEnterBlock(FILoadContext* fc){
	char* base = (char*)fc->datas.Top()->data;
	void* ptr = fc->fieldIts.back().field->GetAddressEx(base, fc->fieldIts.ArrayPos());
	fc->datas.Push(DBG_NEW FINodeData(NULL, ptr));
	fc->fieldIts.push_back(UTTypeDescFieldIt(fc->fieldIts.back().field->type));
}
void FIFile::LoadLeaveBlock(FILoadContext* fc){
	fc->fieldIts.Pop();
	fc->datas.Pop();
}
void FIFile::LoadEndNode(FILoadContext* fc){
	if (fc->datas.Top()->type){
		//	ハンドラがあれば，FINodeHandlerを呼び出す．
		static FINodeHandler key;
		key.AddRef();
		key.type = fc->datas.Top()->type->GetTypeName();
		FINodeHandlers::iterator lower = handlers.lower_bound(&key);
		FINodeHandlers::iterator upper = handlers.upper_bound(&key);
		while(upper != lower){
			--upper;
			(*upper)->Loaded(fc);
		}
		key.DelRef();
		if (fc->datas.Top()->type->GetIfInfo()){
			//	LoadNodeで作ったのノードをスタックから削除
			fc->objects.Pop();
		}
	}
}


bool FIFile::Save(const ObjectIfs& objs, const char* fn){
	FISaveContext sc;
	sc.Open(fn);
	Save(objs, &sc);
	return false;
}
void FIFile::Save(const ObjectIfs& objs, FISaveContext* sc){
	OnSaveFileStart(sc);
	for(ObjectIfs::const_iterator it = objs.begin(); it != objs.end(); ++it){
		SaveNode(sc, *it);
	}
	OnSaveFileEnd(sc);
}
void FIFile::SaveNode(FISaveContext* sc, ObjectIf* obj){
	if (!sc->savedObjects.insert(obj).second){
		sc->objects.Push(obj);
		OnSaveRef(sc);
		sc->objects.Pop();
		return;
	}
	
	//	セーブ中のノードを記録
	sc->objects.Push(obj);

	UTString tn = sc->GetNodeTypeName();
	tn.append("Desc");
	UTTypeDesc* type = typeDb.Find(tn);
	if(type){
		//	セーブ位置を設定
		sc->fieldIts.Push(UTTypeDescFieldIt(type));
		//	オブジェクトからデータを取り出す．
		void* data = (void*)obj->GetDescAddress();
		if (data){
			sc->datas.Push(DBG_NEW FINodeData(type, data));
		}else{
			sc->datas.Push(DBG_NEW FINodeData(type));
			data = sc->datas.back()->data;
			obj->GetDesc(data);
		}
		OnSaveNodeStart(sc);
		OnSaveDataStart(sc);
		//	データのセーブ
		SaveBlock(sc);
		sc->datas.Pop();
		sc->fieldIts.Pop();
		OnSaveDataEnd(sc);
		//	子ノードのセーブ
		size_t nChild = obj->NChildObject();
		if (nChild){
			OnSaveChildStart(sc);
			for(size_t i=0; i<nChild; ++i){
				ObjectIf* child = obj->GetChildObject(i);
				assert(child);
				SaveNode(sc, child);
			}
			OnSaveChildEnd(sc);
		}
		OnSaveNodeEnd(sc);
	}else{
		UTString err("Node '");
		err.append(tn);
		err.append("' not found. can not save data.");
		sc->ErrorMessage(err.c_str());
		//	子ノードのセーブ
		size_t nChild = obj->NChildObject();
		if (nChild){
			OnSaveChildStart(sc);
			for(size_t i=0; i<nChild; ++i){
				ObjectIf* child = obj->GetChildObject(i);
				assert(child);
				SaveNode(sc, child);
			}
			OnSaveChildEnd(sc);
		}
	}
	//	記録をPOP
	sc->objects.Pop();
}
void FIFile::SaveBlock(FISaveContext* sc){
	OnSaveBlockStart(sc);
	void* base = sc->datas.Top()->data;
	while(sc->fieldIts.back().NextField()){
		//UTTypeDesc::Composit::iterator field = sc->fieldIts.back().field;	//	現在のフィールド型
		UTTypeDesc::Field* field = sc->fieldIts.back().field;
		//	要素数の取得
		int nElements = 1;
		if (field->varType == UTTypeDesc::Field::VECTOR){
			nElements = field->VectorSize(base);
		}else if (field->varType == UTTypeDesc::Field::ARRAY){
			nElements = field->length;
		}
		OnSaveFieldStart(sc, nElements);
		PDEBUG_EVAL( DSTR << "Save field '" << field->name << "' : " << field->typeName << " = "; )
		for(int pos=0; pos<nElements; ++pos){
			OnSaveElementStart(sc, pos, (pos==nElements-1));
			switch(sc->fieldIts.back().fieldType){
				case UTTypeDescFieldIt::F_BLOCK:{
					PDEBUG_EVAL( DSTR << "=" << std::endl; )
					void* blockData = field->GetAddress(base, pos);
					sc->datas.Push(new FINodeData(field->type, blockData));
					sc->fieldIts.Push(UTTypeDescFieldIt(field->type));
					SaveBlock(sc);
					sc->fieldIts.Pop();
					sc->datas.Pop();
					}break;
				case UTTypeDescFieldIt::F_BOOL:{
					bool val = field->ReadBool(base, pos);
					PDEBUG_EVAL( DSTR << val ? "true" : "false"; )
					OnSaveBool(sc, val);
					}break;
				case UTTypeDescFieldIt::F_INT:{
					int val = (int)field->ReadNumber(base, pos);
					PDEBUG_EVAL( DSTR << val; )
					OnSaveInt(sc, val);
					}break;
				case UTTypeDescFieldIt::F_REAL:{
					double val = field->ReadNumber(base, pos);
					PDEBUG_EVAL( DSTR << val; )
					OnSaveReal(sc, val);
					}break;
				case UTTypeDescFieldIt::F_STR:{
					UTString val = field->ReadString(base, pos);
					PDEBUG_EVAL( DSTR << val; )
					OnSaveString(sc, val);
					}break;
			}
			PDEBUG_EVAL( if (pos<nElements-1) DSTR << ", "; )
			OnSaveElementEnd(sc, pos, (pos==nElements-1));
		}
		PDEBUG_EVAL( DSTR << ";" << std::endl; )
		OnSaveFieldEnd(sc, nElements);
	}
	OnSaveBlockEnd(sc);
}


}	//	namespace Spr
