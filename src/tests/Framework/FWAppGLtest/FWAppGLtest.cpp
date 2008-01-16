// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <HumanInterface/HIBase.h>

using namespace Spr;

class MyApp;
MyApp* app;

class MyApp: public FWAppGLUT{
public:
	void Step(){
		FWAppGLUT::Step();
		PHSolidIf* s = DCAST(PHSolidIf,  GetSdk()->GetScene()->FindObject("soBlock1"));
	}
	void Keyboard(unsigned char key, int x, int y){
		if (key==0x1b){
			delete app;
			exit(0);
		}
	}
	void Display(){
#ifdef _DEBUG
		GetSdk()->SetDebugMode(true);
		FWSdkIf::FWWinRender* wr = fwSdk->GetWinRender(GetWin());
		GRDebugRenderIf* r = wr->render->Cast();
		r->SetRenderMode(false, true);
//		r->EnableRenderAxis();
		r->EnableRenderForce();
#endif
		FWAppGLUT::Display();
	}
};


int SPR_CDECL main(int argc, char* argv[]){
	app = new MyApp;
	app->Init(argc, argv);
	app->GetSdk()->Clear();
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app->GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	app->GetSdk()->LoadScene("scene.x");
//	app->GetFWScene()->AddHumanInterface(new HIMouse);
	int w1 = app->CreateWin();
	int w2 = app->CreateWin();
	FWSdkIf::FWWinRender* wr = app->GetSdk()->GetWinRender(w1);
	wr->scene = app->GetSdk()->GetScene();
	
	wr = app->GetSdk()->GetWinRender(w2);
	wr->scene = app->GetSdk()->GetScene();

	app->Start();
	return 0;
}
