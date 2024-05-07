/**
\page pageSliderTest フレームワークSDKを使った一番簡単なサンプル
 Springhead2/src/Samples/SliderTest

- プログラムの機能
 - 床の上に箱が載っているだけのシーン。
 - 描画は \ref gpPhysics のデバッグ表示のみ。
- ソースについて
 - シーンの構築は、C++言語でAPIを呼び出して行う。
 - \ref gpFramework と \ref gpPhysics のみを使用
*/

#include <Springhead.h>
#include <Framework/SprFWApp.h>
using namespace Spr;

class MyApp : public FWApp{
public:
	virtual void Init(int argc = 0, char* argv[] = 0){
		FWApp::Init(argc, argv);
		InitCamera();
		BuildScene();
	}
	void InitCamera() {
		Vec3d pos = Vec3d(-0.2, 0.1, 0.4);
		GetCurrentWin()->GetTrackball()->SetPosition(pos);
		Vec3d target = Vec3d(0.1, 0.0, 0.0);
		GetCurrentWin()->GetTrackball()->SetTarget(target);	// カメラ初期位置の設定
	}
	PHSolidIf* soRight = NULL;

	void BuildScene() {
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
		phscene->SetNumIteration(1000);
		phscene->SetTimeStep(0.001);
		CDBoxDesc bd;

		PHSolidIf* soLeft = phscene->CreateSolid();
		soLeft->SetDynamical(false);
		soLeft->SetGravity(false);
		// 箱を作成
		bd.boxsize = 0.01 * Vec3d(1, 1, 1);
		CDShapeIf* shape = phSdk->CreateShape(bd);
		shape->SetDensity(1);
		soLeft->AddShape(shape);
		soLeft->CompInertia();
		soLeft->SetFramePosition(Vec3d(0, 0, 0));
		soLeft->SetInertia(Matrix3d::Unit() * 100000);
		soLeft->SetDynamical(false);


		soRight = phscene->CreateSolid();
		soRight->SetDynamical(true);
		soRight->SetGravity(false);
		bd.boxsize = 0.01 * Vec3d(1, 1, 1);
		shape = phSdk->CreateShape(bd);
		shape->SetDensity(1);
		soRight->AddShape(shape);
		soRight->CompInertia();
		soRight->SetFramePosition(Vec3d(0.3, 0.2, 0));
		PHSliderJointDesc sjd;
		//sjd.spring = 100;
		//sjd.damper = sjd.spring * 0.1;
		sjd.poseSocket.Ori().RotationArc(Vec3d(0, 0, 1), Vec3d(1, 0, 0));
		sjd.posePlug.Ori().RotationArc(Vec3d(0, 0, 1), Vec3d(1, 0, 0));
		PHJointIf* slider = phscene->CreateJoint(soLeft, soRight, sjd)->Cast();

		//	add reversed joint
		//		PHSliderJointDesc sjd2 = sjd;
		PHSliderJointDesc sjd2 = sjd;
//		PHJointIf* slider2 = phscene->CreateJoint(soRight, soLeft, sjd2)->Cast();

		/* //	add shifted joint
		PHSliderJointDesc sjd3 = sjd;
		sjd3.poseSocket.Pos() = Vec3d(0.4, 0, 0);
		PHJointIf* slider3 = phscene->CreateJoint(soLeft, soRight, sjd3)->Cast();	//*/


		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderAxis();
	}
	bool bRun = false;
	void Keyboard(int key, int x, int y) {
		switch (key) {
			case 27:
			case 'q':
				exit(0);
				break;
			case 'r':
				soRight->SetFramePosition(Vec3d(0.3, 1, 0));
				break;
			case ' ':	//	step by space
				if (bRun) {
					bRun = false;
					std::cout << "Paused" << std::endl;
				}
				else {
					Step();
					std::cout << "Step" << std::endl;
				}

				break;
			case '\r': {
				PHSdkIf* phSdk = GetSdk()->GetPHSdk();
				PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
				std::cout << "dt=" << phscene->GetTimeStep() << std::endl;

					bRun = true;
				std::cout << "Run" << std::endl;
			}
		}

	}
	void Step() {
		UserFunc();
		if (GetCurrentWin() && GetCurrentWin()->GetScene()) {
			GetCurrentWin()->GetScene()->Step();
			std::cout << "omega:" << soRight->GetAngularVelocity().z << " y:" << soRight->GetFramePosition().y << std::endl;
		}
		PostRedisplay();
	}

	void TimerFunc(int id) {
		if (bRun) {
			Step();
		}
	}
	
} app;

#if _MSC_VER <= 1600
#include <GL/glut.h>
#endif
int __cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
