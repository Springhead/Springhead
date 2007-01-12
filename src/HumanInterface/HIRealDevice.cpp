#include "HumanInterface.h"
#include "HIRealDevice.h"
#pragma hdrstop

namespace Spr {;

IF_OBJECT_IMP_ABST(HIRealDevice, Object);

void HIRealDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DRDevice name:\"" << Name() << "\"/>" << std::endl;
	o.width(w);
}

}	//	namespace Spr
