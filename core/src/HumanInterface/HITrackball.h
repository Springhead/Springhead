/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HI_TRACKBALL_H
#define HI_TRACKBALL_H

#include <HumanInterface/HIBase.h>
#include <HumanInterface/HIDevice.h>
#include <HumanInterface/SprHITrackball.h>

namespace Spr {;

class SPR_DLL HITrackball: public HIPose, public HITrackballDesc, public DVKeyMouseCallback{
public:
	SPR_OBJECTDEF(HITrackball);

	DVKeyMouseIf*	keyMouse;
	Vec3f			pos;
	Quaternionf		ori;

	HITrackballCallback*	callback;
	bool			enabled;
	bool			ready;

public:
	DVKeyMouseIf*	GetKeyMouse(){ return keyMouse; }
	void			SetKeyMouse(DVKeyMouseIf* dv);

	void	SetCallback(HITrackballCallback* cb){ callback = cb; }

	void	SetMode(bool m);
	bool	GetMode();
	void	SetTarget(Vec3f t);
	Vec3f	GetTarget();
	void	SetAngle(float lon, float lat);
	void	GetAngle(float& lon, float& lat);
	void	SetDistance(float dist);
	float	GetDistance();
	void	SetLongitudeRange(float rmin, float rmax);
	void	GetLongitudeRange(float& rmin, float& rmax);
	void	SetLatitudeRange(float rmin, float rmax);
	void	GetLatitudeRange(float& rmin, float& rmax);
	void	SetDistanceRange(float rmin, float rmax);
	void	GetDistanceRange(float& rmin, float& rmax);
	void	SetRotGain(float g){ rotGain = g; }
	float	GetRotGain(){ return rotGain; }
	void	SetZoomGain(float g){ zoomGain = g; }
	float	GetZoomGain(){ return zoomGain; }
	void	SetTrnGain(float g){ trnGain = g; }
	float	GetTrnGain(){ return trnGain; }

	void	SetPosition(Vec3f pos);
	void	SetOrientation(Quaternionf ori);

	void	SetRotMask(int mask){ rotMask = mask; }
	void	SetZoomMask(int mask){ zoomMask = mask; }
	void	SetTrnMask(int mask){ trnMask = mask; }

	void	UpdateView();
	void	Fit(const GRCameraDesc& cam, float radius);
	void	Rotate(int xrel, int yrel);
	void	Zoom(int yrel);
	void	Translate(int xrel, int yrel);
	void	Enable(bool on){ enabled = on; }
	bool	IsEnabled(){ return enabled; }
	
	// HIBaseの関数
	virtual bool Init(const void* desc);


	// HIPoseの関数
	virtual Vec3f		GetPosition(){ return pos; }
	virtual Quaternionf GetOrientation(){ return ori; }

	/// DVKeyMouseCallbackの関数
	virtual bool OnMouseMove(int state, int x, int y, int zdelta);

	HITrackball(const HITrackballDesc& desc = HITrackballDesc());
};

}	//	namespace Spr
#endif
