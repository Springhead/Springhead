/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <FileIO/FISdk.h>
#include <FileIO/FIFileX.h>
#include <FileIO/FIFileCOLLADA.h>
#include <FileIO/FIFileBinary.h>
#include <FileIO/FIFileSpr.h>
#include <FileIO/FIFileVRML.h>
#include <FileIO/FIImport.h>
#include <stdlib.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	FISdkIf
FISdkIf* SPR_CDECL FISdkIf::CreateSdk(){
	FISdk* rv = DBG_NEW FISdk;
	return rv->Cast();
}

void SPR_CDECL FIRegisterTypeDescs();
void SPR_CDECL FISdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;

	Sdk::RegisterFactory(DBG_NEW FISdkFactory());
	FIRegisterTypeDescs();

	FISdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(FIFileX));
	FISdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(FIFileVRML));
	FISdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(FIFileCOLLADA));
	FISdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(FIFileBinary));
	FISdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImpOwned(FIFileSpr));
	FISdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(Import));

	UTLoadHandlerDbPool::Get("FileIO")->insert(DBG_NEW ImportHandler);
}

//----------------------------------------------------------------------------
//	FISdk

FISdk::FISdk(){
	FISdkIf::RegisterSdk();
}

FISdk::~FISdk(){
}

void FISdk::Clear(){
	Sdk::Clear();
	files.clear();
	imports.clear();
}
/*ObjectIf* FISdk::CreateObject(const IfInfo* info, const void* desc){
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
}*/

bool FISdk::AddChildObject(ObjectIf* o){
	FIFile* file = o->Cast();
	if(file){
		file->sdk = this;
		files.push_back(file);
		return true;
	}
	Import* imp = o->Cast();
	if(imp){
		imports.push_back(imp);
		return true;
	}
	return false;
}

bool FISdk::DelChildObject(ObjectIf* o){
	FIFileIf* file = o->Cast();
	if(file){
		Files::iterator it = find(files.begin(), files.end(), file->Cast());
		if(it != files.end()){
			files.erase(it);
			return true;
		}
	}
	ImportIf* imp = o->Cast();
	if(imp){
		Imports::iterator it = find(imports.begin(), imports.end(), imp->Cast());
		if(it != imports.end()){
			imports.erase(it);
			return true;
		}
	}
	return false;
}

FIFileXIf* FISdk::CreateFileX(){
	//FIFileX* rv = DBG_NEW FIFileX;
	//rv->sdk = this;
	//files.push_back(rv);
	//return rv->Cast();
	return DCAST(FIFileXIf, CreateObject(FIFileXIf::GetIfInfoStatic(), &FIFileXDesc()));
}

FIFileSprIf* FISdk::CreateFileSpr(){
	//FIFileSpr* rv = DBG_NEW FIFileSpr;
	//rv->sdk = this;
	//files.push_back(rv);
	//return rv->Cast();
	return DCAST(FIFileSprIf, CreateObject(FIFileSprIf::GetIfInfoStatic(), &FIFileSprDesc()));
}

FIFileVRMLIf* FISdk::CreateFileVRML(){
	//FIFileVRML* rv = DBG_NEW FIFileVRML;
	//rv->sdk = this;
	//files.push_back(rv);
	//return rv->Cast();
	return DCAST(FIFileVRMLIf, CreateObject(FIFileVRMLIf::GetIfInfoStatic(), &FIFileVRMLDesc()));
}

FIFileCOLLADAIf* FISdk::CreateFileCOLLADA(){
	//FIFileCOLLADA* rv = DBG_NEW FIFileCOLLADA;
	//rv->sdk = this;
	//files.push_back(rv);
	//return rv->Cast();
	return DCAST(FIFileCOLLADAIf, CreateObject(FIFileCOLLADAIf::GetIfInfoStatic(), &FIFileCOLLADADesc()));
}

FIFileBinaryIf* FISdk::CreateFileBinary(){
	//FIFileBinary* rv = DBG_NEW FIFileBinary;
	//rv->sdk = this;
	//files.push_back(rv);
	//return rv->Cast();
	return DCAST(FIFileBinaryIf, CreateObject(FIFileBinaryIf::GetIfInfoStatic(), &FIFileBinaryDesc()));
}

FIFileIf* FISdk::CreateFile(const IfInfo* ii){
/*	if(ii == FIFileXIf::GetIfInfoStatic())
		return CreateFileX();
	if(ii == FIFileVRMLIf::GetIfInfoStatic())
		return CreateFileVRML();
	if(ii == FIFileCOLLADAIf::GetIfInfoStatic())
		return CreateFileCOLLADA();
	if(ii == FIFileBinaryIf::GetIfInfoStatic())
		return CreateFileBinary();
	return NULL;*/
	return DCAST(FIFileIf, CreateObject(ii, 0));
}

FIFileIf* FISdk::CreateFileFromExt(UTString filename){
	UTPath name;
	name.Path(filename);
	UTString ext = name.Ext();

	if(!ext.compare(".x"))
		return CreateFileX();

	if(!ext.compare(".spr") || !ext.compare(".Spr") || !ext.compare(".SPR"))
		return CreateFileSpr();

	if(!ext.compare(".wrl"))
		return CreateFileVRML();
	
	if(!ext.compare(".dae"))
		return CreateFileCOLLADA();
	
	if(!ext.compare(".dat"))
		return CreateFileBinary();
	
	return NULL;
}

ImportIf* FISdk::CreateImport(){
	//Import* import = DBG_NEW Import;
	//imports.push_back(import);
	//return import->Cast();
	ImportIf* imp = CreateObject(ImportIf::GetIfInfoStatic(), &ImportDesc())->Cast();
	AddChildObject(imp);
	return imp;
}

ImportIf*	FISdk::CreateImport(ImportIf* parent, UTString path, ObjectIf* owner, const ObjectIfs& children){
	Import* imp = CreateObject(ImportIf::GetIfInfoStatic(), &ImportDesc())->Cast();
	imp->path = path;
	imp->ownerObj = owner;
	imp->childObjs = children;

	DCAST(Import, parent)->AddChild(imp);

	return imp->Cast();
}

}
