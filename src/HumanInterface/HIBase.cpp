/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HumanInterface.h"
#include <HumanInterface/SprHIBase.h>
#include <HumanInterface/SprHIDevice.h>
#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;
//-----------------------------------------------------------------
//	HIBase
//

IF_SPR_OBJECTIMP_ABST(HIBase, NamedObject);


std::set<HIRealDevice*> HIBase::realDevices;
int HIBase::deviceUpdateStep;

void HIBase::AddDeviceDependency(HIRealDeviceIf* rd){
	assert(0);
	//	hase TODO ‚Ü‚¾ŽÀ‘•‚µ‚Ä‚È‚¢
	//	realDevices.insert(rd);
}
void HIBase::ClearDeviceDependency(){
	realDevices.clear();
}
void HIBase::Update(float dt){
	updateStep ++;
	if (updateStep > deviceUpdateStep){
		for(std::set<HIRealDevice*>::iterator it = realDevices.begin(); it != realDevices.end(); ++it){
			(*it)->Update();
		}
		deviceUpdateStep = updateStep;
	}else{
		updateStep = deviceUpdateStep;
	}
}

//-----------------------------------------------------------------
//	HIPosition
//
IF_SPR_OBJECTIMP(HIPosition, HIBase);
Vec3f HIPosition::GetPosition(){
	return Vec3f();
}

//-----------------------------------------------------------------
//	HIOrientation
//
IF_SPR_OBJECTIMP(HIOrientation, HIBase);
Quaternionf HIOrientation::GetOrientation(){
	return Quaternionf();
}

//-----------------------------------------------------------------
//	HIPose
//
IF_SPR_OBJECTIMP(HIPose, HIPosition);
Quaternionf HIPose::GetOrientation(){
	return Quaternionf();
}
Posef HIPose::GetPose(){
	Posef rv;
	rv.Ori() = GetOrientation();
	rv.Pos() = GetPosition();
	return rv;
}


}	//	namespace Spr
