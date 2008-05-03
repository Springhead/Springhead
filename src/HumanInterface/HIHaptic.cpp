#include "HumanInterface.h"
#include "HIHaptic.h"

#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;
HIForceInterface6D::HIForceInterface6D(){
	assert(0);
}
void HIForceInterface6D::Update(float dt){
	assert(0);
}

#if 0
HIOBJECTIMPABST(HIForceDevice3D, HIForceDevice6D);

HIForceDevice3D::HIForceDevice3D(){
}

HIOBJECTIMPABST(HIForceDevice6D, HIHapticDevice);
HIForceDevice6D::HIForceDevice6D():alpha(0.8f){
}
void HIForceDevice6D::Update(float dt){
	HIHapticDevice::Update(dt);
	Vec3f pos = GetPos();
	Quaternionf ori = GetOri();
	Vec3f v = (pos - lastPos) / dt;
//	if (v.norm() > 6.0f) v = v.unit() * 6.0f;
	Vec3f av = (ori * lastOri.Inv()).Rotation() / dt;
//	if (av.norm() > 8*M_PI) av = av.unit() * 8*M_PI;
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
	lastPos = pos;
	lastOri = ori;
}

HIOBJECTIMPABST(HIHapticDevice, HIBase);
void HIHapticDevice::AddRealDeviceDependency(DRRealDevice* rd){
	realDevices.insert(rd);
}
void HIHapticDevice::ClearRealDeviceDependency(){
	realDevices.clear();
}
void HIHapticDevice::Update(float dt){
	updateStep ++;
	if (updateStep > deviceUpdateStep && realDevices.size() > 0){
		for(std::set<DRRealDevice*>::iterator it = realDevices.begin(); it != realDevices.end(); ++it){
			(*it)->Update();
		}
		deviceUpdateStep = updateStep;
	}else{
		updateStep = deviceUpdateStep;
	}
}
#endif

}	//	namespace Spr
