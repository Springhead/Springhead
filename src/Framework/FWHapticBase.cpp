/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWHapticBase.h>
#include <conio.h>
#include <HumanInterface/SprHIDRUsb.h>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;

FWHapticBase::FWHapticBase(){}

void FWHapticBase::InitHumanInterface(const IfInfo* info = HISpidarGIf::GetIfInfoStatic(), void* desc = &HISpidarGDesc("SpidarG6X3R")){
	DSTR << "Init Human Interface" << std::endl;
	HISdkIf* hisdk = HISdkIf::CreateSdk();
	DRUsb20SimpleDesc usbSimpleDesc;
	hisdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i = 0; i < 10; i++){
		usb20Sh4Desc.number = i;
		hisdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	hisdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	
	hisdk->Init();
	hisdk->Print(DSTR);
	hapticDevice = hisdk->CreateHumanInterface(info)->Cast();
	hapticDevice->Init(desc);
	DSTR << "Init Human Interface Done" << std::endl;
}

HIForceInterface6DIf* FWHapticBase::GetHumanInterface(){
	return hapticDevice;
}

void FWHapticBase::UpdateHumanInterface(){
	hapticDevice->Update((float)timeStep);
	hapticPointer.SetFramePosition(hapticDevice->GetPosition() * posScale);
	hapticPointer.SetOrientation(hapticDevice->GetOrientation());
	hapticPointer.SetVelocity(hapticDevice->GetVelocity() * posScale);
	hapticPointer.SetAngularVelocity(hapticDevice->GetAngularVelocity());
}

void FWHapticBase::HapticRendering(){}
void FWHapticBase::Keyboard(int key, int x, int y){
}

}