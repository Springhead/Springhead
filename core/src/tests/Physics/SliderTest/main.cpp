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
#include<stdio.h>
#include <cstdio>
#include <functional>
using namespace Spr;
using namespace std;

typedef void (*FUNC_POINTER)();
void callback1() {
	std::cout << "test";
	//printf("%s\n", s);
}

PHSolidIf* soRight = NULL;
//PHSolidIf* dynamicalOffSolidForCalc;
//PHSolidIf* solid1ForCalc;
PHSolidIf* dynamicalOffSolidForTest;
Vec3d dynamicalOffSolidPosition;
PHSolidIf* solid1ForTest;
Vec3d solid1PositionForTest;
double mSolid1ForTest;
PHSolidIf* solid2ForTest;
Vec3d solid2PositionForTest;
PHBallJointIf* ballJoint1ForTest;
Vec3d ballJoint1PositionForTest;
PHBallJointIf* ballJoint2ForTest;
Vec3d ballJoint2PositionForTest;

Vec3d wdot1Global;
Vec3d wdot1Local;
Vec3d wdot2_old;
PHRootNodeIf* phRootNodeIfForCalc;
PHTreeNodeIf* phTreeNodeIfForCalc;
PHRootNodeIf* phRootNodeIfForTest;
PHTreeNodeIf* ballJoint1TreeNodeForTest;
PHTreeNodeIf* ballJoint2TreeNodeForTest;
double timeStep;

