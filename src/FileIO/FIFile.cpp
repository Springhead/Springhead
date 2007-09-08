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

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

namespace Spr{;
//---------------------------------------------------------------------------
//	FIFileMap
//	ファイル マッピング
//  既存のファイルのアクセス速度向上を行うために、実際のファイルをメモリ上にマッピングする
class FIFileMap:public UTFileMap{
	FIFileMap::~FIFileMap(){
		if (start) Unmap();
	}
#ifdef _WIN32
		HANDLE hFile, hFileMap;		///<	ファイルハンドル、ファイルマッピングオブジェクト
#else 
		//FILE *hFile;
		//char *buffer;
		int fd;					///<	ファイルディスクリプタ
		struct stat filestat;	///<	ファイルサイズを得るのに使う
		void *sourceptr;
#endif
public:
	///	ファイルのマップ
	bool Map(const UTString fn){
		name = fn;
	#ifdef _WIN32
		// ファイルオープン
		hFile = CreateFile(fn.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);	
		if (!hFile){
			DSTR << "Cannot open input file: " << fn.c_str() << std::endl;
			return false;	
		}		
		// ファイルサイズの取得
		DWORD len = GetFileSize(hFile,NULL);	
		// ファイルマッピングオブジェクト作成
		hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		// ファイルfnを読み属性でマップし、その先頭アドレスを取得
		start = (const char*)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
		end = start + len;
		return true;	
	#else	
		/*
		hFile = fopen(fn.c_str(), "rb");
		if (!hFile) {
			DSTR << "Cannot open input file: " << fn.c_str() << std::endl;
			return false;	
		}		
		fseek(hFile, 0, SEEK_END);
		int const len = ftell(hFile);
		fseek(hFile, 0, SEEK_SET);
		buffer = DBG_NEW char[len];
		fread(buffer, 1, len, hFile);
		start = buffer;
		end = start + len;*/
		fd = open(fn.c_str(), O_RDONLY); 
		if( fd < 0 ) {
			DSTR << "Cannot open input file: " << fn.c_str() << std::endl;
			return false;	
		}		
		if( fstat( fd, &filestat ) == 0 ) {
			// 読み込み専用でファイルマッピング
			sourceptr = mmap( NULL, filestat.st_size, PROT_READ, MAP_SHARED, fd, 0 );
			if( sourceptr != MAP_FAILED ) {
				start = (char*)sourceptr;
				end = start + filestat.st_size;
				return true;
			} 
		}
		return false;
	#endif
	}
	/// ファイル アンマッピング
	void Unmap(){
	#ifdef _WIN32
		UnmapViewOfFile(start);		// マップしたファイルをアンマップ
		CloseHandle(hFileMap);		// ファイルマッピングオブジェクトをクローズ
		CloseHandle(hFile);			// ファイルのハンドルをクローズ
	#else
		//fclose(hFile);
		//delete[] buffer;
		munmap(sourceptr, filestat.st_size);
	#endif
		start = end = NULL;
	}
};
//---------------------------------------------------------------------------
//	FILoadContext
void FILoadContext::PushFileMap(const UTString fn){
	fileMaps.Push(DBG_NEW FIFileMap);
	fileMaps.Top()->Map(fn);
}


IF_OBJECT_IMP_ABST(FIFile, Object);

//#define PDEBUG_EVAL(x)	x
#define PDEBUG_EVAL(x)


//---------------------------------------------------------------------------
//	FIFile

bool FIFile::Load(ObjectIfs& objs, const char* fn){
	DSTR << "Loading " << fn << " ...." << std::endl;
	FILoadContext fc;
	fc.objects.insert(fc.objects.end(), objs.begin(), objs.end());
	fc.PushFileMap(fn);
	Load(&fc);
	if (fc.rootObjects.size()){
		objs.insert(objs.end(), fc.rootObjects.begin(), fc.rootObjects.end());
		return true;
	}
	if (objs.size()) return true;
	return false;
}
void FIFile::Load(FILoadContext* fc){
	if (fc->IsGood()){
		//	ファイルからデータをロード
		LoadImp(fc);
	}
	fc->LinkData();
	fc->loadedDatas.Print(DSTR);

	//	データからノードを作成
	fc->CreateScene();
	//	ノードをリンク
	fc->LinkNode();
	//	リンク後の処理
	fc->PostTask();
}

void FIFile::LBlockStart(FILoadContext* fc){
	char* base = (char*)fc->datas.Top()->data;
	void* ptr = fc->fieldIts.back().field->GetAddressEx(base, fc->fieldIts.ArrayPos());
	fc->datas.Push(DBG_NEW UTLoadedData(fc, NULL, ptr));
	fc->fieldIts.Push(UTTypeDescFieldIt(fc->fieldIts.back().field->type));
}
void FIFile::LBlockEnd(FILoadContext* fc){
	fc->fieldIts.Pop();
	fc->datas.Pop();
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
	UTTypeDesc* type = sc->typeDbs.Top()->Find(tn);
	if(type){
		//	セーブ位置を設定
		sc->fieldIts.Push(UTTypeDescFieldIt(type));
		//	オブジェクトからデータを取り出す．
		void* data = (void*)obj->GetDescAddress();
		if (data){
			sc->datas.Push(DBG_NEW UTLoadedData(NULL, type, data));
		}else{
			sc->datas.Push(DBG_NEW UTLoadedData(NULL, type));
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
		UTTypeDesc::Field* field = &*(sc->fieldIts.back().field);
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
					sc->datas.Push(new UTLoadedData(NULL, field->type, blockData));
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
				default:;
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
