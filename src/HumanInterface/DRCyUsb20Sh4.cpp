/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRCyUsb20Sh4.cpp: DRCyUsb20Sh4 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <HumanInterface/DRCyUsb20Sh4.h>

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")
#include "cyioctl.hpp"
#endif

#include <sstream>

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif


using namespace std;

namespace Spr {;

static GUID CYUSBDRV_GUID = {0xae18aa60, 0x7f6a, 0x11d4, 0x97, 0xdd, 0x0, 0x1, 0x2, 0x29, 0xb9, 0x59};
const int PACKET_SIZE = 512;

DRCyUsb20Sh4::DRCyUsb20Sh4(const DRCyUsb20Sh4Desc& d):DRUsb20Sh4(d), sendBuf(NULL), recvBuf(NULL){
	SetupBuffer();
}
DRCyUsb20Sh4::~DRCyUsb20Sh4(){
	delete [] sendBuf;
	delete [] recvBuf;
	if (hSpidar != INVALID_HANDLE_VALUE) CloseHandle(hSpidar);
}

void* DRCyUsb20Sh4::UsbOpen(int id){
#ifdef _WIN32
	SP_DEVINFO_DATA devInfoData; 
	SP_DEVICE_INTERFACE_DATA  devInterfaceData; 
	PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData; 

	HANDLE hDevice = INVALID_HANDLE_VALUE;
	ULONG requiredLength = 0; 
	int deviceNumber = id;  // Can be other values if more than 1 device connected to driver 

	HDEVINFO hwDeviceInfo = SetupDiGetClassDevs ( (LPGUID) &CYUSBDRV_GUID, 
	                                              NULL, 
	                                              NULL, 
	                                              DIGCF_PRESENT|DIGCF_INTERFACEDEVICE); 

	if (hwDeviceInfo != INVALID_HANDLE_VALUE){ 
		devInterfaceData.cbSize = sizeof(devInterfaceData); 
		if (SetupDiEnumDeviceInterfaces ( hwDeviceInfo, 0, (LPGUID) &CYUSBDRV_GUID, 
			deviceNumber, &devInterfaceData)){ 
			SetupDiGetInterfaceDeviceDetail ( hwDeviceInfo, &devInterfaceData, NULL, 0, 
	                                     &requiredLength, NULL); 
			ULONG predictedLength = requiredLength; 

			functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA) malloc (predictedLength); 
			functionClassDeviceData->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA); 

			devInfoData.cbSize = sizeof(devInfoData); 

			if (SetupDiGetInterfaceDeviceDetail (hwDeviceInfo, 
	                                        &devInterfaceData, 
	                                        functionClassDeviceData, 
	                                        predictedLength, 
	                                        &requiredLength, 
	                                        &devInfoData)) { 

				hDevice = CreateFile (functionClassDeviceData->DevicePath, 
	                         GENERIC_WRITE | GENERIC_READ, 
	                         FILE_SHARE_WRITE | FILE_SHARE_READ, 
	                         NULL, 
	                         OPEN_EXISTING, 
	                         /*FILE_FLAG_OVERLAPPED*/ 0, 
	                         NULL); 
				free(functionClassDeviceData); 
				SetupDiDestroyDeviceInfoList(hwDeviceInfo); 
				//std::cout << "Device Open" << std::endl;
			}
		}
	}
	return hDevice;
#else
	return 0;
#endif
}
bool DRCyUsb20Sh4::UsbClose(void*& h){
#ifdef _WIN32
	if(h){
/*		DWORD dwBytes = 0;
		UCHAR Address = 0x86;
		DeviceIoControl(h, IOCTL_ADAPT_ABORT_PIPE, &Address, sizeof(UCHAR),	NULL, 0, &dwBytes, NULL);
		Address = 0x02;
		DeviceIoControl(h, IOCTL_ADAPT_ABORT_PIPE, &Address, sizeof(UCHAR),	NULL, 0, &dwBytes, NULL);
*/

//		bool rv = CloseHandle(h) != 0;
//		std::cout << "Device Close" << std::endl;
		h = INVALID_HANDLE_VALUE;
//		return rv;
		//	なぜかCloseに時間がかかるので
	}
	return false;
#endif
	return false;
}



