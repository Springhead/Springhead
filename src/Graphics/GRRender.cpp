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
void GRRender::Reshape(Vec2f pos, Vec2f screen){
	SetViewport(pos, screen);
	Vec2f sz = camera.size;
	sz.y = sz.x*(screen.y/screen.x);
	Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
		sz, camera.front, camera.back);
	SetProjectionMatrix(afProj);
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

//----------------------------------------------------------------------------
//	GRCamera
IF_OBJECT_IMP(GRCamera, SceneObject);
GRCameraDesc::GRCameraDesc():
	center(Vec2f()), size(Vec2f(0.2f, 0.2f)), front(0.1f), back(500.0f){
}

}	//	Spr

