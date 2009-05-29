/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWHapticProcessBase.h>
//#include <conio.h>
#include <HumanInterface/SprHIDRUsb.h>


namespace Spr{;

FWHapticProcessBase::FWHapticProcessBase(){}

void FWHapticProcessBase::SetPhysicTimeStep(double dt){ physicTimeStep = dt; }
double FWHapticProcessBase::GetPhysicTimeStep(){ return physicTimeStep; }
void FWHapticProcessBase::SetHapticTimeStep(double dt){ hapticTimeStep = dt; }
double FWHapticProcessBase::GetHapticTimeStep(){ return hapticTimeStep; }
void FWHapticProcessBase::SetHapticInterface(HISpidarGIf* s){ hapticInterface = s; }
HIForceInterface6DIf* FWHapticProcessBase::GetHapticInterface(){ return hapticInterface; }
PHSolid FWHapticProcessBase::GetHapticPointer(){ return hapticPointer; }
int FWHapticProcessBase::GetNExpandedPHSolids(){ return expandedPHSolids.size(); }
FWExpandedPHSolid** FWHapticProcessBase::GetFWExpandedPHSolids(){
	return expandedPHSolids.empty() ? NULL : (FWExpandedPHSolid**)&*expandedPHSolids.begin();
}
int FWHapticProcessBase::GetLoopCount(){ return loopCounter; }

void FWHapticProcessBase::ResetHapticProcess(){
	loopCounter = 1;
}

void FWHapticProcessBase::UpdateHumanInterface(){
	hapticInterface->Update((float)GetHapticTimeStep());
	hapticPointer.SetFramePosition(hapticInterface->GetPosition() * posScale);
	hapticPointer.SetOrientation(hapticInterface->GetOrientation());
	hapticPointer.SetVelocity(hapticInterface->GetVelocity() * posScale);
	hapticPointer.SetAngularVelocity(hapticInterface->GetAngularVelocity());
}
void FWHapticProcessBase::HapticRendering(){}
void FWHapticProcessBase::Keyboard(int key, int x, int y){}


}