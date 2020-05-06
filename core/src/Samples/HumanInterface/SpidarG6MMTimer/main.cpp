/*
 *  Copyright (c) 2003-2011, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*	SPIDARG6の出力を確認するためのプログラム
	マルチメディアタイマの割り込みを使うことで，
	約1msの更新が可能．
*/

#include <conio.h>
#include <Springhead.h>
#include <windows.h>

using namespace Spr;

// 各種変数
UTRef<HISdkIf> hiSdk;
UTRef<HISpidarGIf> spg;
float dt = 0.001f;		// 更新周期
double K = 5000;		// バネ係数
double D = 5;			// ダンパ係数
double fy = -0.009;		// 力覚提示位置

// マルチメディアタイマのコールバック関数
void SPR_CDECL CallBackLoop(int id, void* arg){
	spg->Update(dt);

	double py  = spg->GetPosition().y;	//ポインタの位置
	double pvy = spg->GetVelocity().y;	//ポインタの速度
	double dy  = py-fy;					//床とポインタの位置の差
	double dv  = pvy - 0;				//床とポインタの速度差

	Vec3d f = Vec3d();
#if 1
	if(dy < 0)	f.y= -K * dy - D * dv;		//力の計算
#else	
	if(dy< 0)	f.y = 5;
#endif
	spg->SetForce(f, Vec3d());

	//std::cout << spg->GetPosition() << std::endl;
}

void __cdecl main(){
	// 力覚インタフェースとの接続設定
	hiSdk = HISdkIf::CreateSdk();
	// win32
	DRUsb20SimpleDesc usbSimpleDesc;
	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.channel = i;
		hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	// win64
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

	// マルチメディアタイマーの設定
	UTTimerIf* timer = UTTimerIf::Create();				
	timer->SetMode(UTTimerIf::MULTIMEDIA);		// タイマのモード設定(MULTIMEDIA or THREAD)
	timer->SetCallback(CallBackLoop, NULL);		// 呼びだす関数
	timer->SetResolution(1);					// 分解能(ms)
	timer->SetInterval((unsigned int)(dt*1000));// 刻み(ms)
	timer->Start();								// タイマスタート
	
	std::cout << "Start the application. " << std::endl;
	std::cout << "Press any key to exit." << std::endl;
	while(!_kbhit()){}
	timer->Stop();
	exit(0);
}
