// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <HumanInterface/HIBase.h>

using namespace Spr;

class MyApp: public FWAppGLUT{
public:
	void Step(){
		FWAppGLUT::Step();
		PHSolidIf* s = DCAST(PHSolidIf,  fwScene->FindObject("soBlock1"));
	}
} app;


int SPR_CDECL main(int argc, char* argv[]){
	app.Init(argc, argv);
#ifdef _DEBUG
	app.SetDebugMode(FWApp::DM_DEBUG);
#endif
	app.LoadScene("test.x");
//	app.GetFWScene()->AddHumanInterface(new HIMouse);
	HIPose* poseObj = new HIPose;
	HIOrientationIf* oriIf = poseObj->Cast();

	app.Start();
	return 0;
}
