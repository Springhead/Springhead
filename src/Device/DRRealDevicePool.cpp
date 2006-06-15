#include "Device.h"
#pragma hdrstop

namespace Spr {


DRRealDevicePool::DRRealDevicePool(){
}
DRRealDevicePool::~DRRealDevicePool(){
}
///	“o˜^
void DRRealDevicePool::Register(DRRealDevice* dev){
	push_back(dev);
}
void DRRealDevicePool::Init(DVVirtualDevicePool& vpool){
	for(iterator it = begin(); it != end(); ++it){
		if ((*it)->Init()) (*it)->Register(vpool);
	}
}
void DRRealDevicePool::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DRRealDevicePool>" << std::endl;
	o.width(w+2);
	for(const_iterator it = begin(); it != end(); ++it){
		o << **it;
	}
	o.width(0);
	o << UTPadding(w) << "</DRRealDevicePool>" << std::endl;
	o.width(w);
}

}	//	namespace Spr
