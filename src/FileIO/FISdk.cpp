#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <stdlib.h>

namespace Spr{;
void FIRegisterTypeDescs();


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

ObjectIf* SPR_CDECL CreateSdk(const IfInfo* info, const void* desc){
	if (info == PHSdkIf::GetIfInfoStatic()){
		return CreatePHSdk();
	}
	if (info == GRSdkIf::GetIfInfoStatic()){
		return CreateGRSdk();
	}
	return NULL;
}


//----------------------------------------------------------------------------
//	FISdk
OBJECTIMP(FISdk, Object);
IF_IMP(FISdk, NameManager);


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
	if (info->Inherit(FIFileXIf::GetIfInfoStatic())){
		return CreateFileX();
	}
	return NULL;
}
FIFileXIf* FISdk::CreateFileX(){
	FIFileX* rv = DBG_NEW FIFileX;
	files.push_back(rv);
	return rv;
}

}
