#include "PositionDebug.h"
#include <iostream>
#include <sstream>
#include <Framework/FWInteractScene.h>

#define SPIDAR 1;
using namespace std;



FWLDHapticSample::FWLDHapticSample(){
}
void FWLDHapticSample::Display(){
	/// 描画モードの設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDrawInfo);
	render->EnableRenderForce(bDrawInfo);
	render->EnableRenderContact(bDrawInfo);
	render->EnableRenderWorldAxis(true);
	render->EnableGrid(true,0.0,1.0);

	/// カメラ座標の指定
	GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
	}

	/// 描画の実行
	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();
	
	DisplayContactPlane();
	DisplayLineToNearestPoint();

	glutSwapBuffers();
}
void FWLDHapticSample::Init(int argc, char* argv[]){
	/// Sdkの初期化，シーンの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.02);	// 刻みの設定
	//GetSdk()->GetScene()->GetPHScene()->SetGravity(Vec3d());
	/// 描画モードの設定
	SetGRAdaptee(TypeGLUT);									// GLUTモードに設定
	GetGRAdaptee()->Init(argc, argv);						// Sdkの作成

	/// 描画Windowの作成，初期化
	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "Springhead2";						// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();											// ウィンドウの初期化
	InitCameraView();										// カメラビューの初期化

	/// HumanInterfaceの初期化
	InitHumanInterface();

	/// InteractSceneの作成
	FWInteractSceneDesc desc;
	desc.fwScene = GetSdk()->GetScene();					// fwSceneに対するinteractsceneを作る
	desc.mode = LOCAL_DYNAMICS;								// humaninterfaceのレンダリングモードの設定
	desc.hdt = 0.001;										// マルチレートの場合の更新[s]
	CreateINScene(desc);									// interactSceneの作成

	/// 物理シミュレーションする剛体を作成
	BuildPointer();

	/// タイマの作成，設定
	UTMMTimer* mtimer = CreateMMTimerFunc();				// タイマを作成
	mtimer->Resolution(1);									// 分解能[ms]
	mtimer->Interval(1);									// 呼びだし感覚[ms]
	mtimer->Set(CallBackHapticLoop, NULL);					// コールバックする関数
	mtimer->Create();										// コールバック開始
}
void FWLDHapticSample::InitCameraView(){
	//std::istringstream issView(
	//	"((0.9996 0.0107463 -0.0261432 -0.389004)"
	//	"(-6.55577e-010 0.924909 0.380188 5.65711)"
	//	"(0.0282657 -0.380037 0.92454 13.7569)"
	//	"(     0      0      0      1))"
	//);
	//真上からの視点
	std::istringstream issView(
		"((1.0 0.0 0.0 0.0)"
		"(0.0 0.0 1.0 20.0)"
		"(0.0 -1.0 0.0 0)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWLDHapticSample::InitHumanInterface(){
	/// HISdkの作成
	CreateHISdk();

	DRUsb20SimpleDesc usbSimpleDesc;
	GetHISdk()->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.number = i;
		GetHISdk()->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	GetHISdk()->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	GetHISdk()->Init();
	GetHISdk()->Print(DSTR);

#if SPIDAR

	/// SPIDARG6を2台使う場合
	UTRef<HISpidarGIf> spg[2];
	for(size_t i = 0; i < 2; i++){
		spg[i] = GetHISdk()->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
		if(i == 0) spg[i]->Init(&HISpidarGDesc("SpidarG6X3R"));
		if(i == 1) spg[i]->Init(&HISpidarGDesc("SpidarG6X3L"));
		AddHI(spg[i]);
	}
#else
	/// SPIDAR4Dを使う場合
	UTRef<HISpidar4If> spg[2];
	for(size_t i = 0; i < 2; i++){
		spg[i] = GetHISdk()->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
		if(i == 0) spg[i]->Init(&HISpidar4Desc("SpidarR",Vec4i(1,2,3,4)));
		if(i == 1) spg[i]->Init(&HISpidar4Desc("SpidarL",Vec4i(5,6,7,8)));
		AddHI(spg[i]);
	}
#endif
}
void FWLDHapticSample::BuildPointer(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;

	/// ポインタ
	{	
		for(int i= 0; i < 2; i++){
			PHSolidIf* soPointer = phscene->CreateSolid(desc);
			CDSphereDesc sd;
			sd.radius = 0.5;//1.0;
			CDSphereIf* shapePointer = DCAST(CDSphereIf,  GetSdk()->GetPHSdk()->CreateShape(sd));
			soPointer->AddShape(shapePointer);
			soPointer->SetDynamical(false);
			soPointer->GetShape(0)->SetStaticFriction(1.0);
			soPointer->GetShape(0)->SetDynamicFriction(1.0);
			GetSdk()->GetScene()->GetPHScene()->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			FWInteractPointerDesc idesc;			// interactpointerのディスクリプタ
			idesc.pointerSolid = soPointer;			// soPointerを設定
			idesc.humanInterface = GetHI(i);		// humaninterfaceを設定
			idesc.springK = 10;						// haptic renderingのバネ係数
			idesc.damperD = 0.05;					// haptic renderingのダンパ係数
		#if SPIDAR
			idesc.posScale = 300;					// soPointerの可動域の設定(～倍)
			idesc.forceScale = 1.0;
		#else
			idesc.posScale = 60;					// soPointerの可動域の設定(～倍)
		#endif
			idesc.localRange = 1.0;					// LocalDynamicsを使う場合の近傍範囲
			if(i==0) idesc.defaultPosition = Posed(1,0,0,0,5.0,0.0,0.0); //ポインタの初期位置
			if(i==1) idesc.defaultPosition = Posed(1,0,0,0,-5.0,0.0,0.0);
			GetINScene()->CreateINPointer(idesc);	// interactpointerの作成
		}
	}
}

void FWLDHapticSample::IdleFunc(){
	/// シミュレーションを進める(interactsceneがある場合はそっちを呼ぶ)
	FWAppHaptic::instance->GetINScene()->Step();
	//Balljoint->GetDefomationMode();//naga
	glutPostRedisplay();
}

void FWLDHapticSample::TwoPointerCalib(){
	TMatrixRow<6,3,float> mainMat ;
	TMatrixRow<6,4,float> subMat ;
	TMatrixRow<4,3,float> transMat ;
	Affined TAffine ;

	//mainMat：(x,y,z)を一列とするn行の行列
	for(int i=0; i<6 ; i++){
		for(int j=0; j<3; j++){
			mainMat[i][j] =mainPosition[i][j];
		}
	}
	//subMat：(x,y,z,1)を一列とするn行の行列
	for(int i=0; i<6 ; i++){
		for(int j=0; j<4; j++){
			if(j==3) subMat[i][j] = 1;
			else subMat[i][j] =subPosition[i][j];	
		}
	}
	//DSTR<<"mainMat"<<std::endl<<mainMat<<std::endl;
	//DSTR<<"subMat"<<std::endl<<subMat<<std::endl;

	//疑似逆行列からsubをmainに変換するaffin行列の数値を算出
	TMatrixRow<4,4,float> subMatb = subMat.trans()* subMat;
	transMat = subMatb.inv()* subMat.trans() * mainMat;

	//Affin行列の形に整形
	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++){
			TAffine[i][j]=transMat[j][i];
		}
	}
	TAffine[3][0]=0;
	TAffine[3][1]=0;
	TAffine[3][2]=0;
	TAffine[3][3]=1;
	
	DSTR<<"TAffine"<<std::endl<<TAffine<<std::endl;
	for(int i=0;i<6;i++){
		DSTR<<"mainMat"<<mainMat[i]<<std::endl;
		DSTR<<"subMat"<<subMat[i]<<std::endl;
		Vec3d NewPosition = TAffine*subPosition[i];
		DSTR<<"NewPosition"<<NewPosition<<std::endl;
	}

	Posed tPos;
	tPos.FromAffine(TAffine);
	GetINScene()->GetINPointer(0)->SetDefaultPosition(tPos);
}

