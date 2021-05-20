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
	ready = false;
	//	コンストラクション後に、SetSceneかSetNameManagerされるので、ここではGetSdk()は使えない。
}

bool HITrackball::Init(const void* desc){
	if (desc) SetDesc(desc);
	if (!keyMouse){
		DVKeyMouseIf* dv = GetSdk()->RentVirtualDevice(DVKeyMouseIf::GetIfInfoStatic())->Cast();
		if (!dv) return false;
		SetKeyMouse(dv);
	}
	return true;
}

void HITrackball::SetKeyMouse(DVKeyMouseIf* dv){
	if (keyMouse) keyMouse->RemoveCallback(this);
	keyMouse = dv;
	keyMouse->AddCallback(this);
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
		callback->OnUpdatePose(Cast());
	ready = true;
}

void HITrackball::SetMode(bool m){
	trackball = m;
	UpdateView();
}

bool HITrackball::GetMode(){
	return trackball;
}

void HITrackball::SetTarget(Vec3f t){
	target = t;
	UpdateView();
}

Vec3f HITrackball::GetTarget(){
	return target;
}

void HITrackball::SetAngle(float lon, float lat){
	longitude = lon, latitude = lat;
	UpdateView();
}

void HITrackball::GetAngle(float& lon, float& lat){
	lon = longitude, lat = latitude;
}

void HITrackball::SetDistance(float dist){
	distance = dist;
	UpdateView();
}

float HITrackball::GetDistance(){
	return distance;
}

void HITrackball::SetLongitudeRange(float rmin, float rmax){
	lonRange[0] = rmin, lonRange[1] = rmax;
	UpdateView();
}

void HITrackball::GetLongitudeRange(float& rmin, float& rmax){
	rmin = lonRange[0], rmax = lonRange[1];
}

void HITrackball::SetLatitudeRange(float rmin, float rmax){
	latRange[0] = rmin, latRange[1] = rmax;
	UpdateView();
}

void HITrackball::GetLatitudeRange(float& rmin, float& rmax){
	rmin = latRange[0], rmax = latRange[1];
}

void HITrackball::SetDistanceRange(float rmin, float rmax){
	distRange[0] = rmin, distRange[1] = rmax;
	UpdateView();
}

void HITrackball::GetDistanceRange(float& rmin, float& rmax){
	rmin = distRange[0], rmax = distRange[1];
}

void HITrackball::SetPosition(Vec3f p){
	pos			= p;
	Vec3f r		= pos - target;
	Vec3f r_xz	= r; r_xz.y = 0;
	distance	= r.norm();
	latitude	= atan2(r.y, r_xz.norm());
	longitude	= atan2(r.x, r.z);
	UpdateView();
}

void HITrackball::SetOrientation(Quaternionf ori){
	target		= pos + ori*Vec3f(0,0,-distance);
	Vec3f r		= pos - target;
	Vec3f r_xz	= r; r_xz.y = 0;
	latitude	= atan2(r.y, r_xz.norm());
	longitude	= atan2(r.x, r.z);
	UpdateView();
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
