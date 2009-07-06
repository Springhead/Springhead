#include "IK.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

#include <GL/glut.h>

#include "Physics/PHConstraint.h"
#include "Physics/PHSpatial.h"
#include "Physics/PHIK.h"
#include "Physics/PHScene.h"

#define ESC 27

IK app;

IK::IK(){
	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= true;

	gravity		= Vec3d(0.0, -9.8, 0.0);
}

void IK::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);

	GetSdk()->Clear();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());

	BuildScene();

	FWWinDesc windowDesc;
	windowDesc.title = "Inverse Kinematics Sample";
	window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();

	InitCameraView();
}

void IK::Reset(int sceneNum){
	GetSdk()->Clear();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());

	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= true;

	BuildScene(sceneNum);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	InitCameraView();
}

void IK::InitCameraView(){
	istringstream iss(
		"((0.9996 0.00141267 -0.0282299 -0.0937266)"
		"(1.59067e-012 0.99875 0.0499792 0.165937)"
		"(0.0282652 -0.0499592 0.998351 3.31465)"
		"(	   0      0      0      1))"
		);
	iss >> cameraInfo.view;
}

void IK::BuildScene(int sceneNum){
	PHSdkIf*	phSdk	= GetSdk()->GetPHSdk();
	PHSceneIf*	phScene	= GetSdk()->GetScene()->GetPHScene();

	soPosCtl1 = NULL;
	soPosCtl2 = NULL;

	// ボールジョイント２つの直線状リンク
	if (sceneNum == 0) {
		PHSolidDesc			descSolid;
		CDBoxDesc			descBox;
		PHBallJointDesc		descBallJoint;
		PHIKBallJointDesc	descIKBall;
		PHIKPosCtlDesc		descIKPos;

		/// -- ルート剛体
		///// 剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		PHSolidIf* so1 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		so1->AddShape(phSdk->CreateShape(descBox));

		/// -- 一つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so2 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so2->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.1f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring =   1.0f;
		descBallJoint.damper =   2.0f;
		PHJointIf* jo1 = phScene->CreateJoint(so1, so2, descBallJoint);
		///// IKノード
		descIKBall.joint = jo1->Cast();
		descIKBall.spring = 1000.0f;
		descIKBall.damper =   20.0f;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);

		/// -- 二つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so3 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so3->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring =   1.0f;
		descBallJoint.damper =   2.0f;
		PHJointIf* jo2 = phScene->CreateJoint(so2, so3, descBallJoint);
		///// IKノード
		descIKBall.joint = jo2->Cast();
		descIKBall.spring = 1000.0f;
		descIKBall.damper =   20.0f;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.solid = so3;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl1);
		ikNode2->AddControlPoint(ikPosCtl1);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl1 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl1->AddShape(phSdk->CreateShape(descBox));
	}

	// ボールジョイント５つの直線状リンク
	if (sceneNum == 1) {
		PHSolidDesc			descSolid;
		CDBoxDesc			descBox;
		PHBallJointDesc		descBallJoint;
		PHIKBallJointDesc	descIKBall;
		PHIKPosCtlDesc		descIKPos;

		/// -- ルート剛体
		///// 剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		PHSolidIf* so1 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		so1->AddShape(phSdk->CreateShape(descBox));

		/// -- 一つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so2 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so2->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.1f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo1 = phScene->CreateJoint(so1, so2, descBallJoint);
		///// IKノード
		descIKBall.joint = jo1->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);

		/// -- 二つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so3 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so3->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo2 = phScene->CreateJoint(so2, so3, descBallJoint);
		///// IKノード
		descIKBall.joint = jo2->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);

		/// -- 三つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so4 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so4->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo3 = phScene->CreateJoint(so3, so4, descBallJoint);
		///// IKノード
		descIKBall.joint = jo3->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode3 = phScene->CreateIKNode(descIKBall);

		/// -- 四つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so5 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so5->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo4 = phScene->CreateJoint(so4, so5, descBallJoint);
		///// IKノード
		descIKBall.joint = jo4->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode4 = phScene->CreateIKNode(descIKBall);

		/// -- 五つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so6 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so6->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo5 = phScene->CreateJoint(so5, so6, descBallJoint);
		///// IKノード
		descIKBall.joint = jo5->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode5 = phScene->CreateIKNode(descIKBall);

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.solid = so6;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl1);
		ikNode2->AddControlPoint(ikPosCtl1);
		ikNode3->AddControlPoint(ikPosCtl1);
		ikNode4->AddControlPoint(ikPosCtl1);
		ikNode5->AddControlPoint(ikPosCtl1);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl1 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl1->AddShape(phSdk->CreateShape(descBox));
	}

	// ボールジョイント５つの直線状リンク（優先度付き・復元バネダンパ強め）
	if (sceneNum == 2) {
		PHSolidDesc			descSolid;
		CDBoxDesc			descBox;
		PHBallJointDesc		descBallJoint;
		PHIKBallJointDesc	descIKBall;
		PHIKPosCtlDesc		descIKPos;

		/// -- ルート剛体
		///// 剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		PHSolidIf* so1 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		so1->AddShape(phSdk->CreateShape(descBox));

		/// -- 一つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so2 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so2->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.1f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo1 = phScene->CreateJoint(so1, so2, descBallJoint);
		///// IKノード
		descIKBall.joint = jo1->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   200.0f;
		descIKBall.bias = 1.8;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);

		/// -- 二つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so3 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so3->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo2 = phScene->CreateJoint(so2, so3, descBallJoint);
		///// IKノード
		descIKBall.joint = jo2->Cast();
		descIKBall.spring = 90000.0f;
		descIKBall.damper =   200.0f;
		descIKBall.bias = 1.6;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);

		/// -- 三つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so4 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so4->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo3 = phScene->CreateJoint(so3, so4, descBallJoint);
		///// IKノード
		descIKBall.joint = jo3->Cast();
		descIKBall.spring = 80000.0f;
		descIKBall.damper =   200.0f;
		descIKBall.bias = 1.4;
		PHIKNodeIf* ikNode3 = phScene->CreateIKNode(descIKBall);

		/// -- 四つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so5 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so5->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo4 = phScene->CreateJoint(so4, so5, descBallJoint);
		///// IKノード
		descIKBall.joint = jo4->Cast();
		descIKBall.spring = 70000.0f;
		descIKBall.damper =   200.0f;
		descIKBall.bias = 1.2;
		PHIKNodeIf* ikNode4 = phScene->CreateIKNode(descIKBall);

		/// -- 五つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so6 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so6->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo5 = phScene->CreateJoint(so5, so6, descBallJoint);
		///// IKノード
		descIKBall.joint = jo5->Cast();
		descIKBall.spring = 60000.0f;
		descIKBall.damper =   200.0f;
		descIKBall.bias = 1.0;
		PHIKNodeIf* ikNode5 = phScene->CreateIKNode(descIKBall);

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.solid = so6;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl1);
		ikNode2->AddControlPoint(ikPosCtl1);
		ikNode3->AddControlPoint(ikPosCtl1);
		ikNode4->AddControlPoint(ikPosCtl1);
		ikNode5->AddControlPoint(ikPosCtl1);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl1 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl1->AddShape(phSdk->CreateShape(descBox));
	}

	// シンプルな２分岐リンク
	if (sceneNum == 3) {
		PHSolidDesc			descSolid;
		CDBoxDesc			descBox;
		PHBallJointDesc		descBallJoint;
		PHIKBallJointDesc	descIKBall;
		PHIKPosCtlDesc		descIKPos;

		/// -- ルート剛体
		///// 剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		PHSolidIf* so1 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		so1->AddShape(phSdk->CreateShape(descBox));

		/// -- 一つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so2 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so2->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.1f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		//descBallJoint.spring =   1.0f;
		//descBallJoint.damper =   2.0f;
		PHJointIf* jo1 = phScene->CreateJoint(so1, so2, descBallJoint);
		///// IKノード
		descIKBall.joint = jo1->Cast();
		descIKBall.spring = 1000.0f;
		descIKBall.damper = 1520.0f;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);

		/// -- 二つ目のリンク（Ａ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so3 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so3->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		//descBallJoint.spring =   1.0f;
		//descBallJoint.damper =   2.0f;
		PHJointIf* jo2 = phScene->CreateJoint(so2, so3, descBallJoint);
		///// IKノード
		descIKBall.joint = jo2->Cast();
		descIKBall.spring = 1000.0f;
		descIKBall.damper = 1520.0f;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);

		/// -- 二つ目のリンク（Ｂ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so4 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so4->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		//descBallJoint.spring =   1.0f;
		//descBallJoint.damper =   2.0f;
		PHJointIf* jo3 = phScene->CreateJoint(so2, so4, descBallJoint);
		///// IKノード
		descIKBall.joint = jo3->Cast();
		descIKBall.spring = 1000.0f;
		descIKBall.damper = 1520.0f;
		PHIKNodeIf* ikNode3 = phScene->CreateIKNode(descIKBall);

		/// -- IK制御点
		///// 制御点１の作成
		descIKPos.solid = so3;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl1);
		ikNode2->AddControlPoint(ikPosCtl1);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl1 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl1->AddShape(phSdk->CreateShape(descBox));

		///// 制御点２の作成
		descIKPos.solid = so4;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl2 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl2);
		ikNode3->AddControlPoint(ikPosCtl2);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl2 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl2->AddShape(phSdk->CreateShape(descBox));
	}

	// 少し長めの２分岐リンク
	if (sceneNum == 4) {
		PHSolidDesc			descSolid;
		CDBoxDesc			descBox;
		PHBallJointDesc		descBallJoint;
		PHIKBallJointDesc	descIKBall;
		PHIKPosCtlDesc		descIKPos;

		/// -- ルート剛体
		///// 剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		PHSolidIf* so1 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		so1->AddShape(phSdk->CreateShape(descBox));

		/// -- 一つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so2 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so2->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.1f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo1 = phScene->CreateJoint(so1, so2, descBallJoint);
		///// IKノード
		descIKBall.joint = jo1->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);

		/// -- 二つ目のリンク
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so3 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so3->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo2 = phScene->CreateJoint(so2, so3, descBallJoint);
		///// IKノード
		descIKBall.joint = jo2->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);

		/// -- 三つ目のリンク（Ａ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so4 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so4->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo3 = phScene->CreateJoint(so3, so4, descBallJoint);
		///// IKノード
		descIKBall.joint = jo3->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode3 = phScene->CreateIKNode(descIKBall);

		/// -- 四つ目のリンク（Ａ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so5 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so5->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo4 = phScene->CreateJoint(so4, so5, descBallJoint);
		///// IKノード
		descIKBall.joint = jo4->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode4 = phScene->CreateIKNode(descIKBall);

		/// -- 五つ目のリンク（Ａ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so6 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so6->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo5 = phScene->CreateJoint(so5, so6, descBallJoint);
		///// IKノード
		descIKBall.joint = jo5->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode5 = phScene->CreateIKNode(descIKBall);

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.solid = so6;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl1);
		ikNode2->AddControlPoint(ikPosCtl1);
		ikNode3->AddControlPoint(ikPosCtl1);
		ikNode4->AddControlPoint(ikPosCtl1);
		ikNode5->AddControlPoint(ikPosCtl1);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl1 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl1->AddShape(phSdk->CreateShape(descBox));


		/// -- 三つ目のリンク（Ｂ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so7 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so7->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo6 = phScene->CreateJoint(so3, so7, descBallJoint);
		///// IKノード
		descIKBall.joint = jo6->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode6 = phScene->CreateIKNode(descIKBall);

		/// -- 四つ目のリンク（Ｂ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so8 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so8->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo7 = phScene->CreateJoint(so7, so8, descBallJoint);
		///// IKノード
		descIKBall.joint = jo7->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode7 = phScene->CreateIKNode(descIKBall);

		/// -- 五つ目のリンク（Ｂ）
		///// 剛体
		descSolid.dynamical = true;
		descSolid.mass = 1.0;
		PHSolidIf* so9 = phScene->CreateSolid(descSolid);
		///// 形状
		descBox.boxsize = Vec3f(0.2f, 1.0f, 0.2f);
		so9->AddShape(phSdk->CreateShape(descBox));
		///// 関節
		descBallJoint.poseSocket.Pos() = Vec3f(0.0f, +0.5f, 0.0f);
		descBallJoint.posePlug.Pos() = Vec3f(0.0f, -0.5f, 0.0f);
		descBallJoint.spring = 1000.0f;
		descBallJoint.damper =   20.0f;
		PHJointIf* jo8 = phScene->CreateJoint(so8, so9, descBallJoint);
		///// IKノード
		descIKBall.joint = jo8->Cast();
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		PHIKNodeIf* ikNode8 = phScene->CreateIKNode(descIKBall);

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.solid = so9;
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl2 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		///// ノードへの登録
		ikNode1->AddControlPoint(ikPosCtl2);
		ikNode2->AddControlPoint(ikPosCtl2);
		ikNode6->AddControlPoint(ikPosCtl2);
		ikNode7->AddControlPoint(ikPosCtl2);
		ikNode8->AddControlPoint(ikPosCtl2);
		///// 制御点を指し示す剛体
		descSolid.dynamical = false;
		descSolid.mass = 1.0;
		soPosCtl2 = phScene->CreateSolid(descSolid);
		descBox.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		soPosCtl2->AddShape(phSdk->CreateShape(descBox));
	}

	// Sceneの設定
	/// IKの設定
	phScene->GetIKEngine()->SetNumIter(50);
	phScene->GetIKEngine()->Enable(bIK);
	/// その他の設定
	phScene->SetContactMode(PHSceneDesc::MODE_NONE);
	phScene->SetGravity(gravity);
	phScene->SetTimeStep(0.01);
	phScene->SetNumIteration(15);
}

