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
#include <stdlib.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	FISdkIf
FISdkIf* SPR_CDECL FISdkIf::CreateSdk(){
	FISdk* rv = DBG_NEW FISdk;
	return rv->Cast();
}

//----------------------------------------------------------------------------
//	FISdk
void SPR_CDECL FIRegisterTypeDescs();

FISdk::FISdk(){
	FIRegisterTypeDescs();
	UTLoadHandlerDbPool::Get("FileIO")->insert(DBG_NEW ImportHandler);
}

FISdk::~FISdk(){
}

void FISdk::Clear(){
	Sdk::Clear();
	files.clear();
}
ObjectIf* FISdk::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Object::CreateObject(info, desc);
	if(!rv){
		if(info->Inherit(FIFileXIf::GetIfInfoStatic())){
			rv = CreateFileX();
		}
		else if(info->Inherit(FIFileVRMLIf::GetIfInfoStatic())){
			rv = CreateFileVRML();
		}
		else if(info->Inherit(FIFileCOLLADAIf::GetIfInfoStatic())){
			rv = CreateFileCOLLADA();
		}
		else if(info->Inherit(FIFileBinaryIf::GetIfInfoStatic())){
			rv = CreateFileBinary();
		}
	}
	return rv;
}
bool FISdk::DelChildObject(ObjectIf* o){
	FIFileIf* file = DCAST(FIFileIf, o);
	if(file){
		Files::iterator it = find(files.begin(), files.end(), file->Cast());
		if(it != files.end()){
			files.erase(it);
			return true;
		}
	}
	return false;
}

FIFileXIf* FISdk::CreateFileX(){
	FIFileX* rv = DBG_NEW FIFileX;
	rv->sdk = this;
	files.push_back(rv);
	return rv->Cast();
}

FIFileVRMLIf* FISdk::CreateFileVRML(){
	FIFileVRML* rv = DBG_NEW FIFileVRML;
	rv->sdk = this;
	files.push_back(rv);
	return rv->Cast();
}

FIFileCOLLADAIf* FISdk::CreateFileCOLLADA(){
	FIFileCOLLADA* rv = DBG_NEW FIFileCOLLADA;
	rv->sdk = this;
	files.push_back(rv);
	return rv->Cast();
}

FIFileBinaryIf* FISdk::CreateFileBinary(){
	FIFileBinary* rv = DBG_NEW FIFileBinary;
	rv->sdk = this;
	files.push_back(rv);
	return rv->Cast();
}

FIFileIf* FISdk::CreateFile(const IfInfo* ii){
	if(ii == FIFileXIf::GetIfInfoStatic())
		return CreateFileX();
	if(ii == FIFileVRMLIf::GetIfInfoStatic())
		return CreateFileVRML();
	if(ii == FIFileCOLLADAIf::GetIfInfoStatic())
		return CreateFileCOLLADA();
	if(ii == FIFileBinaryIf::GetIfInfoStatic())
		return CreateFileBinary();
	return NULL;
}

FIFileIf* FISdk::CreateFileFromExt(UTString filename){
	UTPath name;
	name.Path(filename);
	UTString ext = name.Ext();

	if(!ext.compare(".x"))
		return CreateFileX();

	if(!ext.compare(".wrl"))
		return CreateFileVRML();
	
	if(!ext.compare(".dae"))
		return CreateFileCOLLADA();
	
	if(!ext.compare(".dat"))
		return CreateFileBinary();
	
	return NULL;
}

ImportIf* FISdk::CreateImport(){
	Import* import = DBG_NEW Import;
	imports.push_back(import);
	return import->Cast();
}

ImportIf*	FISdk::CreateImport(ImportIf* parent, UTString path, ObjectIf* owner, const ObjectIfs& children){
	Import* import = DBG_NEW Import;
	import->path = path;
	import->ownerObj = owner;
	import->childObjs = children;

	DCAST(Import, parent)->AddChild(import);

	return import->Cast();
}

}
