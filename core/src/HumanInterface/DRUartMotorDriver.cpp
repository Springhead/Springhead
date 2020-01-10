/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUARTMotorDriver.cpp: DRUARTMotorDriver クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <Springhead.h>
#define NMOTOR 1
#define NFORCE 1
#define NTARGET 1
#include <HumanInterface/DRUartMotorDriver.h>
#include <HumanInterface/DRUartMotorDriver/WROOM/main/softRobot/CommandWROOM.h>
#include <HumanInterface/DRUartMotorDriver/WROOM/main/softRobot/Board.h>
#undef NMOTOR
#undef NTARGET
#undef NFORCE

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#include <sstream>

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

#ifdef	__linux__
 #define INVALID_HANDLE_VALUE	NULL
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr {;

DRUARTMotorDriver::DRUARTMotorDriver(const DRUARTMotorDriverDesc& d){
	port = d.port;
	hUART = INVALID_HANDLE_VALUE;
}

DRUARTMotorDriver::~DRUARTMotorDriver(){
	if (hUART) {
		CloseHandle(hUART);
		hUART = NULL;
	}
}

bool DRUARTMotorDriver::Init(){
	std::vector<string> comPorts;
	if (port = -1) {
		HMODULE h;
		char nameBuffer[128 * 100];
		if (((h = GetModuleHandle("kernel32.dll")) != NULL) &&
			(GetProcAddress(h, "QueryDosDeviceA") != NULL) &&
			(QueryDosDevice(NULL, nameBuffer, 65535) != 0)) {
			char* p = nameBuffer;
			while (*p != '\0') {
				if (strncmp(p, "COM", 3) == 0 && p[3] != '\0') {
					comPorts.push_back(std::string(p));
				}
				p += (strlen(p) + 1);
			}
		}
		CloseHandle(h);
	}
	else {
		comPorts.push_back(std::string("COM") + std::to_string(port));
	}
	for (auto comPort : comPorts) {
		hUART = CreateFile(comPort.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,				//ポートの共有方法を指定:オブジェクトは共有しない
			NULL,			//セキュリティ属性:ハンドルを子プロセスへ継承しない
			OPEN_EXISTING,
			0,				//ポートの属性を指定:同期　非同期にしたいときはFILE_FLAG_OVERLAPPED
			NULL);
		EnumBoards();
		if (boards.size() > 0) {
			break;
		}
	}


	//	デバイスの登録
	for(int i = 0; i < 8; i++){
		AddChildObject((DBG_NEW Da(this, i))->Cast());
		AddChildObject((DBG_NEW Counter(this, i))->Cast());
	}
	return true;
}
void DRUARTMotorDriver::EnumBoards() {
	//	TBW by hase
}
void DRUARTMotorDriver::WriteVoltage(int ch, float v) {
	//	TBW by hase
}
void DRUARTMotorDriver::WriteDigit(int ch, int v) {
	//	TBW by hase
}
void DRUARTMotorDriver::WriteCount(int ch, long c) {
	//	TBW by hase
}
long DRUARTMotorDriver::ReadCount(int ch) {
	//	TBW by hase
	return 0;
}
void DRUARTMotorDriver::Update() {
	//	TBW by hase
}
void DRUARTMotorDriver::Reset() {
	//	TBW by hase
}



} //namespace Spr

