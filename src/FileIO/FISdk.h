/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FISdk_H
#define FISdk_H
#include <Foundation/Scene.h>
#include <FileIO/SprFISdk.h>

namespace Spr {;

class FIFile;
class Import;

class SPR_DLL FISdk:public Sdk{
public:
	SPR_OBJECTDEF(FISdk);
	typedef std::vector< UTRef<FIFile> > Files;
	Files files;

	typedef std::vector< UTRef<Import> > Imports;
	Imports imports;
	
	FISdk();
	~FISdk();
	FIFileXIf*			CreateFileX();
	FIFileVRMLIf*		CreateFileVRML();
	FIFileCOLLADAIf*	CreateFileCOLLADA();
	FIFileBinaryIf*		CreateFileBinary();
	FIFileIf*			CreateFile(const IfInfo* ii);
	FIFileIf*			CreateFileFromExt(UTString filename);
	
	ImportIf*			CreateImport();
	ImportIf*			CreateImport(ImportIf* parent, UTString path, ObjectIf* owner, const ObjectIfs& children);

	bool DelChildObject(ObjectIf* o);
	void Clear();
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
};

}
#endif