void DRCyUsb20Sh4::Reset(){
	if (hSpidar != INVALID_HANDLE_VALUE){
		DWORD dwBytes = 0;
		DeviceIoControl(hSpidar, IOCTL_ADAPT_RESET_PARENT_PORT, NULL, 0, NULL, 0, &dwBytes, NULL);
		DWORD e = GetLastError();
		if (e) DSTR << "DRCyUsb20Sh4::Reset(): error code = " << e << std::endl;
	}
}
void DRCyUsb20Sh4::SetupBuffer(){
	sendBufLen = sizeof(SINGLE_TRANSFER) + PACKET_SIZE;
	if (!sendBuf) sendBuf = new unsigned char[sendBufLen];
	sendStart = sendBuf + sizeof(SINGLE_TRANSFER);
	SINGLE_TRANSFER* transfer = (SINGLE_TRANSFER*)sendBuf;
	memset(transfer, 0, sizeof(SINGLE_TRANSFER));
	transfer->ucEndpointAddress = 2;
	transfer->BufferOffset = sizeof(SINGLE_TRANSFER);
	transfer->BufferLength = PACKET_SIZE;
	
	recvBufLen = sizeof(SINGLE_TRANSFER) + PACKET_SIZE;
	if (!recvBuf) recvBuf = new unsigned char[recvBufLen];
	recvStart = recvBuf + sizeof(SINGLE_TRANSFER);
	transfer = (SINGLE_TRANSFER*)recvBuf;
	memset(transfer, 0, sizeof(SINGLE_TRANSFER));
	transfer->ucEndpointAddress = 0x86;
	transfer->BufferOffset = sizeof(SINGLE_TRANSFER);
	transfer->BufferLength = PACKET_SIZE;
}

void DRCyUsb20Sh4::UsbSend(unsigned char* outBuffer){
#ifdef _WIN32
	if (hSpidar == INVALID_HANDLE_VALUE) return;
	memcpy(sendStart, outBuffer, PACKET_SIZE);
	DWORD dwReturnBytes=0;
	
	SetLastError(0);
	DeviceIoControl(hSpidar, IOCTL_ADAPT_SEND_NON_EP0_TRANSFER,
		sendBuf, sendBufLen, sendBuf, sendBufLen, &dwReturnBytes, NULL);
	DWORD e = GetLastError();
	if (e) DSTR << "DRCyUsb20Sh4::UsbSend() error:" << e << std::endl;
	std::cout << "Send" << std::endl;
#endif
}

void DRCyUsb20Sh4::UsbRecv(unsigned char* inBuffer){
#ifdef _WIN32
	if (hSpidar == INVALID_HANDLE_VALUE) return;
	DWORD dwReturnBytes=0;
	SetLastError(0);
	DeviceIoControl(hSpidar, IOCTL_ADAPT_SEND_NON_EP0_TRANSFER,
		recvBuf, recvBufLen, recvBuf, recvBufLen, &dwReturnBytes, NULL);
	DWORD e = GetLastError();
	if (e) DSTR << "DRCyUsb20Sh4::UsbRecv() error:" << e << std::endl;	
	memcpy(inBuffer, recvStart, PACKET_SIZE);
	//std::cout << "Recv" << std::endl;
#endif
}

unsigned DRCyUsb20Sh4::UsbVidPid(void* h){
	return 0x0CEC0205;	//	本当はUSBディスクリプタを確認しないといけない。
	/*
#ifdef _WIN32
	union  {
		struct   {
			UCHAR Recipient:5;
			UCHAR  Type:2;
			UCHAR Direction:1;

		} bmRequest;
		UCHAR bmReq;
	};
	bmRequest.Recipient = 0; //  Device
	bmRequest.Type      = 2; //  Vendor
	bmRequest.Direction = 1; // IN command (from  Device to Host)
	int bufLen = 0x100;
	int iXmitBufSize = sizeof(SINGLE_TRANSFER) + bufLen; //  The size of the two-part structure
	UCHAR *pXmitBuf = new UCHAR[iXmitBufSize];           //  Allocate the memory
	ZeroMemory(pXmitBuf, iXmitBufSize);
	PSINGLE_TRANSFER pTransfer = (PSINGLE_TRANSFER)pXmitBuf; //  The SINGLE_TRANSFER comes first
	pTransfer->SetupPacket.bmRequest = bmReq;
	pTransfer->SetupPacket.bRequest = ReqCode;
	pTransfer->SetupPacket.wValue = Value;
	pTransfer->SetupPacket.wIndex = Index;
	pTransfer->SetupPacket.wLength = bufLen;
	pTransfer->SetupPacket.ulTimeOut = TimeOut / 1000;
	pTransfer->ucEndpointAddress = 0x00;     //  Control pipe

pTransfer->IsoPacketLength = 0;

pTransfer->BufferOffset = sizeof (SINGLE_TRANSFER);

pTransfer->BufferLength = bufLen;

DWORD dwReturnBytes;

 

DeviceIoControl (hDevice, IOCTL_ADAPT_SEND_EP0_CONTROL_TRANSFER,

             pXmitBuf,  iXmitBufSize,

             pXmitBuf,  iXmitBufSize,

             &dwReturnBytes,  NULL);

 

// Copy data into  buf

UCHAR *ptr = pXmitBuf + sizeof (SINGLE_TRANSFER);

memcpy(buf, ptr, dwReturnBytes);



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
	*/
}
	
} //namespace Spr

