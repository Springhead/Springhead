#include "Graphics.h"
#pragma hdrstop

namespace Spr {;
//----------------------------------------------------------------------------
//	GRRenderBase
OBJECTIMPABST(GRRenderBase, Object);
IF_IMP(GRRenderBase, Object);

//----------------------------------------------------------------------------
//	GRRender
OBJECTIMP(GRRender, Object);
IF_IMP(GRRender, GRRenderBase);

void GRRender::Print(std::ostream& os) const{
	device->Print(os);
}

//----------------------------------------------------------------------------
//	GRDevice
OBJECTIMPABST(GRDevice, Object);
IF_IMP(GRDevice, GRRenderBase);

}	//	Spr

