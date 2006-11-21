// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <HIS/HIMouse.h>

using namespace Spr;

FWAppGLUT app;

int SPR_CDECL main(int argc, char* argv[]){
	app.Init(argc, argv);
#ifdef _DEBUG
	app.SetDebugMode(FWApp::DM_DEBUG);
#endif
	app.LoadScene("test.x");
	app.GetFWScene()->AddHumanInterface(new HIMouse);
	app.Start();
	return 0;
}
