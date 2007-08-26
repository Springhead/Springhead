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
	void Keyboard(unsigned char key, int x, int y){
		if (key==0x1b){
			exit(0);
		}
	}
};


int SPR_CDECL main(int argc, char* argv[]){
	MyApp app;
	app.Init(argc, argv);
#ifdef _DEBUG
	app.GetSdk()->SetDebugMode(true);
#endif
	app.GetSdk()->Clear();
	app.GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app.GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app.GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	app.GetSdk()->LoadScene("block.x");
//	app.GetSdk()->GetPHSdk()->GetScene(0)->SetContactMode(PHSceneDesc::MODE_LCP);
//	app.GetFWScene()->AddHumanInterface(new HIMouse);

	app.Start();
	return 0;
}
