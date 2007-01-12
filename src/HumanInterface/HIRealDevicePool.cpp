#include "HumanInterface.h"
#include "HIRealDevicePool.h"

namespace Spr {


HIRealDevicePool::HIRealDevicePool(){
}
HIRealDevicePool::~HIRealDevicePool(){
}
///	“o˜^
void HIRealDevicePool::Register(HIRealDevice* dev){
	push_back(dev);
}
void HIRealDevicePool::Init(HISdkIf* sdk){
	for(iterator it = begin(); it != end(); ++it){
		if ((*it)->Init()) (*it)->Register(sdk);
	}
}
void HIRealDevicePool::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<HIRealDevicePool>" << std::endl;
	o.width(w+2);
	for(const_iterator it = begin(); it != end(); ++it){
		o << **it;
	}
	o.width(0);
	o << UTPadding(w) << "</HIRealDevicePool>" << std::endl;
	o.width(w);
}

}	//	namespace Spr