void FWLDHapticSample::TwoPointerSet(){
	Vec3d mainPos = GetINScene()->GetINPointer(0)->GetPointerSolid()->GetFramePosition();
	Vec3d subPos = GetINScene()->GetINPointer(1)->GetPointerSolid()->GetFramePosition();

	mainPosition.push_back(mainPos);
	subPosition.push_back(subPos);

	DSTR<<"mainPos"<<mainPosition.size()<<":"<<mainPos<<std::endl;
	DSTR<<"subPos"<<subPosition.size()<<":"<<subPos<<std::endl;

	//for(int i=0;i<4;i++){
	//	mainPosition.push_back(Vec3d(0,0,0));
	//	subPosition.push_back(Vec3d(0,0,0));
	//}

	//mainPosition.push_back(Vec3d(1,0,0));
	//subPosition.push_back(Vec3d(3,0,0));
	//mainPosition.push_back(Vec3d(0,1,0));
	//subPosition.push_back(Vec3d(0,3,0));
	//mainPosition.push_back(Vec3d(0,0,1));
	//subPosition.push_back(Vec3d(0,0,3));
	//mainPosition.push_back(Vec3d(0,2,1));
	//subPosition.push_back(Vec3d(0,6,3));

}

void FWLDHapticSample::Keyboard(int key, int x, int y){
	FWAppHaptic::Keyboard(key , x, y);
	PHSceneIf* phscene=GetSdk()->GetPHSdk()->GetScene(0);

	switch (key) {
		case 'r':
			{
				Reset();
			}
			break;
		case ' ':
			{
				DSTR<<cameraInfo.view<<std::endl;
			}
			break;

		case 'v':
			{
				//真上からの視点
				std::istringstream issView(
					"((1.0 0.0 0.0 0.0)"
					"(0.0 0.0 1.0 20.0)"
					"(0.0 -1.0 0.0 0)"
					"(     0      0      0      1))"
				);
				issView >> cameraInfo.view;
			}
			break;

			
		case 'b':
			{
				//真前からの視点
				std::istringstream issView(
					"((1.0 0.0 0.0 0.0)"
					"(0.0 1.0 0.0 0.0)"
					"(0.0 0.0 1.0 20.0)"
					"(     0      0      0      1))"
				);
				issView >> cameraInfo.view;
			}
			break;

		case 'z':
			{
				TwoPointerSet();
			}

			break;
			
		case 'x':
			{
				TwoPointerCalib();
			}

			break;
		default:
			break;
	}
}
