// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <windows.h>
#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>
#include <gl/glut.h>

using namespace Spr;

class MyApp;
MyApp* app;
int quitCount = 0;

class MyApp: public FWAppGLUT{
protected:
	bool bRun;
public:
	MyApp():bRun(true){
	}
	void Step(){
		if (bRun){
			FWAppGLUT::Step();
			//PHSolidIf* s = DCAST(PHSolidIf,  GetSdk()->GetScene()->FindObject("soBlock1"));
			SetCurrentWin(GetWin(0));
			glutPostRedisplay();
			if (GetWin(1)){
				SetCurrentWin(GetWin(1));
				glutPostRedisplay();
			}
		}
		quitCount ++;
		if (quitCount > 60*60) exit(0);
	}
	void Keyboard(int key, int x, int y){
		if (key==0x1b){
			delete app;
			exit(0);
		}
		if (key=='p'){
			if (!bRun) FWAppGLUT::Step();
			bRun = false;
		}
		if (key=='r'){
			bRun = true;
		}
		if (key=='f'){
			GetSdk()->SaveScene("save.x");
		}
		if (key=='w'){
			FWWin* win = GetCurrentWin();
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
		FWWin* wr = GetCurrentWin();
#ifdef _DEBUG
		GetSdk()->SetDebugMode(true);
		GRDebugRenderIf* r = wr->render->Cast();
		r->SetRenderMode(false, true);
//		r->EnableRenderAxis();
		r->EnableRenderForce();
		r->EnableRenderContact();
#endif
		GRCameraIf* cam = wr->scene->GetGRScene()->GetCamera();
		if (cam && cam->GetFrame()){
			//Affinef af = cam->GetFrame()->GetTransform();
			cam->GetFrame()->SetTransform(cameraInfo.view);
		}else{
			wr->render->SetViewMatrix(cameraInfo.view.inv());
		}
		FWAppGLUT::Display();
	}
};


int SPR_CDECL main(int argc, char* argv[]){
	app = new MyApp;
	app->Init(argc, argv);

	app->GetSdk()->Clear();	//	SDK全体を初期化

	//	シーンの構築
#if 0	//	シーンの構築をC++言語でやる場合
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	PHSceneIf* phscene = app->GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phscene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));
#else	//	シーンをファイルからロードするばあい。
	app->GetSdk()->LoadScene("scene.x");
#endif
	//	ウィンドウ1を作成
	FWAppGLUTDesc wd;
	wd.left = 0; wd.top = 0; wd.width = 500; wd.title = "original scene";
	FWWin* w0 = app->CreateWin(wd);	//	作成と同時に，シーン0 がウィンドウ0 に割り当てられる．

#if 1
	//	シーンのセーブ
	app->GetSdk()->SaveScene("save.x");
	//	セーブしたシーンのロード(2番目のシーン(シーン1)がロードされる)
	app->GetSdk()->LoadScene("save.x");
	app->GetSdk()->SaveScene("save2.x");
	
	//	ロードしたシーン1をウィンドウ1に表示するように設定
	wd.left = 512; wd.top = 0; wd.width = 500; wd.title = "saved scene";
	FWWin* w1 = app->CreateWin(wd);
	w1->scene = app->GetSdk()->GetScene(1);
#endif
	app->Start();
	return 0;
}
