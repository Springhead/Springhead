#include "VTraj.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

#include <GL/glut.h>

#include "Physics/PHConstraint.h"
#include "Physics/PHSpatial.h"
#include "Physics/PHIK.h"
#include "Physics/PHScene.h"
#include "Physics/PHBallJoint.h"
#include "Physics/PHEngine.h"

#define ESC 27

VTraj app;

VTraj::VTraj(){
	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= true;

	gravity		= Vec3d(0.0, -9.8, 0.0);

	states		= ObjectStatesIf::Create();
}

void VTraj::TimerFunc(int id){
	switch(id){
		case 0:
			Step();
			GetGRAdaptee()->PostRedisplay();
			break;
	}
}

void VTraj::Init(int argc, char* argv[]){
	SetGRAdaptee(TypeGLUT);
	GetGRAdaptee()->Init(argc, argv);						// Sdkの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成

	// GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.01);

	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "Virtual Trajectory Tracking Sample";// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();
	InitCameraView();										// カメラビューの初期化

	BuildScene(2);

	CreateTimer(FWTimer::GLUT);
}

void VTraj::Reset(int sceneNum){
	GetSdk()->GetScene()->GetPHScene()->Clear();

	bGravity	= true;
	bDebug		= false;
	bStep		= true;
	bOneStep	= false;
	bIK			= true;

	BuildScene(sceneNum);
}

void VTraj::InitCameraView(){
	istringstream iss(
		"((0.9996 0.00141267 -0.0282299 -0.0937266)"
		"(1.59067e-012 0.99875 0.0499792 0.165937)"
		"(0.0282652 -0.0499592 0.998351 3.31465)"
		"(	   0      0      0      1))"
		);
	iss >> cameraInfo.view;
}

void VTraj::BuildScene(int sceneNum){
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
		descBallJoint.spring =   0.01f;
		descBallJoint.damper =   400.0f;
		PHJointIf* jo1 = phScene->CreateJoint(so1, so2, descBallJoint);
		///// IKノード
		descIKBall.spring = 0.1f;
		descIKBall.damper = 0.001f;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);
		ikNode1->AddChildObject(jo1->Cast());

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
		descBallJoint.spring =   0.01f;
		descBallJoint.damper =   400.0f;
		PHJointIf* jo2 = phScene->CreateJoint(so2, so3, descBallJoint);
		///// IKノード
		descIKBall.spring = 0.1f;
		descIKBall.damper = 0.001f;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);
		ikNode2->AddChildObject(jo2->Cast());

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		ikPosCtl1->AddChildObject(so3->Cast());
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
		descIKBall.spring = 100000.0f;
		descIKBall.damper =   2000.0f;
		descIKBall.bias = 1.8;
		PHIKNodeIf* ikNode1 = phScene->CreateIKNode(descIKBall);
		ikNode1->AddChildObject(jo1->Cast());

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
		descIKBall.spring = 90000.0f;
		descIKBall.damper =  2000.0f;
		descIKBall.bias = 1.6;
		PHIKNodeIf* ikNode2 = phScene->CreateIKNode(descIKBall);
		ikNode2->AddChildObject(jo2->Cast());

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
		descIKBall.spring = 80000.0f;
		descIKBall.damper =  2000.0f;
		descIKBall.bias = 1.4;
		PHIKNodeIf* ikNode3 = phScene->CreateIKNode(descIKBall);
		ikNode3->AddChildObject(jo3->Cast());

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
		descIKBall.spring = 70000.0f;
		descIKBall.damper =  2000.0f;
		descIKBall.bias = 1.2;
		PHIKNodeIf* ikNode4 = phScene->CreateIKNode(descIKBall);
		ikNode4->AddChildObject(jo4->Cast());

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
		descIKBall.spring = 60000.0f;
		descIKBall.damper =  2000.0f;
		descIKBall.bias = 1.0;
		PHIKNodeIf* ikNode5 = phScene->CreateIKNode(descIKBall);
		ikNode5->AddChildObject(jo5->Cast());

		/// -- IK制御点
		///// 制御点の作成
		descIKPos.pos = Vec3d(0.0, 0.5, 0.0);
		ikPosCtl1 = phScene->CreateIKControlPoint(descIKPos)->Cast();
		ikPosCtl1->AddChildObject(so6->Cast());
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
	/// IKの設定
	phScene->GetIKEngine()->SetNumIter(15);
	phScene->GetIKEngine()->Enable(bIK);
	/// その他の設定
	phScene->SetContactMode(PHSceneDesc::MODE_NONE);
	phScene->SetGravity(gravity);
	phScene->SetTimeStep(0.01);
	phScene->SetNumIteration(15);
}

