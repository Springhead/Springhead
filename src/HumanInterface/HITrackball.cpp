/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HITrackball.h>
#include <Graphics/SprGRRender.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

void Limit(float& v, Vec2f range){
	if(range[0] <= range[1])
		v = std::min(std::max(range[0], v), range[1]);
}

HITrackball::HITrackball(const HITrackballDesc& desc):HITrackballDesc(desc){
	keyMouse = 0;
	callback = 0;
	enabled = true;
}

bool HITrackball::Init(const void* desc){
	return true;
}

void HITrackball::SetKeyMouse(DVKeyMouseIf* dv){
	keyMouse = dv;
	keyMouse->AddHandler(this);
}

void HITrackball::UpdateView(){
	if(trackball){

	}
	else{
		pos = target + distance * Vec3f(
			cos(latitude) * sin(longitude),
			sin(latitude),
			cos(latitude) * cos(longitude));
		ori = Quaternionf::Rot(longitude, 'y') * Quaternionf::Rot(-latitude, 'x');
	}
	if(callback)
		callback->OnUpdatePose(Cast(), true);
}

void HITrackball::SetPosition(Vec3f p){
	pos			= p;
	Vec3f r		= pos - target;
	distance	= r.norm();
	longitude	= atan2(r.z, r.x);
	latitude	= atan2(r.y, Vec2f(r.x, r.z).norm());
	UpdateView();
}

void HITrackball::Update(float dt){
}

void HITrackball::Fit(const GRCameraDesc& cam, float radius){
	float sz = std::min(cam.size.x, cam.size.y);
	if(sz == 0.0f)
		sz = cam.size.x;
	distance = cam.front * (radius / (sz/2.0f));
	UpdateView();
}

void HITrackball::Rotate(int xrel, int yrel){
	if(trackball){
	}
	else{
		longitude -= (float)xrel * rotGain;
		latitude  += (float)yrel * rotGain;
		Limit(longitude, lonRange);
		Limit(latitude,  latRange);
	}
	UpdateView();
}

void HITrackball::Zoom(int yrel){
	distance *= (float)exp((double)yrel * zoomGain);
	Limit(distance, distRange);
	UpdateView();
}

void HITrackball::Translate(int xrel, int yrel){
	Vec3f rel(-(float)xrel * distance * trnGain, (float)yrel * distance * trnGain, 0.0f);
	target += ori * rel;
	UpdateView();
}

bool HITrackball::OnMouseMove(int button, int x, int y, int dz){ 
	if(!enabled)
		return false;

	int oldx, oldy, time;
	int dx, dy;
	keyMouse->GetMousePosition(oldx, oldy, time, 1);
	dx = x - oldx;
	dy = y - oldy;
	if(button == rotMask){
		Rotate(dx, dy);
		return true;
	}
	if(button == zoomMask){
		Zoom(dy);
		return true;
	}
	if(button == trnMask){
		Translate(dx, dy);
		return true;
	}
	return false;
}

}	//	namespace Spr
