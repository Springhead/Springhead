/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUsb20Simple.cpp: DRUsb20Simple クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#pragma hdrstop
#include <HumanInterface/DRUsb20Simple.h>
#include <HumanInterface/DREzUSB.h>

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#include <sstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr {;

DRUsb20Simple::DRUsb20Simple(const DRUsb20SimpleDesc& d){
	channel = d.channel;

	hSpidar = NULL;
	for(int i=0; i<8; i++){
		daOut[i]		= 0;
		adIn[i]			= 0;
		count[i]		= 0;
		countOffset[i]	= 0;
		pioLevel[i]		= 0;
	}
}

DRUsb20Simple::~DRUsb20Simple(){
}

bool DRUsb20Simple::Init(){
	stringstream ss;
	ss << "Cyverse USB2.0 Simple #" << channel;
	SetName(ss.str().c_str());

	int i, chMax = 0x100;
	for(i = 0; i < chMax; ++i){
		hSpidar = UsbOpen(i);
		if (!hSpidar)
			continue;
		if (UsbVidPid(hSpidar) != GetVidPid()){
			UsbClose(hSpidar);
			continue;
		}		
		Reset();
		Update();
		if (channel == -1 || channel == ReadRotarySwitch())
			break;
		UsbClose(hSpidar);
	}
	if(i == chMax)
		return false;

	for(i = 0; i < 8; i++){
		AddChildObject((DBG_NEW Da(this, i))->Cast());
		AddChildObject((DBG_NEW Counter(this, i))->Cast());
		AddChildObject((DBG_NEW Pio(this, i))->Cast());
	}
	return true;
}

/*bool DRUsb20Simple::InitAny(){
	for(int i=0; i < 0x100; ++i){
		hSpidar = UsbOpen(i);
		if (hSpidar == INVALID_HANDLE_VALUE) return false;
		return true;
//		if (UsbVidPid(hSpidar) == 0x0CEC0205){	//	SH版アンプのVIDとPID
//			UsbClose(hSpidar);
//			return true;
//		}
	}
	return false;
}*/

void* DRUsb20Simple::UsbOpen(int id){
#ifdef _WIN32
	std::ostringstream name;
	name << "\\\\.\\Ezusb-" << id;
	HANDLE rv = CreateFile(name.str().c_str(),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
	if(rv == INVALID_HANDLE_VALUE)
		return 0;
	return rv;
#else
	return 0;
#endif
}
bool DRUsb20Simple::UsbClose(void*& h){
#ifdef _WIN32
	bool rv = CloseHandle(h) != 0;
	h = NULL;
	return rv;
#endif
	return false;
}

/*void DRUsb20Simple::Register(HISdkIf* intf){
	HISdk* sdk = intf->Cast();
	for(int i=0; i<8; i++){
		sdk->RegisterVirtualDevice((new DVDa(this, i))->Cast());
		sdk->RegisterVirtualDevice((new DVCounter(this, i))->Cast());
		sdk->RegisterVirtualDevice((new DVPio(this, i))->Cast());
	}
}*/

void DRUsb20Simple::WriteVoltage(int ch, float v){
	assert(0 <= ch && ch < 8);
	const float DigitPerVolt[]={	//	DA指令値/出力電圧 アンプごとに異なるので計測値をアンプに書いておき，読み出す必要がある．
		415.0f,
		415.0f,
		415.0f,
		415.0f,		
		415.0f,
		415.0f,
		415.0f,
		415.0f,
	};
	daOut[ch] = (int)(v * DigitPerVolt[ch]);
}
void DRUsb20Simple::WriteDigit(int ch, int v){
	daOut[ch] = v;
}
void DRUsb20Simple::WriteCount(int ch, long c){
	countOffset[ch] = - (count[ch] - c); 
}
long DRUsb20Simple::ReadCount(int ch){
	return count[ch] + countOffset[ch];
}
void DRUsb20Simple::WritePio(int ch, bool level){
	pioLevel[ch] = level ? 1 : 0;
}
bool DRUsb20Simple::ReadPio(int ch) {
	if(pioLevel[ch]) return true;
	else return false;
}
int DRUsb20Simple::ReadRotarySwitch(){
	int sw=0;
	for(int i=0; i<4; ++i){
		sw |= pioLevel[i+4] << i;
	}
	return sw;
}

void DRUsb20Simple::Update(){
	UsbDaSet();
	UsbCounterGet();
}

void DRUsb20Simple::Reset(){
	UsbReset();
	UsbCounterClear();
}

void DRUsb20Simple::UsbReset(){
#ifdef _WIN32
	BULK_TRANSFER_CONTROL bulkControl;
	int nBytes = 0;
	int i;
	UCHAR outBuffer[32];
	for(i = 0; i < 32; i++) outBuffer[i] = 0;

	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	WORD outPacketSize = 32;

	// エンコーダ相順設定
	outBuffer[0] = 0xA2;
	outBuffer[1] = 0x1A;
	outBuffer[2] = 0xFF;	//	各チャンネル(各ビットが対応)の正逆転の設定

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);
#endif
}

