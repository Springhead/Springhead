#include <Springhead.h>
#include <Foundation/Object.h>
#include "PHSdk.h"
#include "PHScene.h"

#pragma hdrstop
//	メモリリークチェッカ
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


namespace Spr{;

struct Sdks{
	typedef std::vector< PHSdkIf* > Cont;
	Cont* cont;

	_CrtMemState memStart;
	Sdks(){
		_CrtMemCheckpoint(&memStart);
		cont = new Cont;
	}
	~Sdks(){
		while(cont->size()){
			PHSdkIf* sdk = cont->back();
			cont->erase(cont->end()-1);
			sdk->DelRef();
			if (sdk->RefCount()==0){
				delete sdk;
			}
		}
		delete cont;
		_CrtMemDumpAllObjectsSince(&memStart);
	}
};
static Sdks sdks;
PHSdkIf* CreatePHSdk(){
	PHSdkIf* rv = new PHSdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}

//----------------------------------------------------------------------------
//	PHSdk
OBJECTIMP(PHSdk, Object);
PHSdk::~PHSdk(){
	for(Sdks::Cont::iterator it = sdks.cont->begin(); it != sdks.cont->end(); ++it){
		if (*it == this){
			sdks.cont->erase(it);
			DelRef();
			break;
		}
	}
}

PHSceneIf* PHSdk::CreateScene(){
	PHSceneIf* rv = new PHScene(this);
	scenes.push_back(rv);
	return rv;
}

}