void IK::Step(){
	if(bStep) {
		OneStep();
	} else if (bOneStep) {
		OneStep();
		bOneStep = false;
	}

	glutPostRedisplay();
}

void IK::OneStep(){
	PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();

	phScene->Step();

	soPosCtl1->SetFramePosition(ikPosCtl1->GetGoal());
	if (soPosCtl2) {
		soPosCtl2->SetFramePosition(ikPosCtl2->GetGoal());
	}
}

void IK::Display(){
	// 描画の設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = window->render->Cast();

	// 描画モードの設定
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(bDebug);
	render->EnableRenderIK(true, 10.0);

	// カメラ座標の指定
	GRCameraIf* cam = window->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		window->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GRRenderIf*curRender =  GetCurrentWin()->GetRender();
	FWSceneIf* curScene = GetCurrentWin()->GetScene();

	GetSdk()->SwitchScene(curScene);
	GetSdk()->SwitchRender(curRender);

	if(!curRender) return;
	curRender->ClearBuffer();
	curRender->BeginScene();

	if (curScene) curScene->Draw(curRender, GetSdk()->GetDebugMode());

	//	光源の追加
	GRLightDesc ld;
	ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
	ld.specular = Vec4f(1,1,1,1) * 0.8f;
	ld.ambient = Vec4f(1,1,1,1) * 0.4f;
	ld.position = Vec4f(1,1,1,0);
	render->PushLight(ld);
	render->PopLight();

	curRender->EndScene();
	glutSwapBuffers();
}