//Quaterniond targetRotationBallJoint1 = Quaterniond::Rot(Vec3d(-0.5, 0.5, -0.5));
//Quaterniond targetRotationBallJoint1 = Quaterniond::Rot(Rad(90), 'z');
Quaterniond targetRotationBallJoint1 = Quaterniond(1, 0, 0, 0);
//Quaterniond targetRotationBallJoint2 = Quaterniond::Rot(Rad(90), 'y');
Quaterniond targetRotationBallJoint2 = Quaterniond(1, 0, 0, 0);
Quaterniond preTargetRotationBallJoint1 = Quaterniond(1, 0, 0, 0);
Quaterniond preTargetRotationBallJoint2 = Quaterniond(1, 0, 0, 0);
Vec3d preLocalW1 = Vec3d(1, 1, 0).unit() * 50;
//Vec3d preLocalW1 = Vec3d(0, 0, 0);
Vec3d preLocalW2 = Vec3d(0, 0, 0);
//Vec3d preLocalW2 = Vec3d(1, 1, 1).unit() * 50;
Vec3d preGlobalW2;
//Vec3d preLocalW2 = Vec3d(1, 1, 1).unit() * 50;
//Vec3d preGlobalW2 = Vec3d(1, 1, 1).unit() * 50;
bool first = true;
void CalcForceWithCoriolis(Spr::PHRootNodeIf* r, void* a) {
	cout << setprecision(15) << endl;
	std::cout << "CalcForceWithCoriolis is called" << endl;
	// targetPositionを設定
	//Quaterniond targetRotationBallJoint1 = Quaterniond::Rot(Rad(90), 'z');
	//Quaterniond targetRotationBallJoint2 = Quaterniond::Rot(Rad(90), 'y');

	//ballJoint1ForTest->UpdateState();
	//ballJoint2ForTest->UpdateState();
	//phRootNodeIfForTest->Setup();

	// 剛体のグローバルの加速度と角加速度を求める
	cout << "spring " << ballJoint1ForTest->GetSpring() << endl;
	cout << "damper  " << ballJoint1ForTest->GetDamper() << endl;

	PHSolidIf* socket1 = ballJoint1ForTest->GetSocketSolid();
	cout << " socket1->GetName()" << socket1->GetName() << endl;
	Posed socketPose1;
	ballJoint1ForTest->GetSocketPose(socketPose1);

	Vec3d diff1Local = (targetRotationBallJoint1 * preTargetRotationBallJoint1.Inv()).RotationHalf();
	Vec3d localW1 = diff1Local / timeStep;
	cout << "solid1ForTest->GetAngularVelocity() " << solid1ForTest->GetAngularVelocity() << endl;
	cout << "preLocalW1 " << preLocalW1 << endl;
	Vec3d solid1Velocity = solid1ForTest->GetAngularVelocity();
	if (solid1Velocity.x != preLocalW1.x || solid1Velocity.y != preLocalW1.y || solid1Velocity.z != preLocalW1.z) {
		cout << "preLocalW1とSolidの速度が違う" << endl;
		cout << fixed << setprecision(15) << solid1Velocity.y << " " << preLocalW1.y << endl;
	}
	wdot1Global = socket1->GetPose().Ori() * socketPose1.Ori() * (localW1 - preLocalW1) / timeStep;
	wdot1Local = (localW1 - preLocalW1) / timeStep; // ジョイントの座標系(ジョイントポーズは含まない)
	cout << "socket1->GetPose().Ori() * socketPose1.Ori() " << socket1->GetPose().Ori() * socketPose1.Ori() << endl;
	cout << "localW1 " << localW1 << endl;
	cout << "preLocalW1" << preLocalW1 << endl;
	cout << "diff1Local " << diff1Local << endl;
	cout << "wdot1Global " << wdot1Global << endl;


	PHSolidIf* socket2 = ballJoint2ForTest->GetSocketSolid();
	Posed socketPose2;
	//preLocalW2 = Vec3d(0, 40, 0);
	//Vec3d diff2Local = (targetRotationBallJoint2 * ((socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity()) * timeStep) * preTargetRotationBallJoint2).Inv()).RotationHalf();
	//cout << "(socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity()) " << (socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity())).RotationHalf() << endl;
	Vec3d diff2Local = (targetRotationBallJoint2 * preTargetRotationBallJoint2.Inv()).RotationHalf();
	cout << "diff2Local " << diff2Local << endl;
	//cout << "(socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity()) " << (socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity())).RotationHalf() << endl;
	Vec3d localW2 = diff2Local / timeStep;
	cout << "localW2 " << localW2 << endl;
	ballJoint2ForTest->GetSocketPose(socketPose2);

	//cout << "ballJoint1ForTest->GetAbsolutePoseQ() " << ballJoint1ForTest->GetAbsolutePoseQ();
	//cout << "ballJoint2ForTest->GetAbsolutePoseQ() " << ballJoint2ForTest->GetAbsolutePoseQ();
	//Vec3d globalW2 = ballJoint2ForTest->GetAbsolutePoseQ() * localW2 + ballJoint1ForTest->GetAbsolutePoseQ() *localW1;
	Vec3d globalW2 = socket2->GetPose().Ori() * socketPose2.Ori() * localW2 + socket1->GetPose().Ori() * socketPose1.Ori() * localW1;

	cout << "solid2ForTest->GetAngularVelocity() " << solid2ForTest->GetAngularVelocity() << endl;
	cout << "preGlobalW2 " << preGlobalW2 << endl;
	Vec3d wdot2Global = (globalW2 - preGlobalW2) / timeStep;
	cout << "preLocalW2 " << preLocalW2 << endl;
	cout << "solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity() " << solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity() << endl;
	cout << "localW2 " << localW2 << endl;
	//Vec3d wdot2Local = (localW2 - (solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity())) / timeStep;
	Vec3d wdot2Local = (localW2 - preLocalW2) / timeStep;
	cout << "wdot2Global " << wdot2Global << endl;
	cout << "wdot2Local " << wdot2Local << endl;

	// Jointの位置を求める
	Posed plugPose1;
	ballJoint1ForTest->GetPlugPose(plugPose1);
	PHSolidIf* plug1 = ballJoint1ForTest->GetPlugSolid();
	ballJoint1PositionForTest = (plug1->GetPose() * plugPose1).Pos();

	Posed plugPose2;
	ballJoint2ForTest->GetPlugPose(plugPose2);
	PHSolidIf* plug2 = ballJoint2ForTest->GetPlugSolid();
	ballJoint2PositionForTest = (plug2->GetPose() * plugPose2).Pos();
	{
		// 手動で力とトルクを求める方法(2つ目のボールジョイントが上手くいかない)
		// Solid1について
		// コリオリ加速度
		SpatialVector c = ballJoint1TreeNodeForTest->GetC();
		Vec3d cAcc = solid1ForTest->GetOrientation() * c.v();
		Vec3d cWdot = solid1ForTest->GetOrientation() * c.w();

		Vec3d aSolid1 = socket1->GetPose().Ori() * socketPose1.Ori() * (wdot1Local % (solid1ForTest->GetPose().Pos() - ballJoint1PositionForTest)); // 円運動する座標系の加速度ABAで使える
		//aSolid1 -= cAcc;
		Vec3d fSolid1 = solid1ForTest->GetMass() * aSolid1;
		//wdot1Global -= cWdot;
		Vec3d tSolid1 = solid1ForTest->GetInertia() * wdot1Global;

		// コリオリ加速度
		SpatialVector c2 = ballJoint2TreeNodeForTest->GetC();
		Vec3d cAcc2 = solid2ForTest->GetOrientation() * c2.v();
		Vec3d cWdot2 = solid2ForTest->GetOrientation() * c2.w();

		// コリオリ力を減算
		SpatialVector Ic = ballJoint1TreeNodeForTest->GetIc() / timeStep;
		std::cout << "Ic " << Ic << endl;

		fSolid1 += solid1ForTest->GetOrientation() * Ic.v();
		tSolid1 += solid1ForTest->GetOrientation() * Ic.w();

		// Solid2について
		Vec3d ballJoint2Acc = (socket2->GetPose().Ori() * socketPose2.Ori() * wdot2Local) % (solid2ForTest->GetPose().Pos() - ballJoint2PositionForTest);
		Vec3d ballJoint1Acc = socket1->GetPose().Ori() * socketPose1.Ori() * (wdot1Local % (solid2ForTest->GetPose().Pos() - ballJoint1PositionForTest));

		cout << "ballJoint2Acc " << ballJoint2Acc << endl;
		cout << "ballJoint1Acc " << ballJoint1Acc << endl;
		Vec3d aSolid2 = ballJoint2Acc + ballJoint1Acc; // 円運動する座標系の加速度ABAで使える
		//aSolid2 -= cAcc2;
		Vec3d fSolid2 = solid2ForTest->GetMass() * aSolid2;
		//wdot2Global -= cWdot2;
		Vec3d tSolid2 = solid2ForTest->GetInertia() * wdot2Global;
		// コリオリ力を減算
		SpatialVector IcBallJoint2 = ballJoint2TreeNodeForTest->GetIc() / timeStep;
		std::cout << "IcBallJoint2 " << IcBallJoint2 << endl;
		fSolid2 += solid2ForTest->GetOrientation() * IcBallJoint2.v();
		tSolid2 += solid2ForTest->GetOrientation() * IcBallJoint2.w();

		cout << "aSolid1 " << aSolid1 << endl;
		cout << "fSolid1 " << fSolid1 << endl;
		cout << "tSolid1 " << tSolid1 << endl;
		solid1ForTest->AddForce(fSolid1);
		solid1ForTest->AddTorque(tSolid1);
		cout << "aSolid2 " << aSolid2 << endl;
		cout << "fSolid2 " << fSolid2 << endl;
		cout << "tSolid2 " << tSolid2 << endl;
		solid2ForTest->AddForce(fSolid2);
		solid2ForTest->AddTorque(tSolid2);
	}
	preTargetRotationBallJoint1 = targetRotationBallJoint1;
	preTargetRotationBallJoint2 = targetRotationBallJoint2;
	preLocalW1 = localW1;
	preLocalW2 = localW2;
	preGlobalW2 = globalW2;
}
class MyApp : public FWApp {
public:
	virtual void Init(int argc = 0, char* argv[] = 0) {
		FWApp::Init(argc, argv);
		InitCamera();
		BuildScene();
	}
	void InitCamera() {
		Vec3d pos = Vec3d(0, 0.05, 0.15); // targetからどこまで離れるか
		GetCurrentWin()->GetTrackball()->SetPosition(pos);
		Vec3d target = Vec3d(0.05, 0.15, 0);
		GetCurrentWin()->GetTrackball()->SetTarget(target);	// カメラ初期位置の設定
	}

