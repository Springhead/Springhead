/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FIFileBinary.h"
#include <Foundation/UTLoadHandler.h>
#include <fstream>
#include <sstream>
#include <Springhead.h>

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
	block (as described in typedesc)
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

FIFileBinary::FIFileBinary(){
	Init();
}
void FIFileBinary::Init(){
	using namespace std;
	using namespace Spr;

}

void FIFileBinary::LoadBlock(){
	size_t sz = *(size_t*)ptr;
	if(!skip)
		std::copy(ptr, ptr + sz, (char*)fileContext->datas.Top()->data);
	ptr += sz;
	/*
	while(fileContext->fieldIts.NextField()){
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
				LoadBlock(ptr);
			}
		}
	}
	*/
}

bool FIFileBinary::LoadNode(){
	char id = *ptr++;
	if(id == ID_REF){
		const char* name = ptr;
		fileContext->AddDataLink(UTString(name), ptr);
		ptr += strlen(name) + 1;
		return true;
	}
	if(id == ID_NODE_END){
		fileContext->NodeEnd();
		return true;
	}
	if(id != ID_NODE_BEGIN)
		return false;
	
	// data size in bytes
	//size_t sz = *(size_t*)ptr;
	//ptr += sizeof(size_t);

	// node type
	const char* type = ptr;
	ptr += strlen(type) + 1;
	
	// node name
	const char* name = ptr;
	ptr += strlen(name) + 1;

	fileContext->NodeStart(UTString(name));
	fileContext->datas.back()->SetName(UTString(name));

	// typedesc available?
	if(!(fileContext->fieldIts.size() && fileContext->fieldIts.back().type))
		skip = true;

	LoadBlock();
	
	// child nodes or NODE_END tag
	while(LoadNode());

	//ptr = next;
	return true;
}

void FIFileBinary::LoadImp(FILoadContext* fc){
	ptr = fc->fileMaps.Top()->start;
	end = fc->fileMaps.Top()->end;
	while(ptr < end)
		LoadNode();
}

//----------------------------------------------------
//	セーブ時のハンドラ

void FIFileBinary::OnSaveFileStart(FISaveContext* sc){

}
void FIFileBinary::OnSaveNodeStart(FISaveContext* sc){
	char id = ID_NODE_BEGIN;
	sc->file.write(&id, sizeof(char));
	sc->file << sc->GetNodeTypeName() << '\0';
	UTString name = sc->GetNodeName();
	sc->file << name << '\0';
}
void FIFileBinary::OnSaveNodeEnd(FISaveContext* sc){
	char id = ID_NODE_END;
	sc->file.write(&id, sizeof(char));
}
void FIFileBinary::OnSaveBlockStart(FISaveContext* sc){
	size_t sz = sc->fieldIts.back().type->size;
	sc->file.write((const char*)&sz, sizeof(size_t));
	sc->file.write((const char*)sc->datas.Top()->data, sz);
}
void FIFileBinary::OnSaveRef(FISaveContext* sc){
	NamedObjectIf* n = DCAST(NamedObjectIf, sc->objects.Top());
	sc->file << n->GetName() << '\0';
}

/*
void FIFileBinary::OnSaveDataEnd(FISaveContext* sc){

}

void FIFileBinary::OnSaveFieldStart(FISaveContext* sc, int nElements){

}
void FIFileBinary::OnSaveFieldEnd(FISaveContext* sc, int nElements){

}
void FIFileBinary::OnSaveElementEnd(FISaveContext* sc, int nElements, bool last){

}
void FIFileBinary::OnSaveBool(FISaveContext* sc, bool val){
	sc->file.write((const char*)&val, sizeof(bool));
}
void FIFileBinary::OnSaveInt(FISaveContext* sc, int val){
	sc->file.write((const char*)&val, sizeof(int));
}
void FIFileBinary::OnSaveReal(FISaveContext* sc, double val){
	sc->file.write((const char*)&val, sizeof(double));
}
void FIFileBinary::OnSaveString(FISaveContext* sc, UTString val){
	sc->file << val << '\0';
}*/


};
