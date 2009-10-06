#include "FWGLUISample.h"
#include "SampleModel.h"
#include <iostream>
#include <sstream>
#include <GL/glut.h>
#include <GL/glui.h>

#define ESC 27

FWGLUISample::FWGLUISample(){
	bDrawInfo = false;
}

void FWGLUISample::Init(int argc, char* argv[]){
	SetGRAdaptee(TypeGLUI);
	GetGRAdaptee()->Init(argc, argv);						// Sdkの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.05);

	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "FWAppSample";						// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();
	
	FWGLUIDesc uiDesc;
	{
		uiDesc.fromLeft = 510;	uiDesc.fromTop	=  30;
	}
	// UIを作る場合の処理（if(glui)と同義）
	if(glui = ((FWGLUI*)GetGRAdaptee())->CreateGUI(GetWin(0)->GetID(), uiDesc)){
		DesignGUI();
	}

	InitCameraView();										// カメラビューの初期化
	BuildObject();											// 剛体を作成
	CreateTimer(FWTimer::GLUT)->SetInterval(10);							// タイマを作成，周期10ms
}

void FWGLUISample::InitCameraView(){
	//	Affinef 型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWGLUISample::BuildObject(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;
	CDBoxDesc bd;

	// 床(物理法則に従わない，運動が変化しない)
	{
		// 剛体(soFloor)の作成
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		// 形状(shapeFloor)の作成
		bd.boxsize = Vec3f(50, 10, 50);
		CDShapeIf* shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加する
		soFloor->AddShape(shapeFloor);
		soFloor->SetFramePosition(Vec3d(0, -5, 0));
	}

	// 箱(物理法則に従う，運動が変化)
	{
		// 剛体(soBox)の作成
		desc.mass = 0.5;
		desc.inertia *= 0.033;
		PHSolidIf* soBox = phscene->CreateSolid(desc);
		// 形状(shapeBox)の作成
		bd.boxsize = Vec3f(2,2,2);
		CDShapeIf* shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加
		soBox->AddShape(shapeBox);
		soBox->SetFramePosition(Vec3d(0, 10, 0));
	}
}

void FWGLUISample::TimerFunc(int id){
	GetSdk()->Step();
	GetGRAdaptee()->PostRedisplay();
}

void FWGLUISample::Display(){
	// 描画モードの設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDrawInfo);
	render->EnableRenderForce(bDrawInfo);
	render->EnableRenderContact(bDrawInfo);

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
	std::cout << NWin() << std::endl;
	GetSdk()->Draw();
	glutSwapBuffers();
	glutPostRedisplay();
}

void FWGLUISample::Reset(){
	GetSdk()->GetScene()->GetPHScene()->Clear();
	BuildObject();
}

void FWGLUISample::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			bDrawInfo = !bDrawInfo;
			break;
		case '1':
			DSTR << "box" << std::endl;
			CreateBox(GetSdk());
			break;
		case '2':
			DSTR << "sphere" << std::endl;
			CreateSphere(GetSdk());
			break;
		case '3':
			DSTR << "capsule" << std::endl;
			CreateCapsule(GetSdk());
			break;
		case '4':
			DSTR << "roundcone" << std::endl;
			CreateRoundCone(GetSdk());
			break;
		default:
			break;
	}
}


void FWGLUISample::DesignGUI(){
	panel = glui->add_panel("Sample", true);
	button1 = glui->add_button_to_panel(panel, "Create Box", 1, GLUI_CB(CallButton1));
}

void FWGLUISample::CallButton1(int control){
	((FWGLUISample*)FWApp::instance)->Button1(control);
}

void FWGLUISample::Button1(int control){
	Keyboard('1', 0, 0);
}

