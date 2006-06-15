#include "Device.h"
#pragma hdrstop

namespace Spr {

void DVDeviceManager::Init(){
	RPool().Init(VPool());
}
void DVDeviceManager::Clear(){
	VPool().Clear();
	RPool().Clear();
}

void DVDeviceManager::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DVDeviceManager>" << std::endl;
	o.width(w+2);
	o << VPool();
	o << RPool();
	o << UTPadding(w) << "</DVDeviceManager>" << std::endl;
	o.width(w);
}

}	//	namespace Spr
