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

/*
	This program implements a class to catch 
	very simple Spidar gestures. In this case 
	the program catches the sweeping action 
	in the postive and negative side of the Z axis. 
*/

#include <conio.h>
#include <Springhead.h>
#include <windows.h>
#include <fstream>
#include "SpidarGestures.h"

#define MINSLIDEVEL 2.0

using namespace Spr;

// 各種変数   Various Variables
UTRef<HISdkIf> hiSdk;
UTRef<HISpidarGIf> spg;
float dt = 0.001f;		// 更新周期     //Updating cycle
double K = 5000;		// バネ係数     //Spring coefficient
double D = 5;			// ダンパ係数   //Damper coeficient
double fy = -0.009;		// 力覚提示位置 //Haptic position
//used for debugging std::ofstream myfile; //debug file 
double time;   //time ticks
SpidarGestures sg;  //class that implements the gestures


// マルチメディアタイマのコールバック関数
// Multimedia Timer Callback Function
void SPR_CDECL CallBackLoop(int id, void* arg){
	spg->Update(dt);

	double px  = spg->GetPosition().x;	//ポインタの位置           Pointer position
	double py  = spg->GetPosition().y;	//ポインタの位置           Pointer position
	double pz  = spg->GetPosition().z;	//ポインタの位置           Pointer position
	double pvx = spg->GetVelocity().x;	//ポインタの速度           Pointer speed 
	double pvy = spg->GetVelocity().y;	//ポインタの速度           Pointer speed 
	double pvz = spg->GetVelocity().z;	//ポインタの速度           Pointer speed 

	double dy  = py-fy;					//床とポインタの位置の差   Difference between the pointer and the floor
	double dv  = pvy - 0;				//床とポインタの速度差     Speed difference between the pointer and the floor

	Vec3d f = Vec3d();
#if 1
	if(dy < 0)	f.y= -K * dy - D * dv;		//力の計算             Force calculation*/
#else
	if(dy< 0)	f.y = 5;
#endif
	spg->SetForce(f, Vec3d());

	//Returns 1 if the event happend in the positive axis
	//Returns -1 if the event happend in the negative axis
	//And 0 if the event is not triggered. 
	int r = sg.slideEventTrigger(pz, pvz, time);
	if (r == 1)
		std::cout << "Positive Event Triggered.. " << std::endl;
	else if (r == -1)
		std::cout << "Negative Event Triggered.. " << std::endl;

	// Used for debuging myfile << time << "," << pz << "," << pvz << "," << r << std::endl;

	time = time + dt;
}

void __cdecl main(){
	// 力覚インタフェースとの接続設定             Spidar connection settings
	hiSdk = HISdkIf::CreateSdk();
	// win32
	/*DRUsb20SimpleDesc usbSimpleDesc;
	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.channel = i;
		hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}*/

	//Used for debuging myfile.open("c:\\tmp\\debug_filex.txt"); //debug file 

	// win64
	DRCyUsb20Sh4Desc cyDesc;
	for(int i=0; i<10; ++i){
		cyDesc.channel = i;
		hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
	}
	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	//hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);

	spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidarGDesc("SpidarG6X3R"));
	spg->Calibration();

	// マルチメディアタイマーの設定              Multimedia timer settings
	UTTimerIf* timer = UTTimerIf::Create();				
	timer->SetMode(UTTimerIf::MULTIMEDIA);		// タイマのモード設定 Multimedia timer mode (MULTIMEDIA or THREAD)  
	timer->SetCallback(CallBackLoop, NULL);		// 呼びだす関数       Invoke Function
	timer->SetResolution(1);					// 分解能(ms)         Resolution
	timer->SetInterval((unsigned int)(dt*1000));// 刻み(ms)			  Nick
	timer->Start();								// タイマスタート     Timer start
	
	std::cout << "Start the application. " << std::endl;
	std::cout << "Press any key to exit." << std::endl;
	while(!_kbhit()){}
	timer->Stop();
	//Used for debugging myfile.close();
	exit(0);
}

