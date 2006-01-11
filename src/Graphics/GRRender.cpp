#include "Graphics.h"
#pragma hdrstop

namespace Spr {;
//----------------------------------------------------------------------------
//	GRRender
OBJECTIMP(GRRender, Object);
IF_IMP(GRRender, Object);

void GRRender::Print(std::ostream& os) const{
	device->Print(os);
}

}	//	Spr

