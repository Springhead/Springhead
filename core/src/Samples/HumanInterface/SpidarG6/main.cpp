/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/**
	SPIDARG6の接続確認のためのプログラム
	グリップ位置の表示と柔らかい床が提示される
*/

#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIDRUsb.h>
#include <HumanInterface/SprHIKeyMouse.h>
#include <Foundation/SprUTQPTimer.h>
#include <iomanip>
using namespace Spr;

#ifdef _WIN32
#include <windows.h>
#endif

int __cdecl main(){
	// 力覚インタフェースとの接続設定
	UTRef<HISdkIf> hiSdk = HISdkIf::CreateSdk();

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
	//	UART Motor Driver
	DRUARTMotorDriverDesc umDesc;
	hiSdk->AddRealDevice(DRUARTMotorDriverIf::GetIfInfoStatic(), &umDesc);
	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);

	//UTRef<HISpidar4If> spg = hiSdk->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	//spg->Init(&HISpidar4DDesc());

	UTRef<HISpidarGIf> spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidarGDesc("SpidarG6T1"));
	spg->Calibration();

	int t = 0;
	int lastTime;
#ifdef _WIN32
	lastTime = timeGetTime();
#endif
	while(!_kbhit()){
		t += 1;
		if (t >= 1000) {
			t = 0;
#ifdef _WIN32
			int time = timeGetTime();
#endif
			int diff = time - lastTime;
			DPF("\r\nDuration: %d,  Freq: %f\n", diff, 1000.0 / diff * 1000);
			lastTime = time;
		}
		//DPF("t=%d", t);
		spg->Update(0.001f);
#if 1	//	Virtual floor
		Vec3f spgpos = spg->GetPosition();
//		std::cout << std::setprecision(2) << spgpos << std::endl;
		Vec3f f(0.0, 0.0, 0.0);
		if(spgpos.y < -0.015){
			f.y = (float) (-(spgpos.y -  -0.015) * 1000);
		}
		spg->SetForce(f, Vec3f());
#else	//	print the position and the orientation of the grip
		for(size_t i=0; i<spg->NMotor(); ++i){
			std::cout << " " << std::setprecision(2) << spg->GetMotor(i)->GetLength();
		}
		std::cout << std::endl;
#endif
	}
#if 0	//	test for KeyMouseWin32
	DRKeyMouseWin32If* wif = hiSdk->FindRealDevice("KeyMouseWin32")->Cast();
	wif->Update();	
	DVKeyMouseIf* keyMouse = wif->Rent(DVKeyMouseIf::GetIfInfoStatic(), NULL, 0)->Cast();
	while(1){
		if (keyMouse->GetKeyState('Q') & DVKeySt::PRESSED) return 0;
		for(int i=0; i<200; ++i){
			if (keyMouse->GetKeyState(i) & DVKeySt::PRESSED){
				std::cout << i << " '"<< (char)i << "' " << std::endl;
			}
		}
	}
#endif
}
