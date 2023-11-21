/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HIBase.h>
#include <HumanInterface/HIDevice.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;
//-----------------------------------------------------------------
//	HIBase
//

int HIBase::deviceUpdateStep;
static UTRef<HIBase::RealDevices> realDevices = DBG_NEW HIBase::RealDevices;
UTRef<HIBase::RealDevices> HIBase::GetRealDevices(){
	if (realDevices) return realDevices;
	UTRef<RealDevices> rv = DBG_NEW RealDevices;

	return rv;
}
HISdkIf* HIBase::GetSdk(){
	return GetNameManager()->Cast();
}

void HIBase::AddDeviceDependency(HIRealDeviceIf* rd){
	GetRealDevices()->insert(rd->Cast());
}
void HIBase::ClearDeviceDependency(){
	GetRealDevices()->clear();
}
void HIBase::Update(float dt){
	updateStep ++;
	if (updateStep > deviceUpdateStep){
		UTRef<HIBase::RealDevices> rd = GetRealDevices();
		for(std::set<HIRealDevice*>::iterator it = rd->begin(); it != rd->end(); ++it){
			(*it)->Update();
		}
		deviceUpdateStep = updateStep;
	}else{
		updateStep = deviceUpdateStep;
	}
}
//コロコロ用
//void HIBase::Update(float dt,Vec3d Pos) {
//	updateStep++;
//	if (updateStep > deviceUpdateStep) {
//		UTRef<HIBase::RealDevices> rd = GetRealDevices();
//		for (std::set<HIRealDevice*>::iterator it = rd->begin(); it != rd->end(); ++it) {
//			(*it)->Update();
//		}
//		deviceUpdateStep = updateStep;
//	}
//	else {
//		updateStep = deviceUpdateStep;
//	}
//}
/*
//-----------------------------------------------------------------
//	HIPosition
//
Vec3f HIPosition::GetPosition(){
	return Vec3f();
}

//-----------------------------------------------------------------
//	HIOrientation
//
Quaternionf HIOrientation::GetOrientation(){
	return Quaternionf();
}
*/

void HIHaptic::Update(float dt){
	HIPose::Update(dt);
	Vec3f pos = GetPosition();
	Quaternionf ori = GetOrientation();
	Vec3f v = (pos - lastPos) / dt;
	Vec3f av = (ori * lastOri.Inv()).Rotation() / dt;
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
	lastPos = pos;
	lastOri = ori;
}


}	//	namespace Spr
