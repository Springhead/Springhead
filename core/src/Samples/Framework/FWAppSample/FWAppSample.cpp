#include "FWAppSample.h"
#include "SampleModel.h"

#define ESC 27

FWAppSample::FWAppSample(){
	bDrawInfo = false;
}

void FWAppSample::Init(int argc, char* argv[]){
	CreateSdk();											// Sdkの作成
	GetSdk()->CreateScene();								// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.05);	// シミュレーションの刻み時間を設定

	SetGRAdaptee(TypeGLUT);									// CGをOpenGL(GLUT)で描画指定
	GRInit(argc, argv);										// GLUTの初期化

	FWWinDesc windowDesc;					// GLのウィンドウディスクリプタ
	windowDesc.title = "Springhead2";		// ウィンドウのタイトル
	CreateWin(windowDesc);					// ウィンドウの作成とシーンの割り当て
	InitCameraView();						// カメラビューの初期化

	CreateObjects();		// 剛体を作成
	CreateTimer();			// タイマーの生成
}

void FWAppSample::TimerFunc(int id){	
	GetSdk()->Step();
	PostRedisplay();
}

void FWAppSample::InitCameraView(){
	//	Affinef型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	GetCurrentWin()->GetTrackball()->SetAffine(issView);
}

void FWAppSample::CreateObjects(){
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

void FWAppSample::Display(){
	// 各種、情報を表示するかどうか
	FWSceneIf* fwScene = GetSdk()->GetScene();
	fwScene->EnableRenderAxis(bDrawInfo);		// 座標軸の表示
	fwScene->EnableRenderForce(bDrawInfo);		// 拘束力の表示
	fwScene->EnableRenderContact(bDrawInfo);	// 接触状態の表示

	// シーンの描画
	GetSdk()->SetDebugMode(true);
	GetSdk()->GetRender()->SetViewMatrix(GetCurrentWin()->GetTrackball()->GetAffine().inv());
	GetSdk()->Draw();
	GetSdk()->GetRender()->SwapBuffers();
}

void FWAppSample::Reset(){
	GetSdk()->GetScene()->GetPHScene()->Clear();
	CreateObjects();
}

void FWAppSample::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			FWApp::Clear();
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
