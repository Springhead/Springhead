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

PHBallJointIf* ballJoint;
PHSceneIf * phScene;
UTRef<ObjectStatesIf> state = ObjectStatesIf::Create();

class MyApp: public FWAppGLUT{
protected:
	bool bRun;
public:
	MyApp():bRun(false){
	}
	void Step(){
		if (bRun){
			//	トルクの計算
			state->SaveState(phScene);
			ballJoint->SetMode(PHJointDesc::MODE_VELOCITY);
			ballJoint->SetDesiredVelocity(Vec3f(0,0,10));
			phScene->Step();
			Vec3d f,t;
			ballJoint->GetConstraintForce(f, t);
			state->LoadState(phScene);
			//	実シミュレーション
			ballJoint->SetMode(PHJointDesc::MODE_POSITION);
			ballJoint->SetSpring(1);
			ballJoint->SetDamper(1);
			ballJoint->SetOffsetForce(t);
			DSTR << t << std::endl;
			phScene->Step();

			SetCurrentWin(GetWin(0));
			glutPostRedisplay();
			if (GetWin(1)){
				SetCurrentWin(GetWin(1));
				glutPostRedisplay();
			}
		}
		quitCount ++;
		if (quitCount > 60*60*10) exit(0);
	}
	void Keyboard(int key, int x, int y){
		if (key==0x1b){
			delete app;
			exit(0);
		}
		if (key=='p'){
			if (!bRun){
				bRun = true;
				Step();
			}
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
		r->SetRenderMode(true, true);
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
	app->GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());
	phScene = app->GetSdk()->GetScene()->GetPHScene();
	PHSolidIf* floor = phScene->CreateSolid(PHSolidDesc());
	floor->SetDynamical(false);
	PHSolidIf* box = phScene->CreateSolid(PHSolidDesc());
	CDBoxDesc boxdesc;
	boxdesc.boxsize = Vec3d(0.1, 0.1, 0.1);
	floor->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));
	box->AddShape(app->GetSdk()->GetPHSdk()->CreateShape(boxdesc));

	PHBallJointDesc jdesc;
	//jdesc.limit[1].upper =  0.2;	// 最大スイング角
	//jdesc.limit[2].lower= -0.2;	// ツイスト角範囲
	//jdesc.limit[2].upper =  0.2;
	jdesc.poseSocket.Pos() = Vec3d(0, 0.2, 0);
	jdesc.posePlug.Pos() = Vec3d(0, -0.2, 0);
	ballJoint = phScene->CreateJoint(floor, box, PHBallJointIf::GetIfInfoStatic(), jdesc)->Cast();

	//	ウィンドウ1を作成
	FWAppGLUTDesc wd;
	wd.left = 0; wd.top = 0; wd.width = 500; wd.title = "original scene";
	app->CreateWin(wd);	//	作成と同時に，シーン0 がウィンドウ0 に割り当てられる．
	app->Start();
	return 0;
}
