/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWLDHapticSample.h"
#include <iostream>
#include <sstream>
#include <Framework/FWInteractScene.h>

using namespace std;

FWLDHapticSample::FWLDHapticSample(){
	bStep = true;
	bDrawInfo = false;
}

void FWLDHapticSample::Init(int argc, char* argv[]){
	/// Sdkの初期化，シーンの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.02);	// 刻みの設定

	/// 描画モードの設定
	SetGRAdaptee(TypeGLUT);									// GLUTモードに設定
	GRInit(argc, argv);						// Sdkの作成

	/// 描画Windowの作成，初期化
	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "FWLDHapticSample";					// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();											// ウィンドウの初期化
	InitCameraView();										// カメラビューの初期化

	/// HumanInterfaceの初期化
	InitHumanInterface();

	/// InteractSceneの作成
	FWInteractSceneDesc desc;
	desc.fwScene = GetSdk()->GetScene();					// fwSceneに対するinteractsceneを作る
	desc.iaMode = LOCAL_DYNAMICS;								// humaninterfaceのレンダリングモードの設定
	desc.hdt = 0.001;										// マルチレートの場合の更新[s]
	CreateIAScene(desc);									// interactSceneの作成

	/// 物理シミュレーションする剛体を作成
	BuildScene();
	BuildPointer();

	/// タイマの作成，設定
	int timerId = CreateTimer(FWTimer::MM);
	SetInterval(timerId , 1);
	SetResolution(timerId , 1);
}

void FWLDHapticSample::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
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

#if 1
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
	UTRef<HISpidar4If> spg = GetHISdk()->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidar4Desc("SpidarR",Vec4i(1,2,3,4)));
	UTRef<HISpidar4If> spg2 = GetHISdk()->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	spg2->Init(&HISpidar4Desc("SpidarL",Vec4i(5,6,7,8)));
	AddHI(spg);
	AddHI(spg2);
#endif
}

void FWLDHapticSample::Reset(){
	ReleaseAllTimer();
	GetSdk()->Clear();
	ClearIAScenes();
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.02);	// 刻みの設定
	FWInteractSceneDesc desc;
	desc.fwScene = GetSdk()->GetScene();					// fwSceneに対するinteractsceneを作る
	desc.iaMode = LOCAL_DYNAMICS;								// humaninterfaceのレンダリングモードの設定
	desc.hdt = 0.001;										// マルチレートの場合の更新[s]
	CreateIAScene(desc);									// interactSceneの作成
	BuildScene();
	BuildPointer();
	GetCurrentWin()->SetScene(GetSdk()->GetScene());
	CreateAllTimer();
}

void FWLDHapticSample::TimerFunc(int id){	
	switch(id){
		case 0:{
			GetIAScene()->CallBackHapticLoop();
			break;
			   }
	}
}

void FWLDHapticSample::IdleFunc(){
	/// シミュレーションを進める(interactsceneがある場合はそっちを呼ぶ)
	if(bStep) 	GetIAScene()->Step();
	else if (bOneStep){
			GetIAScene()->Step();
			bOneStep = false;
	}
	PostRedisplay();
}

void FWLDHapticSample::Display(){
	/// 描画モードの設定
	GetSdk()->SetDebugMode(true);
	GRDebugRenderIf* render = GetCurrentWin()->render->Cast();
	render->SetRenderMode(true, false);
	render->EnableRenderAxis(bDrawInfo);
	render->EnableRenderForce(bDrawInfo);
	render->EnableRenderContact(bDrawInfo);

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

	glutSwapBuffers();
}

