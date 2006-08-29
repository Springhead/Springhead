// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <tchar.h>
#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/FWAppGL.h>

namespace Spr{
}

using namespace Spr;

UTRef<FWSdkIf> fwSdk;
int _tmain(int argc, _TCHAR* argv[])
{
	fwSdk = FWSdkIf::CreateSdk();
	PHSdkIf::Register();
	GRSdkIf::Register();
	FWSdkIf::Register();

	FWAppGLIf* fwApp = CreateFWAppGL();
//	fwApp->SetDebugMode(FWAppGLDesc::DM_DEBUG);
	fwApp->StartApp("block.x", 2000);

	return 0;
}

