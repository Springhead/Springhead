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
	hapticInterface = hisdk->CreateHumanInterface(info)->Cast();
	hapticInterface->Init(desc);
	DSTR << "Init Human Interface Done" << std::endl;
}

void FWHapticBase::ResetHapticProcess(){
	loopCounter = 1;
}

HIForceInterface6DIf* FWHapticBase::GetHumanInterface(){
	return hapticInterface;
}

void FWHapticBase::SetHapticTimeStep(double dt){
	hapticTimeStep = dt;
}

double FWHapticBase::GetHapticTimeStep(){
	return hapticTimeStep;
}

void FWHapticBase::SetPhysicTimeStep(double dt){
	physicTimeStep = dt;
}

double FWHapticBase::GetPhysicTimeStep(){
	return physicTimeStep;
}

void FWHapticBase::UpdateHumanInterface(){
	hapticInterface->Update((float)GetHapticTimeStep());
	hapticPointer.SetFramePosition(hapticInterface->GetPosition() * posScale);
	hapticPointer.SetOrientation(hapticInterface->GetOrientation());
	hapticPointer.SetVelocity(hapticInterface->GetVelocity() * posScale);
	hapticPointer.SetAngularVelocity(hapticInterface->GetAngularVelocity());
}

void FWHapticBase::HapticRendering(){}
void FWHapticBase::Keyboard(int key, int x, int y){
}

int FWHapticBase::GetLoopCount(){
	return loopCounter;
}

}