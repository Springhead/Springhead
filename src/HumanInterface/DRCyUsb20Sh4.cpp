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

#include <HumanInterface/DRCyUsb20Sh4.h>

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")
#endif

#include <sstream>

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif


using namespace std;

namespace Spr {;

static GUID CYUSBDRV_GUID = {0xae18aa60, 0x7f6a, 0x11d4, 0x97, 0xdd, 0x0, 0x1, 0x2, 0x29, 0xb9, 0x59};

DRCyUsb20Sh4::DRCyUsb20Sh4(const DRCyUsb20Sh4Desc& d):DRUsb20Sh4(d){

}

void* DRCyUsb20Sh4::UsbOpen(int id){
#ifdef _WIN32
	SP_DEVINFO_DATA devInfoData; 
	SP_DEVICE_INTERFACE_DATA  devInterfaceData; 
	PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData; 

	HANDLE hDevice = NULL;
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
	                         FILE_FLAG_OVERLAPPED, 
	                         NULL); 
				free(functionClassDeviceData); 
				SetupDiDestroyDeviceInfoList(hwDeviceInfo); 
			}
		}
	}
	if(hDevice == INVALID_HANDLE_VALUE) return 0;
	return hDevice;
#else
	return 0;
#endif
}

	
} //namespace Spr