void VTraj::Step(){
	if(bStep) {
		OneStep();
	} else if (bOneStep) {
		OneStep();
		bOneStep = false;
	}

	glutPostRedisplay();
}

void VTraj::OneStep(){
	PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();

	CalcOffsetForce();

	phScene->Step();

	soPosCtl1->SetFramePosition(ikPosCtl1->GetTargetPosition());
	if (soPosCtl2) {
		soPosCtl2->SetFramePosition(ikPosCtl2->GetTargetPosition());
	}
}

void VTraj::CalcOffsetForce(){
	PHSceneIf* phScene = GetSdk()->GetScene()->GetPHScene();
	PHIKEngineIf* ikEngine = phScene->GetIKEngine();

	states->SaveState(phScene);
	
	phScene->GetIKEngine()->Enable(true);

	DCAST(PHEngine,phScene->GetIKEngine())->Step();

	typedef std::vector<BJOffset> BJOffsets;
	BJOffsets bjOffsets;
	
	for (size_t i=0; i < DCAST(PHIKEngine,ikEngine)->nodes.size(); ++i) {
		PHIKNodeIf* ikNode = DCAST(PHIKEngine,ikEngine)->nodes[i];
		if (ikNode) {
			PHIKBallJointIf* ikBJ = ikNode->Cast();
			if (ikBJ) {
				PHIKBallJoint* bj = ikBJ->Cast();
				Vec3d dT = Vec3d();
				for (int j=0; j < bj->ndof; ++j) {
					dT += (bj->dTheta[j]/bj->bias) * bj->e[j];
				}

				PHBallJoint* jo = DCAST(PHBallJoint,bj->joint);

				bjOffsets.resize(bjOffsets.size()+1);
				bjOffsets.back().jo = jo->Cast();
				bjOffsets.back().vel = dT / phScene->GetTimeStep();
				bjOffsets.back().goal = jo->GetTargetPosition();

				if (bjOffsets.back().vel.norm() > Rad(360)) {
					bjOffsets.back().vel = bjOffsets.back().vel.unit() * Rad(360);
				}
			}
		}
	}

	states->LoadState(phScene);

	for (size_t i=0; i<bjOffsets.size(); ++i) {
		bjOffsets[i].damper = bjOffsets[i].jo->GetDamper();
		bjOffsets[i].jo->SetDamper(DBL_MAX);
		bjOffsets[i].jo->SetTargetVelocity(bjOffsets[i].vel);
	}

	phScene->GetIKEngine()->Enable(false);

	phScene->Step();

	for (size_t i=0; i<bjOffsets.size(); ++i) {
		bjOffsets[i].offset = bjOffsets[i].jo->GetOffsetForce();
	}

	states->LoadState(phScene);

	for (size_t i=0; i<bjOffsets.size(); ++i) {
		bjOffsets[i].jo->SetDamper(bjOffsets[i].damper);
		bjOffsets[i].jo->SetTargetPosition(bjOffsets[i].goal);
		bjOffsets[i].jo->SetTargetVelocity(bjOffsets[i].vel);
		bjOffsets[i].jo->SetOffsetForce(bjOffsets[i].offset);
	}
}

