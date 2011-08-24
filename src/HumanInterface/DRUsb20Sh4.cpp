/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUsb20Sh4.cpp: DRUsb20Sh4 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <HumanInterface/DRUsb20Sh4.h>
#include <HumanInterface/DREzUSB.h>

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#include <sstream>

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif


using namespace std;

namespace Spr {;

DRUsb20Sh4::DRUsb20Sh4(const DRUsb20Sh4Desc& d):DRUsb20Simple(d){
	adVoltPerDigit = 24.0f/4096.0f;
}

bool DRUsb20Sh4::Init(){
	//	初期化（チャンネルに合うUSBデバイスを見つけて、D/A カウンタ PIOを登録する。）
	bool rv = DRUsb20Simple::Init();
	//	初期化に成功していたら、A/Dを登録する。
	if (rv) for(int i = 0; i < 8; i++) AddChildObject((DBG_NEW Ad(this, i))->Cast());
	stringstream ss;
	ss << "Cyberse USB2.0 SH4 #" << channel;
	SetName(ss.str().c_str());
	return rv;
}

void DRUsb20Sh4::Update(){
	UsbUpdate();
}

//--------------------------------------------------------------------------------
//	USBでの通信
#define PACKET_SIZE			512
#define OP_ALL_DATA_UPDATE	0x01
#define OP_BASE_ANALYZER	0x0e


void DRUsb20Sh4::UsbUpdate(){
#ifdef _WIN32
	UCHAR outBuffer[PACKET_SIZE];
	UCHAR inBuffer[PACKET_SIZE];

	int cur = 0;	//	パケット先頭からの位置
	outBuffer[cur++] = OP_ALL_DATA_UPDATE;
	outBuffer[cur++] = OP_BASE_ANALYZER;
	for(int i=0; i < 8; i++){
		outBuffer[cur++] = daOut[i] & 0xFF;
		outBuffer[cur++] = daOut[i] >> 8;
	}
	UsbSend(outBuffer);
	UsbRecv(inBuffer);

	cur = 16;	//	パケット先頭からの位置，D/Aの設定値は読み飛ばす．
	//	カウンタの値の読み出し
	for(int i=0; i<8; ++i){
		DWORD ctIn = inBuffer[cur++];
		ctIn |= (DWORD)(inBuffer[cur++]) << 8;
		//	カウンタは16bitしかないので，前回の値から32bitに拡張する．
		DWORD ct = -count[i];
		DWORD last = ct&0xFFFF;
		int diff = (int)ctIn - (int)last;
		if (diff > 0){
			if (diff > 0x8000){
				diff = int(ctIn-0x10000) - (int)last;
			}
		}else{
			if (diff < -0x8000){
				diff = (int)(ctIn+0x10000) - (int)last;
			}
		}
		ct += diff;
		count[i] = -(long)ct;
	}
	//	A/Dの読み出し
	for(int i=0; i<8; ++i){
		WORD ad = inBuffer[cur++];
		ad |= (WORD)(inBuffer[cur++]) << 8;
		adIn[i] = ad;
	}
	//	PIOの読み出し
	UCHAR pio[2];
	pio[0] = inBuffer[cur++];
	pio[1] = inBuffer[cur++];
//	DSTR << "PIO:";
	for(int i=0; i<16; ++i){
		pioLevel[i] = (pio[i/8] & (0x01 << (i%8))) ? 1 : 0;
//		DSTR << pioLevel[i];
	}
//	DSTR << " " << RotarySwitch();
//	DSTR << std::endl;

	//	infoってなに？
	DWORD info;
	info = inBuffer[cur++];
	info |= (DWORD)(inBuffer[cur++]) << 8;
	info |= (DWORD)(inBuffer[cur++]) << 16;
	info |= (DWORD)(inBuffer[cur++]) << 24;
//	DSTR << "info:" << info << std::endl;
#endif
}

void DRUsb20Sh4::UsbSend(unsigned char* outBuffer){
#ifdef _WIN32
	if (hSpidar == INVALID_HANDLE_VALUE) return;
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 1;
	WORD outPacketSize = PACKET_SIZE;
	int nBytes = 0;
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		outBuffer,
		outPacketSize,
		(unsigned long *)&nBytes, NULL);
#endif
}

void DRUsb20Sh4::UsbRecv(unsigned char* inBuffer){
#ifdef _WIN32
	if (hSpidar == INVALID_HANDLE_VALUE) return;
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 2;
	WORD inPacketSize = PACKET_SIZE;
	int nBytes = 0;
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_READ,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		inBuffer,
		inPacketSize,
		(unsigned long *)&nBytes,
		NULL);
#endif
}
void DRUsb20Sh4::Reset(){
	DWORD pipeNum = 2;
	if (hSpidar != INVALID_HANDLE_VALUE){
		OVERLAPPED ov;
		memset(&ov, 0, sizeof(ov));
		DeviceIoControl(hSpidar,
			IOCTL_Ezusb_ABORTPIPE,
			&pipeNum, sizeof(pipeNum), NULL, 0, NULL, &ov);
		DeviceIoControl(hSpidar,
			IOCTL_Ezusb_RESETPIPE,
			&pipeNum, sizeof(pipeNum), NULL, 0, NULL, &ov);	
	}
}
/*bool DRUsb20Sh4::FindDevice(int ch){
	for(int i=0; i < 0x100; ++i){
		hSpidar = UsbOpen(i);
		if (hSpidar == INVALID_HANDLE_VALUE) return false;
		if (UsbVidPid(hSpidar) != 0x0CEC0205){	//	SH版アンプのVIDとPID
			UsbClose(hSpidar);
			continue;
		}		
		Reset();
		Update();
		if (ch == RotarySwitch()) return true;
		UsbClose(hSpidar);
	}
	return INVALID_HANDLE_VALUE;
}*/

} //namespace Spr

