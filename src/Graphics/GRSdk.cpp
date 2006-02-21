#include <Graphics/Graphics.h>
#pragma hdrstop

namespace Spr{;

struct GRSdks{
	typedef std::vector< GRSdkIf* > Cont;
	Cont* cont;

	GRSdks(){
#if defined _MSC_VER && _DEBUG
		// ‚±‚ÌŠÖ”‚ðŒÄ‚ñ‚¾Žž“_‚ÅŠJ•ú‚³‚ê‚Ä‚¢‚È‚¢ƒƒ‚ƒŠ‚Ìî•ñ‚ð•\Ž¦‚·‚é
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif		
		cont = DBG_NEW Cont;
	}
	~GRSdks(){
		while(cont->size()){
			GRSdkIf* sdk = cont->back();
			cont->erase(cont->end()-1);
			sdk->DelRef();
			if (sdk->RefCount()==0){
				delete sdk;
			}
		}
		delete cont;
	}
};
static GRSdks sdks;
GRSdkIf* _cdecl CreateGRSdk(){
	GRSdkIf* rv = DBG_NEW GRSdk;
	sdks.cont->push_back(rv);
	rv->AddRef();
	return rv;
}

//----------------------------------------------------------------------------
//	GRSdk
OBJECTIMP(GRSdk, Object);
IF_IMP(GRSdk, Object);
GRSdk::~GRSdk(){
	for(GRSdks::Cont::iterator it = sdks.cont->begin(); it != sdks.cont->end(); ++it){
		if (*it == this){
			sdks.cont->erase(it);
			DelRef();
			break;
		}
	}
}
GRDebugRenderIf* GRSdk::CreateDebugRender(){
	GRDebugRender* rv = DBG_NEW GRDebugRender;
	objects.push_back(rv);
	return rv;
}
GRDeviceGLIf* GRSdk::CreateDeviceGL(int w){
	GRDeviceGL* rv = DBG_NEW GRDeviceGL(w);
	objects.push_back(rv);
	return rv;
}

}
