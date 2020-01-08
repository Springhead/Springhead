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
#include <HumanInterface/DRUartMotorDriver.h>

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
		AddChildObject((DBG_NEW Pio(this, i))->Cast());
	}
	return true;
}
void DRUARTMotorDriver::EnumBoards() {

}

void DRUARTMotorDriver::WriteVoltage(int ch, float v){
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
void DRUARTMotorDriver::WriteDigit(int ch, int v){
	daOut[ch] = v;
}
void DRUARTMotorDriver::WriteCount(int ch, long c){
	countOffset[ch] = - (count[ch] - c); 
}
long DRUARTMotorDriver::ReadCount(int ch){
	return count[ch] + countOffset[ch];
}
void DRUARTMotorDriver::WritePio(int ch, bool level){
	pioLevel[ch] = level ? 1 : 0;
}
bool DRUARTMotorDriver::ReadPio(int ch) {
	if(pioLevel[ch]) return true;
	else return false;
}
int DRUARTMotorDriver::ReadRotarySwitch(){
	int sw=0;
	for(int i=0; i<4; ++i){
		sw |= pioLevel[i+4] << i;
	}
	return sw;
}

void DRUARTMotorDriver::Update(){
	UsbDaSet();
	UsbCounterGet();
}

void DRUARTMotorDriver::Reset(){
	UsbReset();
	UsbCounterClear();
}

void DRUARTMotorDriver::UsbReset(){
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

void DRUARTMotorDriver::UsbCounterClear(){
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

void DRUARTMotorDriver::UsbDaSet(){
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

void DRUARTMotorDriver::UsbCounterGet(){
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

unsigned DRUARTMotorDriver::UsbVidPid(void* h){
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