void VTraj::Display(){
	// 描画モードの設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDebug);
	render->EnableRenderForce(bDebug);
	render->EnableRenderContact(bDebug);

	// カメラ座標の指定
	GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
	}

	// 描画の実行
	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();
	glutSwapBuffers();
}

void VTraj::Keyboard(int key, int x, int y){
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
				Vec3d goal = ikPosCtl1->GetTargetPosition();
				goal += Vec3f(+0.2f,  0.0f,  0.0f);
				ikPosCtl1->SetTargetPosition(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetTargetPosition();
					goal += Vec3f(+0.2f,  0.0f,  0.0f);
					ikPosCtl2->SetTargetPosition(goal);
				}
			}
			break;

		case 'j':
			{
				Vec3d goal = ikPosCtl1->GetTargetPosition();
				goal += Vec3f(-0.2f,  0.0f,  0.0f);
				ikPosCtl1->SetTargetPosition(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetTargetPosition();
					goal += Vec3f(-0.2f,  0.0f,  0.0f);
					ikPosCtl2->SetTargetPosition(goal);
				}
			}
			break;

		case 'i':
			{
				Vec3d goal = ikPosCtl1->GetTargetPosition();
				goal += Vec3f( 0.0f, +0.2f,  0.0f);
				ikPosCtl1->SetTargetPosition(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetTargetPosition();
					goal += Vec3f( 0.0f, +0.2f,  0.0f);
					ikPosCtl2->SetTargetPosition(goal);
				}
			}
			break;

		case 'k':
			{
				Vec3d goal = ikPosCtl1->GetTargetPosition();
				goal += Vec3f( 0.0f, -0.2f,  0.0f);
				ikPosCtl1->SetTargetPosition(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetTargetPosition();
					goal += Vec3f( 0.0f, -0.2f,  0.0f);
					ikPosCtl2->SetTargetPosition(goal);
				}
			}
			break;

		case 'u':
			{
				Vec3d goal = ikPosCtl1->GetTargetPosition();
				goal += Vec3f( 0.0f,  0.0f, +0.2f);
				ikPosCtl1->SetTargetPosition(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetTargetPosition();
					goal += Vec3f( 0.0f,  0.0f, +0.2f);
					ikPosCtl2->SetTargetPosition(goal);
				}
			}
			break;

		case 'o':
			{
				Vec3d goal = ikPosCtl1->GetTargetPosition();
				goal += Vec3f( 0.0f,  0.0f, -0.2f);
				ikPosCtl1->SetTargetPosition(goal);
				if (soPosCtl2) {
					Vec3d goal = ikPosCtl2->GetTargetPosition();
					goal += Vec3f( 0.0f,  0.0f, -0.2f);
					ikPosCtl2->SetTargetPosition(goal);
				}
			}
			break;


		case 'L':
			{
				Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
				relGoal += Vec3f(+0.2f,  0.0f,  0.0f);
				ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
			}
			break;

		case 'J':
			{
				Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
				relGoal += Vec3f(-0.2f,  0.0f,  0.0f);
				ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
			}
			break;

		case 'I':
			{
				Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
				relGoal += Vec3f( 0.0f, +0.2f,  0.0f);
				ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
			}
			break;

		case 'K':
			{
				Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
				relGoal += Vec3f( 0.0f, -0.2f,  0.0f);
				ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
			}
			break;

		case 'U':
			{
				Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
				relGoal += Vec3f( 0.0f,  0.0f, +0.2f);
				ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
			}
			break;

		case 'O':
			{
				Vec3d relGoal = ikPosCtl2->GetTargetPosition() - ikPosCtl1->GetTargetPosition();
				relGoal += Vec3f( 0.0f,  0.0f, -0.2f);
				ikPosCtl2->SetTargetPosition(relGoal + ikPosCtl1->GetTargetPosition());
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
