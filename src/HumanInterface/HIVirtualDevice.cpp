#include "HumanInterface.h"
#pragma hdrstop

namespace Spr {;

IF_OBJECT_IMP_ABST(HIVirtualDevice, Object);

void HIVirtualDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DVDevice type:\"" << Type() << "\" name:\"" << Name() << "\"/>" << std::endl;
	o.width(w);
}


}	//	namespace Spr
