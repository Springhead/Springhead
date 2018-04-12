// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

int quitCount = 0;

class MyApp: public FWApp{
protected:
	bool bRun;
public:
	MyApp():bRun(true){
	}
	void TimerFunc(int id){
		if (bRun){
			FWApp::TimerFunc(id);
			quitCount++;
			if (quitCount > 60 * 60) exit(0);
		}
	}
	void Keyboard(int key, int x, int y){
		if (key==0x1b){
			exit(0);
		}
		if (key=='p'){
			if (!bRun) FWApp::TimerFunc(0);
			bRun = false;
		}
		if (key=='r'){
			bRun = true;
		}
		if (key=='f'){
			GetSdk()->SaveScene("save.x");
		}
		if (key=='w'){
			FWWinIf* win = GetCurrentWin();
			FWSceneIf* s = win->GetScene();
			if (GetCurrentWin()->GetFullScreen()){
				DestroyWin(GetCurrentWin());
				FWWinDesc wd;
				wd.height = 600;
				wd.width = 800;
				win = CreateWin(wd);
			}else{
				DestroyWin(GetCurrentWin());
				FWWinDesc wd;
				wd.fullscreen = true;
				wd.height = 768;
				wd.width = 1024;
				win = CreateWin(wd);				
			}
			win->SetScene(s);
		}
	}
	void Display(){
#ifdef _DEBUG
		GetSdk()->SetDebugMode(true);
		FWWinIf* w = GetCurrentWin();
		FWSceneIf* fs = w->GetScene();
		fs->SetRenderMode(false, true);
		fs->EnableRenderAxis();
		fs->EnableRenderForce();
		fs->EnableRenderContact();
#endif
		FWApp::Display();
	}
} app;


int SPR_CDECL main(int argc, char* argv[]){
	//_crtBreakAlloc = 171212;
	{
		//	Window を自動でつくらないために、app.Init()を呼ばずに他だけを初期化。
		app.CreateSdk();
		app.GetSdk()->CreateScene();
		app.GRInit(argc, argv);
		// タイマを作成
		app.CreateTimer();

		//	シーンの構築
#if 0	//	シーンの構築をC++言語でやる場合
		app.GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
		PHSceneIf* phscene = app.GetSdk()->GetScene()->GetPHScene();
		PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
		floor->SetDynamical(false);
		CDBoxDesc boxdesc;
		boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
		floor->AddShape(app.GetSdk()->GetPHSdk()->CreateShape(boxdesc));
#else	//	シーンをファイルからロードするばあい。
		app.GetSdk()->LoadScene("scene.x");
#endif
		//	ウィンドウ1を作成
		FWWinDesc wd;
		wd.left = 0; wd.top = 0; wd.width = 500; wd.title = "original scene";
		FWWinIf* w0 = app.CreateWin(wd);	//	作成と同時に，シーン0 がウィンドウ0 に割り当てられる．

#if 1
	//	シーンのセーブ
		app.GetSdk()->SaveScene("save.x");
		//	セーブしたシーンのロード(2番目のシーン(シーン1)がロードされる)
		app.GetSdk()->LoadScene("save.x");
		app.GetSdk()->SaveScene("save2.x");

		//	ロードしたシーン1をウィンドウ1に表示するように設定
		wd.left = 512; wd.top = 0; wd.width = 500; wd.title = "saved scene";
		FWWinIf* w1 = app.CreateWin(wd);
		w1->SetScene(app.GetSdk()->GetScene(1));
#endif
	}
	app.StartMainLoop();
	return 0;
}
