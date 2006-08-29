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
#include <stdlib.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	FISdkIf
FISdkIf* SPR_CDECL FISdkIf::CreateSdk(){
	FISdkIf* rv = DBG_NEW FISdk;
	return rv;
}

//----------------------------------------------------------------------------
//	FISdk
IF_OBJECT_IMP(FISdk, NameManager);

UTRef<UTTypeDescDb> FISdk::typeDb;

void SPR_CDECL FIRegisterTypeDescs();

FISdk::FISdk(){
	FIRegisterTypeDescs();
}

FISdk::~FISdk(){
}
UTTypeDescDb* FISdk::GetTypeDb(){
	if (!typeDb) typeDb = DBG_NEW UTTypeDescDb;
	return typeDb;
}

void FISdk::Clear(){
	files.clear();
}
ObjectIf* FISdk::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Object::CreateObject(info, desc);
	if (!rv){
		if (info->Inherit(FIFileXIf::GetIfInfoStatic())){
			rv = CreateFileX();
		}
	}
	return rv;
}
FIFileXIf* FISdk::CreateFileX(){
	FIFileX* rv = DBG_NEW FIFileX;
	rv->sdk = this;
	files.push_back(rv);
	return rv;
}

}
