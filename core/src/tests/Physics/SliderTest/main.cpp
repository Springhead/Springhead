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

class MyApp : public FWApp {
public:
	virtual void Init(int argc = 0, char* argv[] = 0) {
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
	//PHSolidIf* dynamicalOffSolidForCalc;
	//PHSolidIf* solid1ForCalc;
	PHSolidIf* dynamicalOffSolidForTest;
	Vec3d dynamicalOffSolidPosition;
	PHSolidIf* solid1ForTest;
	Vec3d solid1PositionForTest;
	PHSolidIf* solid2ForTest;
	Vec3d solid2PositionForTest;
	PHBallJointIf* ballJoint1ForTest;
	Vec3d ballJoint1PositionForTest;
	PHBallJointIf* ballJoint2ForTest;
	Vec3d ballJoint2PositionForTest;

	Vec3d wdot1;
	Vec3d wdot2;
	PHRootNodeIf* phRootNodeIfForCalc;
	PHTreeNodeIf* phTreeNodeIfForCalc;
	PHRootNodeIf* phRootNodeIfForTest;
	PHTreeNodeIf* ballJoint1TreeNodeForTest;
	PHTreeNodeIf* ballJoint2TreeNodeForTest;
	double timeStep;

	void BuildScene() {
		FWSceneIf* fwScene = GetCurrentWin()->GetScene();
	    fwScene->EnableRenderAxis(true, true, true);		///< 座標軸
		fwScene->SetAxisStyle(FWSceneIf::AXIS_ARROWS);	///< 座標軸のスタイル
		fwScene->SetAxisScale(0.015, 0.015, 0.015);

		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();
		timeStep = (1.0 / 60);
		phScene->SetTimeStep(timeStep);
		phScene->SetGravity(Vec3d(0, 0, 0));

		//soFloor = CreateFloor(true);
		CDBoxDesc bd;
		bd.boxsize = Vec3d(0.02, 0.02, 0.02);
		//cout << "ShapeScale() " << ShapeScale() << endl;
		CDBoxIf* boxShape = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		double density = 0.3 / boxShape->CalcVolume();
		boxShape->SetDensity(density);
		//{
		//	dynamicalOffSolidForCalc = phScene->CreateSolid();
		//	{
		//		dynamicalOffSolidForCalc->SetName("DynamicalOffSolid");
		//		dynamicalOffSolidForCalc->SetDynamical(false);
		//		dynamicalOffSolidForCalc->AddShape(boxShape);
		//		Vec3d p(0, 0.1, 0);
		//		Quaterniond q(1, 0, 0, 0);
		//		dynamicalOffSolidForCalc->SetMass(1);
		//		dynamicalOffSolidForCalc->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
		//		dynamicalOffSolidForCalc->SetFramePosition(p);
		//		dynamicalOffSolidForCalc->SetOrientation(q);
		//		//dynamicalOffSolidForCalc->SetInertia();
		//	}
		//	solid1ForCalc = phScene->CreateSolid();
		//	{
		//		solid1ForCalc->SetName("DynamicalOffSolid");
		//		solid1ForCalc->SetDynamical(true);
		//		solid1ForCalc->AddShape(boxShape);
		//		Vec3d p(0.1, 0.1, 0);
		//		Quaterniond q(1, 0, 0, 0);
		//		solid1ForCalc->SetMass(1);
		//		solid1ForCalc->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
		//		solid1ForCalc->SetFramePosition(p);
		//		solid1ForCalc->SetOrientation(q);
		//		//solid1ForCalc->CompInertia();
		//	}
		//	PHBallJointDesc jdesc;
		//	Vec3d dynamicalOffSolidPosition = dynamicalOffSolidForCalc->GetFramePosition();
		//	Vec3d solid1Position = solid1ForCalc->GetFramePosition();
		//	Vec3d jointPosition = Vec3d(0.05, 0.1, 0);
		//	jdesc.poseSocket.Pos() = jointPosition - dynamicalOffSolidPosition;
		//	jdesc.posePlug.Pos() = jointPosition - solid1Position;
		//	jdesc.spring = 1;
		//	jdesc.damper = 1;
		//	PHBallJointIf* ballJointForCalc = (PHBallJointIf*)phScene->CreateJoint(dynamicalOffSolidForCalc, solid1ForCalc, jdesc);
		//	phRootNodeIfForCalc = phScene->CreateRootNode(dynamicalOffSolidForCalc);
		//	phTreeNodeIfForCalc = phScene->CreateTreeNode(phRootNodeIfForCalc, solid1ForCalc);
		//	Quaterniond q_z90 = Quaterniond::Rot(Rad(90), 'z');
		//	ballJointForCalc->SetTargetPosition(q_z90);

		//	//Vec3d p1 = dynamicalOffSolidPosition + q_z90 * (solid1Position - jointPosition);
		//	//a = (p1 - solid1Position) / (timeStep * timeStep);

		//	phRootNodeIfForCalc->Setup();
		//}


		// 適用する方
		{
			// 固定の剛体を作成
			dynamicalOffSolidForTest = phScene->CreateSolid();
			dynamicalOffSolidForTest->SetName("DynamicalOffSolidForTest");
			dynamicalOffSolidForTest->SetDynamical(false);
			dynamicalOffSolidForTest->AddShape(boxShape);
			dynamicalOffSolidPosition = Vec3d(0, 0.1, 0.1);
			dynamicalOffSolidForTest->SetMass(1);
			dynamicalOffSolidForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
			dynamicalOffSolidForTest->SetFramePosition(dynamicalOffSolidPosition);
			dynamicalOffSolidForTest->SetOrientation(Quaterniond(1, 0, 0, 0));

			// 剛体1を作成
			solid1ForTest = phScene->CreateSolid();
			solid1ForTest->SetName("solid1ForTest");
			solid1ForTest->SetDynamical(true);
			solid1ForTest->AddShape(boxShape);
			solid1PositionForTest = Vec3d(0.1, 0.1, 0.1);
			solid1ForTest->SetMass(1);
			solid1ForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
			solid1ForTest->SetFramePosition(solid1PositionForTest);
			solid1ForTest->SetOrientation(Quaterniond(1, 0, 0, 0));

			// 剛体2を作成
			solid2ForTest = phScene->CreateSolid();
			solid2ForTest->SetName("solid2ForTest");
			solid2ForTest->SetDynamical(true);
			solid2ForTest->AddShape(boxShape);
			solid2PositionForTest = Vec3d(0.2, 0.1, 0.1);
			solid2ForTest->SetMass(10000000000000000000);
			solid2ForTest->SetInertia(Matrix3d(10000000000000000000, 0, 0, 0, 10000000000000000000, 0, 0, 0, 10000000000000000000));
			solid2ForTest->SetFramePosition(solid2PositionForTest);
			solid2ForTest->SetOrientation(Quaterniond(1, 0, 0, 0));

			// 固定の剛体とsolid1を繋ぐボールジョイント1を作成
			PHBallJointDesc jdesc1;
			ballJoint1PositionForTest = Vec3d(0.05, 0.1, 0.1);
			jdesc1.poseSocket.Pos() = ballJoint1PositionForTest - dynamicalOffSolidPosition;
			jdesc1.posePlug.Pos() = ballJoint1PositionForTest - solid1PositionForTest;
			jdesc1.spring = 0;
			jdesc1.damper = 0;
			ballJoint1ForTest = (PHBallJointIf*)phScene->CreateJoint(dynamicalOffSolidForTest, solid1ForTest, jdesc1);
			ballJoint1ForTest->SetName("ballJoint1ForTest");

			// solid1とsolid2を繋ぐボールジョイント2を作成
			PHBallJointDesc jdesc2;
			ballJoint2PositionForTest = Vec3d(0.15, 0.1, 0.1);
			jdesc2.poseSocket.Pos() = ballJoint2PositionForTest - solid1PositionForTest;
			jdesc2.posePlug.Pos() = ballJoint2PositionForTest - solid2PositionForTest;
			jdesc2.spring = 0;
			jdesc2.damper = 0;
			ballJoint2ForTest = (PHBallJointIf*)phScene->CreateJoint(solid1ForTest, solid2ForTest, jdesc2);
			ballJoint2ForTest->SetName("ballJoint2ForTest");

			// ABAを使用するためにNodeを構築
			phRootNodeIfForTest = phScene->CreateRootNode(dynamicalOffSolidForTest);
			ballJoint1TreeNodeForTest = phScene->CreateTreeNode(phRootNodeIfForTest, solid1ForTest);
			ballJoint2TreeNodeForTest = phScene->CreateTreeNode(ballJoint1TreeNodeForTest, solid2ForTest);

			// targetPositionを設定
			Quaterniond targetRotationBallJoint1 = Quaterniond::Rot(Rad(90), 'z');
			Quaterniond targetRotationBallJoint2 = Quaterniond::Rot(Rad(90), 'z');
			ballJoint1ForTest->SetTargetPosition(targetRotationBallJoint1);
			ballJoint2ForTest->SetTargetPosition(targetRotationBallJoint2);

			ballJoint1ForTest->UpdateState();
			ballJoint2ForTest->UpdateState();
			phRootNodeIfForTest->Setup();
			cout << "spring " << ballJoint1ForTest->GetSpring() << endl;
			cout << "damper  " << ballJoint1ForTest->GetDamper() << endl;
			Vec3d diff1 = targetRotationBallJoint1.RotationHalf();
			wdot1 = diff1 / (timeStep * timeStep);
			cout << "diff1 " << diff1 << endl;
			cout << "wdot1 " << wdot1 << endl;
			Vec3d diff2 = (targetRotationBallJoint1 * targetRotationBallJoint2).RotationHalf();
			wdot2 = diff2 / (timeStep * timeStep);
			cout << "diff2 " << diff2 << endl;
			cout << "wdot2 " << wdot2 << endl;
		}

		Vec3d t1 = CalcOffsetForceForTracking(ballJoint1ForTest, ballJoint1TreeNodeForTest, ballJoint1PositionForTest, solid1PositionForTest, wdot1);
		Vec3d t2 = CalcOffsetForceForTracking(ballJoint2ForTest, ballJoint2TreeNodeForTest, ballJoint2PositionForTest, solid2PositionForTest, wdot2);

		//Vec3d a1 = wdot1 % (solid1PositionForTest - ballJoint1PositionForTest);
		//Vec3d a2 = wdot1 % (solid2PositionForTest - ballJoint2PositionForTest);

		//Vec3d tx = Vec3d(0.05,0,0) % (1*a1+10000000000000000000*a2);
		//Vec3d ty = Vec3d(0.05,0,0) % (10000000000000000000 * a2+ 1*a1/2);
		//cout << "tx " << tx << endl;
		//cout << "ty " << ty << endl;
		ballJoint1ForTest->SetOffsetForce(t1 + t2);
		ballJoint2ForTest->SetOffsetForce(t2);

		cout << endl;

		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderAxis();
	}

	Vec3d CalcOffsetForceForTracking(PHBallJointIf* ballJoint, PHTreeNodeIf* phTreeNode, Vec3d ballJointPosition, Vec3d solidPosition, Vec3d wdot) {
		cout << endl;
		cout << "---------" << ballJoint->GetName() << "---------" << endl;
		SpatialMatrix I = phTreeNode->GetI();
		SpatialVector Z = phTreeNode->GetZ() / timeStep; // Zが力積だからtimeStepで割る
		SpatialVector f;

		Vec3d a = wdot % (solidPosition - ballJointPosition);
		SpatialVector a2(a, wdot); // ballJoint想定のため遠心力の加速度は無視

		cout << "a2 " << a2 << endl;
		f = I * a2 + Z;
		Vec3d t = f.w(); // トルク

		Vec3d t_f = (solidPosition - ballJointPosition) % f.v(); // 力をJoint周りのトルクに変換 ^ % どちらも外積優先順位が

		Vec3d offsetForce = t_f + f.w();
		//ballJoint->SetOffsetForce(offsetForce);
		//solid1ForTest->AddForce(t_f + f.v()); //力を引数に トルクは原点関係ない
		cout << "I " << I;
		cout << "Z " << Z << endl;
		cout << "f " << f << endl;
		cout << "SetOffsetForce " << offsetForce << endl;
		return offsetForce;
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

		Posed solid1ForTestPose = solid1ForTest->GetPose();
		cout << "Solid1ForTest Position " << solid1ForTestPose << endl;
		cout << "Solid1ForTest Rotation  rotationhalf " << solid1ForTestPose.Ori().RotationHalf() << endl;
		Posed solid2ForTestPose = solid2ForTest->GetPose();
		cout << "solid2ForTest Position " << solid2ForTestPose << endl;
		cout << "solid2ForTest Rotation  rotationhalf " << solid2ForTestPose.Ori().RotationHalf() << endl;
		ballJoint1ForTest->SetOffsetForce(Vec3d(0, 0, 0));
		ballJoint2ForTest->SetOffsetForce(Vec3d(0, 0, 0));
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
int __cdecl main(int argc, char* argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
