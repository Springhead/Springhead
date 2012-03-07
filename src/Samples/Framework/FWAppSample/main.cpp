/**
\page pageFWAppSample フレームワークSDKを使った一番簡単なサンプル
 Springhead2/src/Samples/FWAppSample

- プログラムの機能
 - 床の上に箱が載っているだけのシーン。
 - 描画は \ref pagePhysics のデバッグ表示のみ。
- ソースについて
 - シーンの構築は、C++言語でAPIを呼び出して行う。
 - \ref pageFramework と \ref pagePhysics のみを使用
*/

#include <Springhead.h>
#include <Framework/SprFWApp.h>
//#include "Samples\SampleApp.h"
using namespace Spr;

class MyApp : public FWApp{
public:
	HIXbox360ControllerIf* con;
	PHSolidIf* pointer;
	PHSolidIf* box;
	virtual void Init(int argc = 0, char* argv[] = 0){
		FWApp::Init(argc, argv);

		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
		CDBoxDesc bd;
		con = GetSdk()->GetHISdk()->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
		
		// 床を作成
		PHSolidIf* floor = phscene->CreateSolid();
		floor->SetDynamical(false);
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, -1.0));
	
		// 箱を作成
		box = phscene->CreateSolid();
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		box->AddShape(phSdk->CreateShape(bd));
		box->SetFramePosition(Vec3d(-0.1, 0.0, -1.0));
		box->SetDynamical(false);

		pointer = phscene->CreateSolid();
		pointer->AddShape(phSdk->CreateShape(bd));
		pointer->SetFramePosition(Vec3d(0.0, 1.0, -1.0));
		pointer->SetDynamical(false);
		

		GetSdk()->SetDebugMode(true);
	}

	void TimerFunc(int id){
		UserFunc();
		//GetSdk()->Step();
		box->AddForce(Vec3d(-100.0, 0.0, 0.0));
		con->Update(GetCurrentWin()->GetScene()->GetPHScene()->GetTimeStep());
		GetCurrentWin()->GetScene()->Step();
		PostRedisplay();
	}

	void Display(){
		GetCurrentWin()->Display();
	}

	void Keyboard(int key, int x, int y){
		double distance = 0.1;
	switch(key){
		case 'a':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(-distance, 0.0, 0.0));
			}
			break;
		case 's':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(distance, 0.0, 0.0));
			}
			break;
		case 'w':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(0.0, distance, 0.0));
			}
			break;
		case 'z':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(0.0, -distance, 0.0));
				//sobox->SetDynamical(false);
			}
			break;	
	}
	}
} app;

int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