	void BuildScene() {
		FWSceneIf* fwScene = GetCurrentWin()->GetScene();
		fwScene->EnableRenderAxis(true, true, true);		///< 座標軸
		fwScene->EnableRenderForce();
		fwScene->SetAxisStyle(FWSceneIf::AXIS_ARROWS);	///< 座標軸のスタイル
		fwScene->SetAxisScale(0.015, 0.015, 0.015);

		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();
		timeStep = (1.0 / 60);
		phScene->SetTimeStep(timeStep);
		phScene->SetGravity(Vec3d(0, 0, 0));
		//phScene->SetNumIteration(50);
		//soFloor = CreateFloor(true);
		CDBoxDesc bd;
		bd.boxsize = Vec3d(0.02, 0.02, 0.02);
		//cout << "ShapeScale() " << ShapeScale() << endl;
		CDBoxIf* boxShape = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		double density = 0.3 / boxShape->CalcVolume();
		boxShape->SetDensity(density);


		// 適用する方
		{
			// 固定の剛体を作成
			dynamicalOffSolidForTest = phScene->CreateSolid();
			dynamicalOffSolidForTest->SetName("DynamicalOffSolidForTest");
			dynamicalOffSolidForTest->SetDynamical(false);
			dynamicalOffSolidForTest->AddShape(boxShape);
			dynamicalOffSolidPosition = Vec3d(0, 0.1, 0);
			dynamicalOffSolidForTest->SetMass(1);
			dynamicalOffSolidForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
			dynamicalOffSolidForTest->SetFramePosition(dynamicalOffSolidPosition);
			dynamicalOffSolidForTest->SetOrientation(Quaterniond(1, 0, 0, 0));

			// 剛体1を作成
			solid1ForTest = phScene->CreateSolid();
			solid1ForTest->SetName("solid1ForTest");
			solid1ForTest->SetDynamical(true);
			solid1ForTest->AddShape(boxShape);
			solid1PositionForTest = Vec3d(0.1, 0.1, 0);
			solid1ForTest->SetMass(100);
			solid1ForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
			solid1ForTest->SetFramePosition(solid1PositionForTest);
			solid1ForTest->SetOrientation(Quaterniond(1, 0, 0, 0));
			solid1ForTest->SetAngularVelocity(preLocalW1);

			cout << "preLocalW1 " << preLocalW1 << endl;
			cout << "solid1ForTest->GetAngularVelocity() " << solid1ForTest->GetAngularVelocity() << endl;
			// 剛体2を作成
			solid2ForTest = phScene->CreateSolid();
			solid2ForTest->SetName("solid2ForTest");
			solid2ForTest->SetDynamical(true);
			solid2ForTest->AddShape(boxShape);
			solid2PositionForTest = Vec3d(0.2, 0.1, 0);
			solid2ForTest->SetMass(10000000);
			solid2ForTest->SetInertia(Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
			solid2ForTest->SetFramePosition(solid2PositionForTest);
			solid2ForTest->SetOrientation(Quaterniond(1, 0, 0, 0));
			//solid2ForTest->SetVelocity(Vec3d(0, 1, 0));
			
			// preGlobalW2の計算
			//PHSolidIf* socket1 = ballJoint1ForTest->GetSocketSolid();
			//Posed socketPose1;
			//ballJoint1ForTest->GetSocketPose(socketPose1);
			//PHSolidIf* socket2 = ballJoint2ForTest->GetSocketSolid();
			//Posed socketPose2;
			//ballJoint2ForTest->GetSocketPose(socketPose2);
			//preGlobalW2 = socket2->GetPose().Ori() * socketPose2.Ori() * preLocalW2 + socket1->GetPose().Ori() * socketPose1.Ori() * preLocalW1;
			preGlobalW2 = preLocalW2 + preLocalW1; // これあまりよくないが今の物理モデルなら良い

			solid2ForTest->SetAngularVelocity(preGlobalW2);

			// 固定の剛体とsolid1を繋ぐボールジョイント1を作成
			PHBallJointDesc jdesc1;
			ballJoint1PositionForTest = Vec3d(0.05, 0.1, 0);
			jdesc1.poseSocket.Pos() = ballJoint1PositionForTest - dynamicalOffSolidPosition;
			jdesc1.posePlug.Pos() = ballJoint1PositionForTest - solid1PositionForTest;
			jdesc1.spring = 0;
			jdesc1.damper = 0;
			ballJoint1ForTest = (PHBallJointIf*)phScene->CreateJoint(dynamicalOffSolidForTest, solid1ForTest, jdesc1);
			ballJoint1ForTest->SetName("ballJoint1ForTest");

			// solid1とsolid2を繋ぐボールジョイント2を作成
			PHBallJointDesc jdesc2;
			ballJoint2PositionForTest = Vec3d(0.15, 0.1, 0);
			jdesc2.poseSocket.Pos() = ballJoint2PositionForTest - solid1PositionForTest;
			jdesc2.posePlug.Ori() = solid2ForTest->GetPose().Ori().Inv() * Quaterniond(1, 0, 0, 0);
			jdesc2.posePlug.Pos() = solid2ForTest->GetPose().Ori().Inv() * (ballJoint2PositionForTest - solid2PositionForTest);
			//cout << "solid2ForTest->GetPose().Ori() " << solid2ForTest->GetPose().Ori() << endl;
			jdesc2.spring = 0;
			jdesc2.damper = 0;
			ballJoint2ForTest = (PHBallJointIf*)phScene->CreateJoint(solid1ForTest, solid2ForTest, jdesc2);
			ballJoint2ForTest->SetName("ballJoint2ForTest");

			// ABAを使用するためにNodeを構築
			phRootNodeIfForTest = phScene->CreateRootNode(dynamicalOffSolidForTest);
			ballJoint1TreeNodeForTest = phScene->CreateTreeNode(phRootNodeIfForTest, solid1ForTest);
			ballJoint2TreeNodeForTest = phScene->CreateTreeNode(ballJoint1TreeNodeForTest, solid2ForTest);

		}

		//Vec3d a1 = wdot1 % (solid1PositionForTest - ballJoint1PositionForTest);
		//Vec3d a2_local = wdot1 % (solid2PositionForTest - ballJoint2PositionForTest);
		//Vec3d a2_global = wdot2 % (solid2PositionForTest - ballJoint2PositionForTest);

		//ballJoint1ForTest->SetOffsetForce(t1 + t2/* + t3 + t4*/);
		//ballJoint2ForTest->SetOffsetForce(t2);

		cout << endl;


		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderAxis();

		phRootNodeIfForTest->SetCompControlForceCallback(CalcForceWithCoriolis, NULL);
		//FUNC_POINTER p;
		//p = callback1;
		//p();
	}

	void Step() {
		//phRootNodeIfForTest->ExecuteCompControlForceCallback();
		//CalcForceWithCoriolis(NULL, NULL);
		//CalcForce(targetRotationBallJoint1, preTargetRotationBallJoint2);
		UserFunc();
		if (GetCurrentWin() && GetCurrentWin()->GetScene()) {
			GetCurrentWin()->GetScene()->Step();
		}
		PostRedisplay();

		Posed solid1ForTestPose = solid1ForTest->GetPose();
		cout << "Solid1ForTest Position " << solid1ForTestPose.Pos() << endl;
		cout << "Solid1ForTest Rotation  rotationhalf " << solid1ForTestPose.Ori().RotationHalf() << endl;
		Posed solid2ForTestPose = solid2ForTest->GetPose();
		cout << "solid2ForTest Position " << solid2ForTestPose.Pos() << endl;
		cout << "solid2ForTest Rotation  rotationhalf " << solid2ForTestPose.Ori().RotationHalf() << endl;
		//ballJoint1ForTest->SetOffsetForce(Vec3d(0, 0, 0));
		//ballJoint2ForTest->SetOffsetForce(Vec3d(0, 0, 0));


		//Posed plugPose;
		//ballJoint2ForTest->GetPlugPose(plugPose);
		//PHSolidIf* plug = ballJoint2ForTest->GetPlugSolid();

		//Posed ballJoint1Pose = plug->GetPose() * plugPose;
		//cout << "ballJoint1Pose " << ballJoint1Pose << endl;
		//cout << "ballJoint1Position " << ballJoint1Pose.Pos() << endl;
	}
	//void CalcForceWithCoriolis(Spr::PHRootNodeIf* r, void* a) {
	//	CalcForce(targetRotationBallJoint1, targetRotationBallJoint2);
	//}
	//void CalcForce(Quaterniond targetRotationBallJoint1, Quaterniond targetRotationBallJoint2) {
	//	// targetPositionを設定
	//	//Quaterniond targetRotationBallJoint1 = Quaterniond::Rot(Rad(90), 'z');
	//	//Quaterniond targetRotationBallJoint2 = Quaterniond::Rot(Rad(90), 'y');

	//	//ballJoint1ForTest->UpdateState();
	//	//ballJoint2ForTest->UpdateState();
	//	//phRootNodeIfForTest->Setup();

	//	// 剛体のグローバルの加速度と角加速度を求める
	//	cout << "spring " << ballJoint1ForTest->GetSpring() << endl;
	//	cout << "damper  " << ballJoint1ForTest->GetDamper() << endl;

	//	PHSolidIf* socket1 = ballJoint1ForTest->GetSocketSolid();
	//	Posed socketPose1;
	//	ballJoint1ForTest->GetSocketPose(socketPose1);

	//	Vec3d diff1 = (targetRotationBallJoint1 * preTargetRotationBallJoint1.Inv()).RotationHalf();
	//	Vec3d localW1 = diff1 / timeStep;//ここ編
	//	cout << "solid1ForTest->GetAngularVelocity() " << solid1ForTest->GetAngularVelocity() << endl;
	//	cout << "preLocalW1 " << preLocalW1 << endl;
	//	wdot1 = socket1->GetPose().Ori() * socketPose1.Ori() * (localW1 - preLocalW1) / timeStep;
	//	cout << "localW1 " << localW1 << endl;
	//	cout << "preLocalW1" << preLocalW1 << endl;
	//	cout << "diff1 " << diff1 << endl;
	//	cout << "wdot1 " << wdot1 << endl;


	//	//PHSolidIf* socket2 = ballJoint2ForTest->GetSocketSolid();
	//	//Posed socketPose2;
	//	/*
	//	preLocalW2 = Vec3d(0, 40, 0);
	//	//Vec3d diff2Local = (targetRotationBallJoint2 * ((socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity()) * timeStep) * preTargetRotationBallJoint2).Inv()).RotationHalf();
	//	//cout << "(socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity()) " << (socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity())).RotationHalf() << endl;
	//	Vec3d diff2Local = (targetRotationBallJoint2 * (Quaterniond::Rot(preLocalW2 * timeStep) * preTargetRotationBallJoint2).Inv()).RotationHalf();
	//	cout << "(socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity()) " << (socket2->GetPose().Ori() * socketPose2.Ori()).Inv() * Quaterniond::Rot((solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity())).RotationHalf() << endl;
	//	Vec3d localW2 = diff2Local / timeStep;
	//	ballJoint1ForTest->GetSocketPose(socketPose1);
	//	ballJoint2ForTest->GetSocketPose(socketPose2);

	//	cout << "ballJoint1ForTest->GetAbsolutePoseQ() " << ballJoint1ForTest->GetAbsolutePoseQ();
	//	cout << "ballJoint2ForTest->GetAbsolutePoseQ() " << ballJoint2ForTest->GetAbsolutePoseQ();
	//	//Vec3d globalW2 = ballJoint2ForTest->GetAbsolutePoseQ() * localW2 + ballJoint1ForTest->GetAbsolutePoseQ() *localW1;
	//	Vec3d globalW2 = socket2->GetPose().Ori() * socketPose2.Ori() * localW2 + socket1->GetPose().Ori() * socketPose1.Ori() *localW1;

	//	cout << "solid2ForTest->GetAngularVelocity() " << solid2ForTest->GetAngularVelocity() << endl;
	//	cout << "preGlobalW2 " << preGlobalW2 << endl;
	//	Vec3d wdot2Global = (globalW2 - solid2ForTest->GetAngularVelocity()) / timeStep;
	//	Quaterniond ballJointOri = solid2ForTest->GetPose().Ori();
	//	cout << "preLocalW2 " << preLocalW2 << endl;
	//	cout << "solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity() " << solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity() << endl;
	//	cout << "localW2 " << localW2 << endl;
	//	//Vec3d wdot2Local = (localW2 - (solid2ForTest->GetAngularVelocity() - solid1ForTest->GetAngularVelocity())) / timeStep;
	//	Vec3d wdot2Local = (localW2 - preLocalW2) / timeStep;
	//	cout << "wdot2Global " << wdot2Global << endl;
	//	cout << "wdot2Local " << wdot2Local << endl;

	//	// Jointの位置を求める
	//	Posed plugPose1;
	//	ballJoint1ForTest->GetPlugPose(plugPose1);
	//	PHSolidIf* plug1 = ballJoint1ForTest->GetPlugSolid();
	//	ballJoint1PositionForTest = (plug1->GetPose() * plugPose1).Pos();

	//	Posed plugPose2;
	//	ballJoint2ForTest->GetPlugPose(plugPose2);
	//	PHSolidIf* plug2 = ballJoint2ForTest->GetPlugSolid();
	//	ballJoint2PositionForTest = (plug2->GetPose() * plugPose2).Pos();
	//	*/
	//	{
	//		// 手動で力とトルクを求める方法(2つ目のボールジョイントが上手くいかない)
	//		// Solid1について
	//		Vec3d aSolid1 = wdot1 % (solid1ForTest->GetPose().Pos() - ballJoint1PositionForTest); // 円運動する座標系の加速度ABAで使える
	//		Vec3d fSolid1 = solid1ForTest->GetMass() * aSolid1;

	//		Vec3d tSolid1 = solid1ForTest->GetInertia() * wdot1;

	//		/*
	//		// Solid2について
	//		Vec3d ballJoint2Acc = (socket2->GetPose().Ori() * socketPose2.Ori() * wdot2Local) % (solid2ForTest->GetPose().Pos() - ballJoint2PositionForTest);
	//		Vec3d ballJoint1Acc = wdot1 % (solid2ForTest->GetPose().Pos() - ballJoint1PositionForTest);

	//		cout << "ballJoint2Acc " << ballJoint2Acc << endl;
	//		cout << "ballJoint1Acc " << ballJoint1Acc << endl;
	//		Vec3d aSolid2 = ballJoint2Acc + ballJoint1Acc; // 円運動する座標系の加速度ABAで使える
	//		Vec3d fSolid2 = solid2ForTest->GetMass() * aSolid2;

	//		Vec3d tSolid2 = solid2ForTest->GetInertia() * wdot2Global;

	//		*/
	//		cout << "aSolid1 " << aSolid1 << endl;
	//		cout << "fSolid1 " << fSolid1 << endl;
	//		cout << "tSolid1 " << tSolid1 << endl;
	//		solid1ForTest->AddForce(fSolid1);
	//		solid1ForTest->AddTorque(tSolid1);
	//		//cout << "aSolid2 " << aSolid2 << endl;
	//		//cout << "fSolid2 " << fSolid2 << endl;
	//		//cout << "tSolid2 " << tSolid2 << endl;
	//		//solid2ForTest->AddForce(fSolid2);
	//		//solid2ForTest->AddTorque(tSolid2);
	//	}
	//	preLocalW1 = localW1;
	//	preTargetRotationBallJoint1 = targetRotationBallJoint1;
	//	preTargetRotationBallJoint2 = targetRotationBallJoint2;
	//	//preLocalW1 = localW1;
	//	//preLocalW2 = localW2;
	//	//preGlobalW2 = globalW2;
	//}
	SpatialVector CalcForceAndTorqueForTracking(PHBallJointIf* ballJoint, PHTreeNodeIf* phTreeNode, Vec3d wdot, Vec3d a) {
		cout << endl;
		cout << "---------" << ballJoint->GetName() << "---------" << endl;
		SpatialMatrix I = phTreeNode->GetI();
		SpatialVector Z = phTreeNode->GetZ() / timeStep; // Zが力積だからtimeStepで割る
		SpatialVector f;

		SpatialVector a2(a, wdot); // ballJoint想定のため遠心力の加速度は無視

		cout << "a2 " << a2 << endl;
		f = I * a2 + Z;
		Vec3d t = f.w(); // トルク

		cout << "I " << I;
		cout << "Z " << Z << endl;
		cout << "f " << f << endl;
		return f;
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