void DRUsb20Simple::UsbCounterClear(){
#ifdef _WIN32
	// エンコーダカウンタクリア
	UCHAR outBuffer[32];
	outBuffer[0] = 0xA2;
	outBuffer[1] = 0x19;
	outBuffer[2] = 0xFF;
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 1;
	WORD outPacketSize = 32;
	int nBytes = 0;
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);
#endif
}

//----------------------------------------------------------------------
// D/A出力

void DRUsb20Simple::UsbDaSet(){
#ifdef _WIN32
	WORD  outPacketSize;
	int nBytes = 0;

	BULK_TRANSFER_CONTROL bulkControl; //バルク、インタラプト転送用コントロール構造体

	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 1;
	outPacketSize = 32;

	// DAコンバータセット
	int cur = 0;	//	パケット先頭からの位置
	UCHAR outBufferDA[32];
	memset(outBufferDA, 0, sizeof(outBufferDA));
	outBufferDA[cur++] = 0x52;	//	D/Aセット
	for(int i=0; i < 8; i++){
		outBufferDA[cur++] = daOut[i] & 0xFF;
		outBufferDA[cur++] = daOut[i] >> 8;
	}
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBufferDA[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);
#endif
}

void DRUsb20Simple::UsbCounterGet(){
#ifdef _WIN32
	// ホストからデバイス側へ
	// PipeNum:1 type:blk Endpoint:2 OUT MaxPktSize:0x200(512bytes)
	BULK_TRANSFER_CONTROL bulkControl;
	bulkControl.pipeNum = 1;
	WORD outPacketSize = 32;
	int nBytes = 0;

	// エンコーダ値のホールドを指令
	UCHAR outBuffer[32];
	outBuffer[0] = 0x51;

	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_WRITE,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&outBuffer[0],
		outPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	// デバイス側からホストへ
	// PipeNum:2 type:blk Endpoint:6 IN MaxPktSize:0x200(512bytes)
	bulkControl.pipeNum = 2;
	WORD inPacketSize = 32;
	UCHAR inBuffer[32];
	int piBuffer[8];
	
	//	エンコーダ値の読み出し
	DeviceIoControl (hSpidar,
		IOCTL_EZUSB_BULK_READ,
		&bulkControl,
		sizeof(BULK_TRANSFER_CONTROL),
		&inBuffer[0],
		inPacketSize,
		(unsigned long *)&nBytes,
		NULL);

	UCHAR Data;
	bool flag;
	for(int i = 0; i < 8; i++){
		piBuffer[i] = 0;
		Data = inBuffer[i * 3 + 2];
		if(Data & 0x80)
			flag = true;
		else
			flag = false;
		piBuffer[i] += Data << 16;
		Data = inBuffer[i * 3 + 1];
		piBuffer[i] += Data << 8;
		Data = inBuffer[i * 3 + 0];
		piBuffer[i] += Data;
		if(flag)
			piBuffer[i] |= 0xFF000000;
	}

	for(int i=0;i<8;i++){
		pioLevel[i] = (inBuffer[24] & (1 << i)) ? 1 : 0;
	}

	for(int i=0;i<8;i++){
		count[i] = piBuffer[i];
	}
#endif
}

unsigned DRUsb20Simple::UsbVidPid(void* h){
#ifdef _WIN32
	USB_DEVICE_DESCRIPTOR desc;
	DWORD nBytes=0;
	if (DeviceIoControl(h,
	       IOCTL_Ezusb_GET_DEVICE_DESCRIPTOR,
	       NULL,
	       0,
	       &desc,
	       sizeof(desc),
	       &nBytes,
		   NULL)){
		return (desc.idVendor << 16) | desc.idProduct;
	}
#endif
	return 0;
}

} //namespace Spr

