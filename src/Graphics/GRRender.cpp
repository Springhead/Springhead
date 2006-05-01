#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

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
OBJECTIMPABST(GRDevice, GRRenderBase);
IF_IMP(GRDevice, GRRenderBase);

}	//	Spr

