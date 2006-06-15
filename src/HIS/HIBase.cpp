#include "HIS.h"

#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;
HIOBJECTIMPBASEABST(HIBase);

std::set<DRRealDevice*> HIBase::realDevices;
int HIBase::deviceUpdateStep;

void HIBase::AddRealDeviceDependency(DRRealDevice* rd){
	realDevices.insert(rd);
}
void HIBase::ClearRealDeviceDependency(){
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


}	//	namespace Spr
