#include "Device.h"
#pragma hdrstop

namespace Spr {;

DVCounterBase::DVCounterBase(){}
DVDaBase::DVDaBase(){}

void DVVirtualDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DVDevice type:\"" << Type() << "\" name:\"" << Name() << "\"/>" << std::endl;
	o.width(w);
}
}	//	namespace Spr