void FWLDHapticSample::BuildScene(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;
	CDBoxDesc bd;

	/// 床(物理法則に従わない，運動が変化しない)
	{
		/// 剛体(soFloor)の作成
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		/// 形状(shapeFloor)の作成
		bd.boxsize = Vec3f(50, 10, 50);
		CDShapeIf* shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd);
		/// 剛体に形状を付加する
		soFloor->AddShape(shapeFloor);
		soFloor->GetShape(0)->SetVibration(5,80,100);
		soFloor->SetFramePosition(Vec3d(0, -10, 0));
	}

	/// 箱(物理法則に従う，運動が変化)
	{
		/// 剛体(soBox)の作成
		desc.mass = 0.05;
		desc.inertia = 0.333 * Matrix3d::Unit();
		PHSolidIf* soBox = phscene->CreateSolid(desc);
		/// 形状(shapeBox)の作成
		bd.boxsize = Vec3f(4,4,4);
		CDShapeIf* shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd);
		/// 剛体に形状を付加
		soBox->AddShape(shapeBox);
		soBox->GetShape(0)->SetStaticFriction(2.0);
		soBox->GetShape(0)->SetDynamicFriction(1.0);
		soBox->GetShape(0)->SetVibration(5,80,300);
		soBox->SetFramePosition(Vec3d(0, 10, 0));
	}
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
			idesc.springK = 10;//0.8						// haptic renderingのバネ係数
			idesc.damperD = 0.01;//0.01					// haptic renderingのダンパ係数
			idesc.posScale = 300;					// soPointerの可動域の設定(～倍)
			idesc.localRange = 1.0;					// LocalDynamicsを使う場合の近傍範囲
			if(i==0) idesc.defaultPosition =Posed(1,0,0,0,5,0,0);	// 初期位置の設定
			if(i==1) idesc.defaultPosition =Posed(1,0,0,0,-5,0,0);
			GetIAScene()->CreateIAPointer(idesc);	// interactpointerの作成
		}
	}
}

void FWLDHapticSample::Keyboard(int key, int x, int y){
	BeginKeyboard();
	switch (key) {
		case  27: //ESC
		case 'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			bDrawInfo = !bDrawInfo;
			break;
		case 's':
			bStep = false;
			bOneStep = true;
			DSTR << "Stepwise Execution" << endl;
			break;
		case 'a':
			bStep = true;
			DSTR << "Play" << endl;
			break;
		case 'c':
			{
				ReleaseAllTimer();
				for(int i = 0; i < GetIAScene()->NIAPointers(); i++){
					GetIAScene()->GetIAPointer(i)->Calibration();
				}
				CreateAllTimer();
			}
			break;
		case 'f':
			{
				static bool bf = false;
				bf = !bf;
				for(int i = 0; i < GetIAScene()->NIAPointers(); i++){
					GetIAScene()->GetIAPointer(i)->EnableForce(bf);
					if(!bf){
						HIBaseIf* hib = GetIAScene()->GetIAPointer(i)->GetHI();
						if(DCAST(HIForceInterface6DIf, hib)){
							HIForceInterface6DIf* hif = hib->Cast();
							hif->SetForce(Vec3d(), Vec3d());
						}
						if(DCAST(HIForceInterface3DIf, hib)){
							HIForceInterface3DIf* hif = hib->Cast();
							hif->SetForce(Vec3d());
						}
					}
				}
				if(bf){
					DSTR << "Enable Force Feedback" << std::endl;
				}else{
					DSTR << "Disable Force Feedback" << std::endl;
				}
			}
			break;
		case 'o':
			{
				static bool bv = false;
				bv = !bv;
				for(int i = 0; i < GetIAScene()->NIAPointers(); i++){
					GetIAScene()->GetIAPointer(i)->EnableVibration(bv);
				}
				if(bv){
					DSTR << "Enable Vibration Feedback" << std::endl;
				}else{
					DSTR << "Disable Vibration Feedback" << std::endl;
				}
			}
			break;
		case 'j':
			{
					GetIAScene()->SetHMode(PENALTY);
					DSTR << "PENALTY MODE" << std::endl;
					break;
			}
		case 'k':
			{
					GetIAScene()->SetHMode(PROXY);
					DSTR << "PROXY MODE" << std::endl;
					break;
			}
		case 'l':
			{
					GetIAScene()->SetHMode(PROXYSIMULATION);
					DSTR << "PROXYSIMULATION MODE" << std::endl;
					break;
			}
		default:
			break;
	}
	EndKeyboard();
}
