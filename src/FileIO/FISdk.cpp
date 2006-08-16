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
void SPR_CDECL FIRegisterTypeDescs();


struct Sdks{
	typedef std::vector< FISdkIf* > Cont;
	Cont* cont;

	Sdks(){
#if defined _DEBUG && _MSC_VER			
		// メモリリークチェッカ
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif		
		cont = DBG_NEW Cont;
	}
	~Sdks(){
		while(cont->size()){
			FISdkIf* sdk = cont->back();
			cont->erase(cont->end()-1);
			sdk->DelRef();
			if (sdk->RefCount()==0){
				delete sdk;
			}
		}
		delete cont;
	}
};
static Sdks sdks;
FISdkIf* SPR_CDECL CreateFISdk(){
	FISdkIf* rv = DBG_NEW FISdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}


//----------------------------------------------------------------------------
//	FISdk
IF_OBJECT_IMP(FISdk, NameManager);


FISdk::FISdk(){
	FIRegisterTypeDescs();
}

FISdk::~FISdk(){
	for(Sdks::Cont::iterator it = sdks.cont->begin(); it != sdks.cont->end(); ++it){
		if (*it == this){
			sdks.cont->erase(it);
			DelRef();
			break;
		}
	}
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
