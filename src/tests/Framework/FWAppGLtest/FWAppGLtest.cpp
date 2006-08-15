// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <tchar.h>
#include <Springhead.h>
#include <SprFramework.h>

namespace Spr{
	void SPR_CDECL PHRegisterTypeDescs();
	void SPR_CDECL CDRegisterTypeDescs();
	void SPR_CDECL GRRegisterTypeDescs();
	void SPR_CDECL FIRegisterTypeDescs();

	void SPR_CDECL FWRegisterTypeDescs();
	void SPR_CDECL FWRegisterFactories();
}

using namespace Spr;

int _tmain(int argc, _TCHAR* argv[])
{
	PHRegisterTypeDescs();
	CDRegisterTypeDescs();
	GRRegisterTypeDescs();
	FIRegisterTypeDescs();
	FWRegisterTypeDescs();
	FWRegisterFactories();

	PHRegisterSdk();
	GRRegisterSdk();
	FWRegisterSdk();

	FWAppGLIf* fwApp = CreateFWAppGL();
	fwApp->StartApp("block.x", 2000);

	return 0;
}

