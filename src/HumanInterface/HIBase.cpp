#include "HumanInterface.h"
#include <Device/DRRealDevice.h>
#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;
//-----------------------------------------------------------------
//	HIBase
//

IF_OBJECT_IMP_ABST(HIBase, NamedObject);


std::set<DRRealDevice*> HIBase::realDevices;
int HIBase::deviceUpdateStep;

void HIBase::AddDeviceDependency(DRRealDeviceIf* rd){
//	realDevices.insert(rd);
}
void HIBase::ClearDeviceDependency(){
	realDevices.clear();
}
void HIBase::Update(float dt){
	updateStep ++;
	if (updateStep > deviceUpdateStep){
		for(std::set<DRRealDevice*>::iterator it = realDevices.begin(); it != realDevices.end(); ++it){
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
IF_OBJECT_IMP(HIPosition, HIBase);
Vec3f HIPosition::GetPosition(){
	return Vec3f();
}

//-----------------------------------------------------------------
//	HIOrientation
//
IF_OBJECT_IMP(HIOrientation, HIBase);
Quaternionf HIOrientation::GetOrientation(){
	return Quaternionf();
}

//-----------------------------------------------------------------
//	HIPose
//
IF_OBJECT_IMP(HIPose, HIPosition);
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
