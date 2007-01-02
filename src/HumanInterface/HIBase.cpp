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

IF_OBJECT_IMP_ABST(HIBase, NamedObject);


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
