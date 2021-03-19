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
	HISpidarGDesc desc("SpidarG6X4R");
	for (int i = 0; i < 8; ++i) desc.motors[i].minForce = 0.3;
	spg->Init(&desc);
	//spg->Init(&HISpidarGDesc("SpidarG6X3L"));
	spg->Calibration();

	int t = 0;
	int lastTime;
#ifdef _WIN32
	lastTime = timeGetTime();
#endif

	float length[8];
	for (int i = 0; i < 8; ++i) {
		length[i] = spg->GetMotor(i)->GetLength();
	}
	enum Mode {
		NONE,
		STRING,
		POSE,
		FORCE,
		TENSE,
		DEBUG,
	} mode = NONE;
	std::string help = "ESC/Q: Quit,  f: force feedback,  s: string,  p: pose,  c: calibration.";
	std::cout << std::endl << help << std::endl;
	while(1){
		if (_kbhit()) {
			switch (_getch()) {
			case '0x1b':
			case 'q':
			case 'Q':
				std::cout << "Quit." << std::endl;
				goto next;
			case 'f':
			case 'F':
				mode = FORCE;
				std::cout << "force feedback mode." << std::endl;
				break;
			case 'T':
			case 't':
				mode = TENSE;
				std::cout << "Full tense mode." << std::endl;
				break;
			case 'd':
			case 'D':
				mode = DEBUG;
				std::cout << "Debug mode." << std::endl;
				break;
			case 's':
			case 'S':
				mode = STRING;
				std::cout << "string length mode." << std::endl;
				break;
			case 'p':
			case 'P':
				mode = POSE;
				std::cout << "pose mode." << std::endl;
				break;
			case 'C':
			case 'c':
				spg->Calibration();
				std::cout << "Calibrate." << std::endl;
				break;
			default:
				mode = NONE;
				std::cout << help << std::endl;
				break;
			}
		}
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
		if (mode == FORCE) {
			//	Virtual floor
			Vec3f spgpos = spg->GetPosition();
			//		std::cout << std::setprecision(2) << spgpos << std::endl;
			Vec3f f(0.0, 0.0, 0.0);
			if (spgpos.y < -0.015) {
				f.y = (float)(-(spgpos.y - -0.015) * 1000);
			}
			spg->SetForce(f, Vec3f());
			Vec6f pose;
			pose.sub_vector(0, Vec3f()) = spg->GetPosition();
			pose.sub_vector(3, Vec3f()) = spg->GetOrientation().Rotation();
			for (size_t i = 0; i < pose.size(); ++i) {
				printf(" %7.4f", pose[i]);
			}
			std::cout << std::endl;
		}else if (mode == TENSE) {
			for (int i = 0; i < spg->NMotor(); ++i) {
				spg->GetMotor(i)->SetForce(100.0f);
			}
			Vec6f pose;
			pose.sub_vector(0, Vec3f()) = spg->GetPosition();
			pose.sub_vector(3, Vec3f()) = spg->GetOrientation().Rotation();
			for (size_t i = 0; i < pose.size(); ++i) {
				printf(" %7.4f", pose[i]);
			}
			std::cout << std::endl;
		}
		else if (mode == STRING) {
			//	print string length
			for (size_t i = 0; i < spg->NMotor(); ++i) {
				printf(" %7.4f", spg->GetMotor(i)->GetLength() - length[i]);
			}
			std::cout << std::endl;
		}else if (mode == POSE){
			Vec6f pose;
			pose.sub_vector(0, Vec3f()) = spg->GetPosition();
			pose.sub_vector(3, Vec3f()) = spg->GetOrientation().Rotation();
			for (size_t i = 0; i < pose.size(); ++i) {
				printf(" %7.4f", pose[i]);
			}
			std::cout << std::endl;
		}
		else if (mode == DEBUG) {
			spg->SetForce(Vec3d(10000, 0, 0), Vec3d(10000, 0, 0));
		}
	}
next:;
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
