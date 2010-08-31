/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#pragma hdrstop

#include <iomanip>
#include <sstream>
#include "boost/regex.hpp"

namespace Spr {;

static boost::regex Tex3DRegex("^(.*_tex3d_)([0-9]+)(\\Q.\\E[^\\Q.\\E]+)$");
bool GRMaterialDesc::Is3D() const{
	return boost::regex_match(texname, Tex3DRegex);
}

//----------------------------------------------------------------------------
//	GRRender
void GRRender::Print(std::ostream& os) const{
	device->Print(os);
}
void GRRender::Reshape(Vec2f pos, Vec2f screen){
	viewportPos = pos;
	viewportSize = screen;
	SetViewport(pos, screen);
	bool yIsZero = false, xIsZero = false;
	if (camera.size.y<=0){
		yIsZero = true;
		camera.size.y = camera.size.x*(viewportSize.y/viewportSize.x);
	}
	if (camera.size.x<=0){
		xIsZero = true;
		camera.size.x = camera.size.y*(viewportSize.x/viewportSize.y);
	}
	Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
		camera.size, camera.front, camera.back);
	SetProjectionMatrix(afProj);
	if (yIsZero) { camera.size.y = 0; }
	if (xIsZero) { camera.size.x = 0; }
}
void GRRender::SetCamera(const GRCameraDesc& c){
	if (memcmp(&camera,&c, sizeof(c)) != 0){
		camera = c;
		if (camera.size.y==0) camera.size.y = camera.size.x*(viewportSize.y/viewportSize.x);
		if (camera.size.x==0) camera.size.x = camera.size.y*(viewportSize.x/viewportSize.y);
		Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
			camera.size, camera.front, camera.back);
		SetProjectionMatrix(afProj);
	}
}
Vec2f GRRender::GetPixelSize(){
	// ピクセルサイズ = スクリーンサイズ / ビューポートサイズ
	Vec2f px(camera.size.x / viewportSize.x, camera.size.y / viewportSize.y);
	if(px.x == 0.0f)
		px.x = px.y;
	if(px.y == 0.0f)
		px.y = px.x;
	return px;
}	
Vec3f GRRender::ScreenToCamera(int x, int y, float depth, bool LorR){
	//アスペクト比
	double r = (viewportSize.x / viewportSize.y);

	//スクリーンサイズ
	Vec2f camSize = camera.size;
	if(camSize.x ==0) camSize.x = camSize.y * r ;	//片方を0に設定してある場合，内部で自動的に比率が計算されているので，再度計算し代入する
	if(camSize.y ==0) camSize.y = camSize.x / r ;

	Vec3f pos(
		 camera.center.x + ((float)x - viewportSize.x / 2.0f) * (camSize.x / viewportSize.x),
		 camera.center.y + (viewportSize.y / 2.0f - (float)y) * (camSize.y / viewportSize.y),
		 (LorR ? camera.front : -camera.front));
	pos *= (depth / camera.front);
	return pos;
}

//----------------------------------------------------------------------------
//	GRMaterial
void GRMaterial::Render(GRRenderIf* render){
	render->SetMaterial(*this);
}

//----------------------------------------------------------------------------
//	GRLight
void GRLight::Render(GRRenderIf* render){
	render->PushLight(*this);
}
void GRLight::Rendered(GRRenderIf* render){
	render->PopLight();
}

//----------------------------------------------------------------------------
//	GRCamera
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

}	//	Spr
