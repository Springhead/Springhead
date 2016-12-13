/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "App.h"	

using namespace Spr;

App::App(){
	hdt = 0.001f;
}

void App::InitInterface(){
	HISdkIf* hiSdk = GetSdk()->GetHISdk();

	// x86
	DRUsb20SimpleDesc usbSimpleDesc;
	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i< 10; ++i){
		usb20Sh4Desc.channel = i;
		hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	// x64
	DRCyUsb20Sh4Desc cyDesc;
	for(int i=0; i<10; ++i){
		cyDesc.channel = i;
		hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
	}
	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);

	spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidarGDesc("SpidarG6X3R"));
	spg->Calibration();
}

void App::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);							// アプリケーションの初期化
	BuildScene();
	InitInterface();									// インタフェースの初期化
	


	UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 力覚スレッド用のマルチメディアタイマを作成
	timer->SetResolution(1);								// 分解能(ms)
	timer->SetInterval(unsigned int(hdt * 1000));			// 刻み(ms)h
	hapticTimerID = timer->GetID();							// 力覚スレッドのタイマIDの取得
	timer->Start();											// タイマスタート
}
void App::BuildScene() {
	PHSdkIf* phSdk = GetSdk()->GetPHSdk();				// シェイプ作成のためにPHSdkへのポインタをとってくる
	phScene = GetSdk()->GetScene()->GetPHScene();		// 剛体作成のためにPHSceneへのポインタをとってくる

	Vec3d pos = Vec3d(0, 0, 1.21825);					// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
	GetSdk()->SetDebugMode(true);						// デバック表示の有効化

														// 床を作成
	CDBoxDesc bd;
	bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
	PHSolidIf* floor = phScene->CreateSolid();
	floor->AddShape(phSdk->CreateShape(bd));
	floor->SetFramePosition(Vec3d(0, -1.0, 0.0));
	floor->SetDynamical(false);
	floor->SetName("floor");


	// 球体（力覚ポインタ）の作成
	pointer = phScene->CreateSolid();	// 力覚ポインタの作成
	CDSphereDesc cd;
	cd.radius = 0.1f;
	pointer->AddShape(phSdk->CreateShape(cd));	// シェイプの追加
	Posed defaultPose;
	defaultPose.Pos() = Vec3d(0.0, -0.35, 0.0);
	pointer->SetPose(defaultPose);		// 力覚ポインタ初期姿勢の設定
	pointer->SetDynamical(false);
	pointer->SetName("pointer");
}


void App::TimerFunc(int id){
	if(hapticTimerID == id){
		RenderHaptic();
		PostRedisplay();
	}
}
void App::RenderHaptic() {
	static Affinef af = Affinef::Trn(0, 0.5f, 0);
	//af.PosX() += 0.001;
	pointer->SetPose(af);
}

void App::Keyboard(int key, int x, int y){
	// 各スレッドの共有メモリのアクセス違反回避のために全てのタイマをとめる
	float dr = 0.01f;
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Stop();
	switch(key){
		case 0x1b:
		case 'q':
			// アプリケーションの終了
			exit(0);
		break;
		case 'c':
			{
				// インタフェースのキャリブレーション
				spg->Calibration();
			}
			break;
		default:
			break;
	}
	// 全てのタイマを始動
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Start();
}
