#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;

//----------------------------------------------------------------------------
//	GRRenderBase
IF_OBJECT_IMP_ABST(GRRenderBase, Object);

//----------------------------------------------------------------------------
//	GRRender
IF_OBJECT_IMP(GRRender, Object);

void GRRender::Print(std::ostream& os) const{
	device->Print(os);
}

//----------------------------------------------------------------------------
//	GRDevice
IF_OBJECT_IMP_ABST(GRDevice, GRRenderBase);

//----------------------------------------------------------------------------
//	GRMaterial
IF_OBJECT_IMP(GRMaterial, GRVisual);
void GRMaterial::Render(GRRenderIf* render){
	render->SetMaterial(this);
}

//----------------------------------------------------------------------------
//	GRLight
IF_OBJECT_IMP(GRLight, GRVisual);
void GRLight::Render(GRRenderIf* render){
	render->PushLight(this);
}
void GRLight::Rendered(GRRenderIf* render){
	render->PopLight();
}

}	//	Spr

