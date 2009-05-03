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
	bIK			= false;

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

	// Sceneの設定
	phScene->SetContactMode(PHSceneDesc::MODE_NONE);
	phScene->GetIKEngine()->SetNumIter(10);
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
			}
			break;

		case 'j':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f(-0.2f,  0.0f,  0.0f);
				ikPosCtl1->SetGoal(goal);
			}
			break;

		case 'i':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f, +0.2f,  0.0f);
				ikPosCtl1->SetGoal(goal);
			}
			break;

		case 'k':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f, -0.2f,  0.0f);
				ikPosCtl1->SetGoal(goal);
			}
			break;

		case 'u':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f,  0.0f, +0.2f);
				ikPosCtl1->SetGoal(goal);
			}
			break;

		case 'o':
			{
				Vec3d goal = ikPosCtl1->GetGoal();
				goal += Vec3f( 0.0f,  0.0f, -0.2f);
				ikPosCtl1->SetGoal(goal);

			}
			break;

		case '1':
			Reset(0);
			break;

		case '2':
			Reset(1);
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
