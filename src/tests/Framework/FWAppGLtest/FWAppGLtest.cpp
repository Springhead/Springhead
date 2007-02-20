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
		PHSolidIf* s = DCAST(PHSolidIf,  GetSdk()->GetScene()->FindObject("soBlock1"));
	}
} app;


int SPR_CDECL main(int argc, char* argv[]){
	app.Init(argc, argv);
#ifdef _DEBUG
//	app.SetDebugMode(FWApp::DM_DEBUG);
#endif
	app.GetSdk()->Clear();
	app.GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app.GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app.GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	app.GetSdk()->LoadScene("test.x");
//	app.GetFWScene()->AddHumanInterface(new HIMouse);

	app.Start();
	return 0;
}
