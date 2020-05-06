/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTLoadHandler.h>
#include <FileIO/FIFileBinary.h>
#include <FileIO/FILoadContext.h>
#include <FileIO/FISaveContext.h>
#include <fstream>
#include <sstream>

namespace Spr{;

//#define TRACE_PARSE
#ifdef TRACE_PARSE
# define PDEBUG(x)	x
#else 
# define PDEBUG(x)
#endif

/* データ構造
 
 *node

 node:
	indicator (char) = ID_NODE_BEGIN
	type (null-terminated string)
	name (null-terminated string)
	block_size (int)
	block[block_size] (as described in typedesc)
	*(node | ref)
	indicator (char) = ID_NODE_END

 ref:
	indicator (char) = ID_REF
	name (null-terminated string)

*/

enum{
	ID_NODE_BEGIN,
	ID_NODE_END,
	ID_REF
};

FIFileBinary::FIFileBinary(const FIFileBinaryDesc& desc){
	Init();
}
void FIFileBinary::Init(){
	using namespace std;
	using namespace Spr;

}

void FIFileBinary::LoadBlock(){
	/*size_t sz = *(size_t*)ptr;
	ptr += sizeof(size_t);
	if(!skip)
		std::copy(ptr, ptr + sz, (char*)fileContext->datas.Top()->data);
	ptr += sz;*/

	void* base = fileContext->datas.Top()->data;
	while(fileContext->fieldIts.NextField(base)){
		UTTypeDescFieldIt& fieldIt = fileContext->fieldIts.back();
		UTTypeDesc::Field* field = &*(fieldIt.field);

		// 可変長フィールドの場合，長さを読み込みセット
		if(field->varType == UTTypeDescIf::VECTOR){
			fieldIt.arrayLength = (int) *(size_t*)ptr;
			ptr += sizeof(size_t);
		}

		while(fileContext->fieldIts.IncArrayPos()){
			if(fileContext->fieldIts.IsBool()){
				fileContext->WriteBool(*(bool*)ptr);
				ptr += sizeof(bool);
			}
			else if(fileContext->fieldIts.IsInteger()){
				fileContext->WriteNumber(*(int*)ptr);
				ptr += sizeof(int);
			}
			else if(fileContext->fieldIts.IsReal()){
				fileContext->WriteNumber(*(double*)ptr);
				ptr += sizeof(double);
			}
			else if(fileContext->fieldIts.IsString()){
				fileContext->WriteString(UTString(ptr));
				ptr += strlen(ptr) + 1;
			}
			else if(fileContext->fieldIts.IsBlock()){
				fileContext->CompositStart();
				LoadBlock();
				fileContext->CompositEnd();
			}
		}
	}
}

/* 戻り値:
	 1 : ノードのロード完了
	 0 : 親ノードの閉じタグ検知
	-1 : エラー
 */
int FIFileBinary::LoadNode(){
	char id = *ptr++;
	if(id == ID_REF){
		const char* name = ptr;
		fileContext->AddDataLink(UTString(name), ptr);
		ptr += strlen(name) + 1;
		return 1;
	}
	if(id == ID_NODE_END){
		fileContext->NodeEnd();
		return 0;
	}
	if(id != ID_NODE_BEGIN)
		return -1;
	
	// data size in bytes
	//size_t sz = *(size_t*)ptr;
	//ptr += sizeof(size_t);

	// node type
	const char* type = ptr;
	ptr += strlen(type) + 1;
	
	// node name
	const char* name = ptr;
	ptr += strlen(name) + 1;

	fileContext->NodeStart(UTString(type));
	fileContext->datas.back()->SetName(UTString(name));

	// typedesc available?
	if(!(fileContext->fieldIts.size() && fileContext->fieldIts.back().type))
		skip = true;

	LoadBlock();
	
	int ret;
	while(true){
		ret = LoadNode();
		// このノードの閉じタグを検知
		if(ret == 0){
			ret = 1;
			break;
		}
		// エラー
		if(ret == -1){
			ret = -1;
			break;
		}
	}
	return ret;
}

void FIFileBinary::LoadImp(FILoadContext* fc){
	fc->RegisterGroupToDb("Foundation Physics Graphics FileIO Framework Creature OldSpringhead");
	ptr = fc->fileMaps.Top()->start;
	end = fc->fileMaps.Top()->end;
	fileContext = fc;
	while(ptr < end)
		LoadNode();
}

//----------------------------------------------------
//	セーブ時のハンドラ

void FIFileBinary::OnSaveFileStart(FISaveContext* sc){
	sc->RegisterGroupToDb("Foundation Physics Graphics FileIO Framework Creature OldSpringhead");
}
void FIFileBinary::OnSaveNodeStart(FISaveContext* sc){
	char id = ID_NODE_BEGIN;
	sc->Stream().write(&id, sizeof(char));
	sc->Stream() << sc->GetNodeTypeName() << '\0';
	UTString name = sc->GetNodeName();
	sc->Stream() << name << '\0';
}
void FIFileBinary::OnSaveNodeEnd(FISaveContext* sc){
	char id = ID_NODE_END;
	sc->Stream().write(&id, sizeof(char));
}
void FIFileBinary::OnSaveBlockStart(FISaveContext* sc){
/*	size_t sz = sc->fieldIts.back().type->size;
	sc->Stream().write((const char*)&sz, sizeof(size_t));
	sc->Stream().write((const char*)sc->datas.Top()->data, sz);*/
}
void FIFileBinary::OnSaveRef(FISaveContext* sc){
	char id = ID_REF;
	sc->Stream().write(&id, sizeof(char));
	NamedObjectIf* n = DCAST(NamedObjectIf, sc->objects.Top());
	sc->Stream() << n->GetName() << '\0';
}
void FIFileBinary::OnSaveDataEnd(FISaveContext* sc){

}
bool FIFileBinary::OnSaveFieldStart(FISaveContext* sc, int nElements){
	UTTypeDesc::Composit::iterator field = sc->fieldIts.back().field;
	if(field->varType == UTTypeDescIf::VECTOR){
		// 可変長フィールドの場合，その長さを出力
		size_t len = nElements;
		sc->Stream().write((const char*)&len, sizeof(size_t));
	}
	return true;
}
void FIFileBinary::OnSaveFieldEnd(FISaveContext* sc, int nElements){

}
void FIFileBinary::OnSaveElementEnd(FISaveContext* sc, int nElements, bool last){

}
// flushしないとゴミデータが出力されることがある（原因不明）
void FIFileBinary::OnSaveBool(FISaveContext* sc, bool val){
	sc->Stream().write((const char*)&val, sizeof(bool));
	sc->Stream().flush();
}
void FIFileBinary::OnSaveInt(FISaveContext* sc, int val){
	sc->Stream().write((const char*)&val, sizeof(int));
	sc->Stream().flush();
}
void FIFileBinary::OnSaveReal(FISaveContext* sc, double val){
	sc->Stream().write((const char*)&val, sizeof(double));
	sc->Stream().flush();
}
void FIFileBinary::OnSaveString(FISaveContext* sc, UTString val){
	sc->Stream() << val << '\0';
	sc->Stream().flush();
}


};
