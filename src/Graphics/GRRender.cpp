/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;

//----------------------------------------------------------------------------
//	GRRender
IF_OBJECT_IMP_ABST(GRRenderBase, Object);
IF_OBJECT_IMP(GRRender, GRRenderBase);

void GRRender::Print(std::ostream& os) const{
	device->Print(os);
}
void GRRender::Reshape(Vec2f pos, Vec2f screen){
	viewportPos = pos;
	viewportSize = screen;
	SetViewport(pos, screen);
	Vec2f sz = camera.size;
	if (sz.y==0) sz.y = sz.x*(viewportSize.y/viewportSize.x);
	if (sz.x==0) sz.x = sz.y*(viewportSize.x/viewportSize.y);
	Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
		sz, camera.front, camera.back);
	SetProjectionMatrix(afProj);
}
void GRRender::SetCamera(const GRCameraDesc& c){
	if (memcmp(&camera,&c, sizeof(c)) != 0){
		camera = c;
		Vec2f sz = camera.size;
		if (sz.y==0) sz.y = sz.x*(viewportSize.y/viewportSize.x);
		if (sz.x==0) sz.x = sz.y*(viewportSize.x/viewportSize.y);
		Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
			sz, camera.front, camera.back);
		SetProjectionMatrix(afProj);
	}
}

//----------------------------------------------------------------------------
//	GRDevice
IF_OBJECT_IMP_ABST(GRDevice, GRRenderBase);

//----------------------------------------------------------------------------
//	GRMaterial
IF_OBJECT_IMP(GRMaterial, GRVisual);
void GRMaterial::Render(GRRenderIf* render){
	render->SetMaterial(*this);
}

//----------------------------------------------------------------------------
//	GRLight
IF_OBJECT_IMP(GRLight, GRVisual);
void GRLight::Render(GRRenderIf* render){
	render->PushLight(*this);
}
void GRLight::Rendered(GRRenderIf* render){
	render->PopLight();
}

//----------------------------------------------------------------------------
//	GRCamera
IF_OBJECT_IMP(GRCamera, GRVisual);
size_t GRCamera::NChildObject() const {
	return frame ? 1 : 0;
}
ObjectIf* GRCamera::GetChildObject(size_t pos){
	if (pos == 0) return frame ? frame : NULL;
	return NULL;
}
bool GRCamera::AddChildObject(ObjectIf* o){
	GRFrame* f = DCAST(GRFrame, o);
	if (f){
		frame = f->Cast();
		return true;
	}
	return false;
}
void GRCamera::Render(GRRenderIf* r){
	if (frame){
		Affinef af = frame->GetWorldTransform();
		r->SetViewMatrix(af.inv());
	}
	r->SetCamera(*this);
}
void GRCamera::SetDesc(const GRCameraDesc& desc){
	*(GRCameraDesc*)this = desc;
}

}	//	Spr
