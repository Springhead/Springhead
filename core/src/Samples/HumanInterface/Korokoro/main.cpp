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
#include<HumanInterface/SprHIKorokoro.h>
#include<HumanInterface/HIKorokoro.h>
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
	DRUARTMotorDriverIf* uartMotorDriver = hiSdk->AddRealDevice(DRUARTMotorDriverIf::GetIfInfoStatic(), &umDesc)->Cast();
	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());

	hiSdk->Print(std::cout);


	UTRef<HIKorokoroIf> korokoro = hiSdk->CreateHumanInterface(HIKorokoroIf::GetIfInfoStatic())->Cast();
	if (uartMotorDriver->NMotor() >= 3) {	//	UART driver
		korokoro->Init(&HIKorokoroDesc());
		DSTR << "Init Korokoro" << std::endl;
	}
	
	hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);

	enum class Mode {
		NONE,
		FORCE_UP,
		FORCE_DOWN,
		RIGHT,
		LEFT,
		GRIP,
		DEBUG
	} mode = Mode::NONE;
	

	std::string help = "ESC/Q: Quit,  R: Right wheel,  L: Left wheel,  W:force up,  S:force down.";
	std::cout << std::endl << help << std::endl;
	while(1){
		if (_kbhit()) {
			switch (_getch()) {
			case '0x1b':
			case 'q':
			case 'Q':
				std::cout << "Quit." << std::endl;
				goto next;
			case 'r':
			case 'R':
				mode = Mode::RIGHT;
				std::cout << "right wheel" << std::endl;
				break;
			case 'l':
			case 'L':
				mode = Mode::LEFT;
				std::cout << "left wheel" << std::endl;
				break;
			case 'g':
			case 'G':
				mode = Mode::GRIP;
				std::cout << "Grip" << std::endl;
				break;
			case 'd':
			case 'D':
				mode = Mode::DEBUG;
				std::cout << "Debug mode." << std::endl;
				break;
			default:
				mode = Mode::NONE;
				std::cout << help << std::endl;
				break;
			}
		}
		
		//DPF("t=%d", t);
		korokoro->Update(0.001f);
		Vec3f korokoroForce=korokoro->handForce;
		//手にかかる力のほうのSetForce?
		//Vec3f handForce(10.0, 10.0, 10.0);
		//korokoro->SetForce(handForce, Vec3f());
		if (mode == Mode::RIGHT) {			
			if (_kbhit()) {
				if (_getch() == 'w') {
					korokoroForce.x += 10.0f;
				}
				if (_getch() == 's') {
					korokoroForce.x -= 10.0f;
				}
			}
		}
		else if (mode == Mode::LEFT) {
			if (_kbhit()) {
				if (_getch() == 'w') {
					korokoroForce.y += 10.0f;
				}
				if (_getch() == 's') {
					korokoroForce.y -= 10.0f;
				}
			}
		}
		else if (mode == Mode::GRIP) {
			if (_kbhit()) {
				if (_getch() == 'w') {
					korokoroForce.z += 10.0f;
				}
				if (_getch() == 's') {
					korokoroForce.z -= 10.0f;
				}
			}
		}
		/*else if (mode == Mode::DEBUG) {
			korokoro->SetForce(Vec3d(10000, 0, 0), Vec3d(10000, 0, 0));
		}*/

			
			//Vec3f handForce(10.0, 10.0, 10.0);
			
			
			//手にかかる力のほうのSetForce
			//korokoro->SetForce(handForce, Vec3f());
			
			
		
		
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
