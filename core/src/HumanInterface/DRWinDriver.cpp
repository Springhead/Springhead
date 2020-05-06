/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "DRWinDriver.h"
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

#ifdef _WIN32

namespace Spr {
//---------------------------------------------------------------
//	DRLoadWindowsDriver

DRLoadWindowsDriver::DRLoadWindowsDriver(){
	hSCMan = NULL;
	hService = NULL;
}
DRLoadWindowsDriver::~DRLoadWindowsDriver(){
	Cleanup();
}
void DRLoadWindowsDriver::Path(const char* p){
	path.Path(p);
	drvName = path.Main();
}
void DRLoadWindowsDriver::File(const char* f){
	if (path.Search(f)){
		Path(path.Path().c_str());
	}
}

void DRLoadWindowsDriver::Init(){
	hSCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCMan){
		DSTR << "Can't connect to service control manager" << std::endl;
	}
}
void DRLoadWindowsDriver::Cleanup(){
	if (hSCMan){
		CloseServiceHandle(hSCMan);
		hSCMan = NULL;
	}
}
void DRLoadWindowsDriver::Install(){
	Init();
	// add to service control manager's database
	hService = CreateService(hSCMan, drvName.c_str(), 
      drvName.c_str(), SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
      SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, path.Path().c_str(), 
      NULL, NULL, NULL, NULL, NULL);
	if (!hService){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot create service from \'" << path.Path().c_str();
		 DSTR << "\' . Error#=" << dwStatus << std::endl;
	}
}
void DRLoadWindowsDriver::Open(){
	if (hService) return;
	Init();
	hService = OpenService(hSCMan, drvName.c_str(), SERVICE_ALL_ACCESS);
}
void DRLoadWindowsDriver::Start(){
	Open();
	if (!StartService(hService, 0, NULL)){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot start service \'" << drvName.c_str()
			 << "\' . Error#=" << dwStatus << std::endl;
	}
}
void DRLoadWindowsDriver::Stop(){
	Open();
	SERVICE_STATUS serviceStatus;
	BOOL rv = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
	if (!rv){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot stop service \'" << drvName.c_str()
			 << "\' . Error#=" << dwStatus << std::endl;
	}
}
void DRLoadWindowsDriver::Remove(){
	Open();
	if (!DeleteService(hService)){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot remove service \'" << drvName.c_str()
			 << "\' . Error#=" << dwStatus << std::endl;
	}
}
bool DRLoadWindowsDriver::IsOpen(){
	return hService != NULL;
}
bool DRLoadWindowsDriver::IsStart(){
	if (!IsOpen() ) Open();
	  // handle of service
	SERVICE_STATUS st;
	if (QueryServiceStatus(hService, &st)){
		return st.dwCurrentState == SERVICE_RUNNING;
	}
	return false;
}

}	//	namespace Spr

#endif