void IK::Keyboard(int key, int x, int y){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:

		case 'q':
			exit(0);
			break;

		case 'r':
			Reset();
			break;

		case 'w':
			InitCameraView();
			DSTR << "InitCameraView" << endl;
			break;

		case 'd':
			if(bDebug){
				bDebug = false;
				DSTR << "Debug Mode OFF" << endl;
			}else{
				bDebug = true;
				DSTR << "Debug Mode ON" << endl;
			}
			break;

		case 'g':
			if(bGravity){
				bGravity = false;
				phscene->SetGravity(Vec3d(0,0,0));
				DSTR << "Gravity OFF" << endl;
			}else{
				bGravity = true;
				phscene->SetGravity(gravity);
				DSTR << "Gravity ON" << endl;
			}
			break;

		case 's':
			bStep = false;
			bOneStep = true;
			break;

		case 'a':
			bStep = true;
			break;

		case 'f':
			{
				bIK = !bIK;
				DSTR << (bIK ? "IK ON" : "IK OFF") << endl;
				phscene->GetIKEngine()->Enable(bIK);
			}
			break;

		case 'l':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f(+0.2f,  0.0f,  0.0f);
				ikPosCtl1->SetGoal(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetGoal();
					goal += Vec3f(+0.2f,  0.0f,  0.0f);
					ikPosCtl2->SetGoal(goal);
				}
			}
			break;

		case 'j':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f(-0.2f,  0.0f,  0.0f);
				ikPosCtl1->SetGoal(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetGoal();
					goal += Vec3f(-0.2f,  0.0f,  0.0f);
					ikPosCtl2->SetGoal(goal);
				}
			}
			break;

		case 'i':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f, +0.2f,  0.0f);
				ikPosCtl1->SetGoal(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetGoal();
					goal += Vec3f( 0.0f, +0.2f,  0.0f);
					ikPosCtl2->SetGoal(goal);
				}
			}
			break;

		case 'k':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f, -0.2f,  0.0f);
				ikPosCtl1->SetGoal(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetGoal();
					goal += Vec3f( 0.0f, -0.2f,  0.0f);
					ikPosCtl2->SetGoal(goal);
				}
			}
			break;

		case 'u':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f,  0.0f, +0.2f);
				ikPosCtl1->SetGoal(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetGoal();
					goal += Vec3f( 0.0f,  0.0f, +0.2f);
					ikPosCtl2->SetGoal(goal);
				}
			}
			break;

		case 'o':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f,  0.0f, -0.2f);
				ikPosCtl1->SetGoal(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetGoal();
					goal += Vec3f( 0.0f,  0.0f, -0.2f);
					ikPosCtl2->SetGoal(goal);
				}
			}
			break;


		case 'L':
			{
				Vec3d relGoal = ikPosCtl2->GetGoal() - ikPosCtl1->GetGoal();
				relGoal += Vec3f(+0.2f,  0.0f,  0.0f);
				ikPosCtl2->SetGoal(relGoal + ikPosCtl1->GetGoal());
			}
			break;

		case 'J':
			{
				Vec3d relGoal = ikPosCtl2->GetGoal() - ikPosCtl1->GetGoal();
				relGoal += Vec3f(-0.2f,  0.0f,  0.0f);
				ikPosCtl2->SetGoal(relGoal + ikPosCtl1->GetGoal());
			}
			break;

		case 'I':
			{
				Vec3d relGoal = ikPosCtl2->GetGoal() - ikPosCtl1->GetGoal();
				relGoal += Vec3f( 0.0f, +0.2f,  0.0f);
				ikPosCtl2->SetGoal(relGoal + ikPosCtl1->GetGoal());
			}
			break;

		case 'K':
			{
				Vec3d relGoal = ikPosCtl2->GetGoal() - ikPosCtl1->GetGoal();
				relGoal += Vec3f( 0.0f, -0.2f,  0.0f);
				ikPosCtl2->SetGoal(relGoal + ikPosCtl1->GetGoal());
			}
			break;

		case 'U':
			{
				Vec3d relGoal = ikPosCtl2->GetGoal() - ikPosCtl1->GetGoal();
				relGoal += Vec3f( 0.0f,  0.0f, +0.2f);
				ikPosCtl2->SetGoal(relGoal + ikPosCtl1->GetGoal());
			}
			break;

		case 'O':
			{
				Vec3d relGoal = ikPosCtl2->GetGoal() - ikPosCtl1->GetGoal();
				relGoal += Vec3f( 0.0f,  0.0f, -0.2f);
				ikPosCtl2->SetGoal(relGoal + ikPosCtl1->GetGoal());
			}
			break;


		case '1': case '2': case '3': case '4': case '5':
			Reset(key - '1');
			break;

			// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
			// 
			// 独自のキーファンクションはここで定義します．
			// 
			// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

		default:
			break;
	}
}
