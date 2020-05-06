/**
\page pageFWAppSample フレームワークSDKを使った一番簡単なサンプル
 Springhead2/src/Samples/FWAppSample

- プログラムの機能
 - 床の上に箱が載っているだけのシーン。
 - 描画は \ref gpPhysics のデバッグ表示のみ。
- ソースについて
 - シーンの構築は、C++言語でAPIを呼び出して行う。
 - \ref gpFramework と \ref gpPhysics のみを使用
*/

#include <Springhead.h>
#include <Framework/SprFWApp.h>
//#include "Samples\SampleApp.h"
using namespace Spr;

class MyApp : public FWApp{
public:
	virtual void Init(int argc = 0, char* argv[] = 0){
		FWApp::Init(argc, argv);

		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
		CDBoxDesc bd;
		
		// 床を作成
		PHSolidIf* floor = phscene->CreateSolid();
		floor->SetDynamical(false);
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, -1.0));
	
		// 箱を作成
		PHSolidIf* box = phscene->CreateSolid();
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		box->AddShape(phSdk->CreateShape(bd));
		box->SetFramePosition(Vec3d(-0.1, 0.0, -1.0));
		
		GetSdk()->SetDebugMode(true);
	}
} app;

#if _MSC_VER <= 1600
#include <GL/glut.h>
#endif
int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
