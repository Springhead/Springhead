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
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <windows.h> 
using namespace Spr;

//Variables to save the sensor data
std::string expFileName;    //file name
std::ofstream expFile;	   //file object

///Gets the complete executable file path
std::string getExePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}

///Opens/Creates a CSV file to save the sensor values 
void openCSVFile (){
	std::string execPath = getExePath();
	expFileName = execPath + "\\sensordata.csv";
	expFile.open(expFileName);
}

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
	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);

	UTRef<HISpidarGIf> spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidarGDesc("SpidarG6X3F"));
	spg->Calibration();

	//The port is 3 because the sensor is connected to the port 3 on the Spidar's AD Converter
	DVAdIf* ad = hiSdk->RentVirtualDevice(DVAdIf::GetIfInfoStatic(), "", 3)->Cast();
	
	//Open the csv file
	openCSVFile();

	int t = 0;
	float volts = 0.0f;
	while(!_kbhit()){
		t += 1;
		spg->Update(0.001f);
#if 0
		Vec3f spgpos = spg->GetPosition();
		std::cout << std::setprecision(2) << spgpos << std::endl;
		Vec3f f(0.0, 0.0, 0.0);
		if(spgpos.y < -0.015){
			f[1] = (float) (-(spgpos.y -  -0.015) * 1000);
		}
		spg->SetForce(f, Vec3f());
#else if
		for(size_t i=0; i<spg->NMotor(); ++i){
			std::cout << " " << std::setprecision(2) << spg->GetMotor(i)->GetLength();
		}
		std::cout << std::endl;

		if (ad) {

			volts = ad->Voltage();

			DPF("Volts: %f\n", volts);
			expFile << volts << std::endl;  //Use Excel to plot the data. 
		}
#endif
	}
	expFile.close();
	_getch();
/*
	DRKeyMouseWin32If* wif = DCAST(DRKeyMouseWin32If, sdk->FindRealDevice("KeyMouseWin32"));
	wif->Update();	
	UTRef<HIMouse6DIf> mouse6D = DCAST(HIMouse6DIf, sdk->CreateHumanInterface("HIMouse6D", NULL));
	while(1){
		if (mouse6D->GetKeyMouse()->GetKeyState('Q') & DVKeyMouseIf::PRESS) return 0;
		for(int i=0; i<200; ++i){
			if (mouse6D->GetKeyMouse()->GetKeyState(i) & DVKeyMouseIf::PRESS){
				std::cout << i << " '"<< (char)i << "' " << std::endl;
			}
		}
	}
*/
}
