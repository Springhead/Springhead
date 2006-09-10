// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>

using namespace Spr;

FWAppGLUT app;

int SPR_CDECL main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.SetDebugMode(FWApp::DM_DEBUG);
	app.LoadScene("test.x");
	app.Start();
	return 0;
}
