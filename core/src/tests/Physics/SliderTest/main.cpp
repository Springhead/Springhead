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
using namespace std;

class MyApp : public FWApp{
public:
	virtual void Init(int argc = 0, char* argv[] = 0){
		FWApp::Init(argc, argv);
		InitCamera();
		BuildScene();
	}
	void InitCamera() {
		Vec3d pos = Vec3d(-0.05, 0.2, 0.3);
		GetCurrentWin()->GetTrackball()->SetPosition(pos);
		Vec3d target = Vec3d(0.1, 0.0, 0.0);
		GetCurrentWin()->GetTrackball()->SetTarget(target);	// カメラ初期位置の設定
	}
	PHSolidIf* soRight = NULL;
	PHSolidIf* dynamicalOffSolidForCalc;
	PHSolidIf* solid1ForCalc;
	PHSolidIf* dynamicalOffSolidForTest;
	PHSolidIf* solid1ForTest;
	PHRootNodeIf* phRootNodeIfForCalc;
	PHTreeNodeIf* phTreeNodeIfForCalc;
	PHRootNodeIf* phRootNodeIfForTest;
	PHTreeNodeIf* phTreeNodeIfForTest;
	Vec3d wdot, a;
	PHBallJointIf* ballJointForTest;

	void BuildScene() {
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();
		double timeStep = (1.0 / 60);
		phScene->SetGravity(Vec3d(0, 0, 0));

		//soFloor = CreateFloor(true);
		CDBoxDesc bd;
		bd.boxsize = Vec3d(0.02, 0.02, 0.02);
		//cout << "ShapeScale() " << ShapeScale() << endl;
		CDBoxIf* boxShape = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		double density = 0.3 / boxShape->CalcVolume();
		boxShape->SetDensity(density);
		Vec3d solid1Position;
		Vec3d jointPosition;
		// トルク求めるためのもの
		{
			dynamicalOffSolidForCalc = phScene->CreateSolid();
			{
				dynamicalOffSolidForCalc->SetName("DynamicalOffSolid");
				dynamicalOffSolidForCalc->SetDynamical(false);
				dynamicalOffSolidForCalc->AddShape(boxShape);
				Vec3d p(0, 0.1, 0);
				Quaterniond q(1, 0, 0, 0);
				dynamicalOffSolidForCalc->SetMass(1);
				dynamicalOffSolidForCalc->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
				dynamicalOffSolidForCalc->SetFramePosition(p);
				dynamicalOffSolidForCalc->SetOrientation(q);
				//dynamicalOffSolidForCalc->SetInertia();
			}
			solid1ForCalc = phScene->CreateSolid();
			{
				solid1ForCalc->SetName("DynamicalOffSolid");
				solid1ForCalc->SetDynamical(true);
				solid1ForCalc->AddShape(boxShape);
				Vec3d p(0.05, 0.1, 0);
				Quaterniond q(1, 0, 0, 0);
				solid1ForCalc->SetMass(1);
				solid1ForCalc->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
				solid1ForCalc->SetFramePosition(p);
				solid1ForCalc->SetOrientation(q);
				//solid1ForCalc->CompInertia();
			}
			PHBallJointDesc jdesc;
			Vec3d dynamicalOffSolidPosition = dynamicalOffSolidForCalc->GetFramePosition();
			Vec3d solid1Position = solid1ForCalc->GetFramePosition();
			Vec3d jointPosition = Vec3d(0.05, 0.1, 0);
			jdesc.poseSocket.Pos() = jointPosition - dynamicalOffSolidPosition;
			jdesc.posePlug.Pos() = jointPosition - solid1Position;
			jdesc.spring = 100;
			jdesc.damper = 0;
			PHBallJointIf* ballJointForCalc = (PHBallJointIf*)phScene->CreateJoint(dynamicalOffSolidForCalc, solid1ForCalc, jdesc);
			phRootNodeIfForCalc = phScene->CreateRootNode(dynamicalOffSolidForCalc);
			phTreeNodeIfForCalc = phScene->CreateTreeNode(phRootNodeIfForCalc, solid1ForCalc);
			Quaterniond q_z90 = Quaterniond::Rot(Rad(90), 'z');
			ballJointForCalc->SetTargetPosition(q_z90);

			//Vec3d p1 = dynamicalOffSolidPosition + q_z90 * (solid1Position - jointPosition);
			//a = (p1 - solid1Position) / (timeStep * timeStep);

			phRootNodeIfForCalc->Setup();
		}


		// 適用する方
		{
			dynamicalOffSolidForTest = phScene->CreateSolid();
			{
				dynamicalOffSolidForTest->SetName("DynamicalOffSolid");
				dynamicalOffSolidForTest->SetDynamical(false);
				dynamicalOffSolidForTest->AddShape(boxShape);
				Vec3d p(0, 0.1, 0.1);
				Quaterniond q(1, 0, 0, 0);
				dynamicalOffSolidForTest->SetMass(1);
				dynamicalOffSolidForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
				dynamicalOffSolidForTest->SetFramePosition(p);
				dynamicalOffSolidForTest->SetOrientation(q);
			}
			solid1ForTest = phScene->CreateSolid();
			{
				solid1ForTest->SetName("DynamicalOffSolid");
				solid1ForTest->SetDynamical(true);
				solid1ForTest->AddShape(boxShape);
				Vec3d p(0.05, 0.1, 0.1);
				Quaterniond q(1, 0, 0, 0);
				solid1ForTest->SetMass(1);
				solid1ForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
				solid1ForTest->SetFramePosition(p);
				solid1ForTest->SetOrientation(q);
			}
			PHBallJointDesc jdesc;
			Vec3d dynamicalOffSolidPosition = dynamicalOffSolidForTest->GetFramePosition();
			solid1Position = solid1ForTest->GetFramePosition();
			jointPosition = Vec3d(0.05, 0.1, 0.1);
			//jointPosition = (dynamicalOffSolidPosition + solid1Position) / 2;
			jdesc.poseSocket.Pos() = jointPosition - dynamicalOffSolidPosition;
			jdesc.posePlug.Pos() = jointPosition - solid1Position;
			jdesc.spring = 100;
			jdesc.damper = 0;
			ballJointForTest = (PHBallJointIf*)phScene->CreateJoint(dynamicalOffSolidForTest, solid1ForTest, jdesc);
			phRootNodeIfForTest = phScene->CreateRootNode(dynamicalOffSolidForTest);
			phTreeNodeIfForTest = phScene->CreateTreeNode(phRootNodeIfForTest, solid1ForTest);
			//Quaterniond qd; qd.RotationArc();
			Quaterniond q_z90 = Quaterniond::Rot(Rad(90), 'z');
			ballJointForTest->SetTargetPosition(q_z90);
			phRootNodeIfForTest->Setup();

			Vec3d diff = q_z90.RotationHalf();
			wdot = diff / (timeStep * timeStep);
			cout << "diff " << diff << endl;
			cout << "wdot " << wdot << endl;
		}

		SpatialMatrix I = phTreeNodeIfForTest->GetI();
		SpatialVector Z = phTreeNodeIfForTest->GetZ() / timeStep; // Zが力積だからtimeStepで割る
		SpatialVector f;
		
		a = wdot % (solid1Position - jointPosition);
		SpatialVector a2(a, wdot);

		cout << "a2 " << a2 << endl;
		f = I * a2 + Z;
		Vec3d t = f.w(); // トルク
		//ballJointForTest->SetOffsetForce(t);
		
		Vec3d t_f = (solid1Position - jointPosition) % f.v(); // 力をJoint周りのトルクに変換 ^ % どちらも外積優先順位が

		ballJointForTest->SetOffsetForce((t_f + f.w())*15);
		//solid1ForTest->AddForce(t_f + f.v()); //力を引数に トルクは原点関係ない
		cout << "I " << I << endl;
		cout << "Z " << Z << endl;
		cout << "f " << f << endl;
		cout << "SetOffsetForce " << t_f + f.w() << endl;


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
			case '\r':
				bRun = true;
				std::cout << "Run" << std::endl;
		}

	}

	void Step() {
		UserFunc();
		if (GetCurrentWin() && GetCurrentWin()->GetScene()) {
			GetCurrentWin()->GetScene()->Step();
		}
		PostRedisplay();
		ballJointForTest->SetOffsetForce(Vec3d(0, 0, 0));
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
