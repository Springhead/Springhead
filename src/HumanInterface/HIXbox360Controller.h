/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef HI_XBOX360CONTROLLER_H
#define HI_XBOX360CONTROLLER_H

#include <HumanInterface/HIBase.h>
#ifdef _WIN32
 #include <Windows.h>
 #include <Xinput.h>
 #pragma comment(lib, "Xinput.lib")
#endif

namespace Spr{;

class HIXbox360Controller : public HIHaptic{
public:
	SPR_OBJECTDEF(HIXbox360Controller);

	DWORD controllerID;
	XINPUT_STATE state;
	Vec2i thumbL;
	Vec2i thumbR;
	int leftTrigger;
	int rightTrigger;
	Vec2f n_thumbL;
	Vec2f n_thumbR;
	float n_leftTrigger;
	float n_rightTrigger;
	float trnScale; // [m/s]
	float rotScale; // [rad/s]
	float ts, rs;
	float vibScale;
	Posef currPose;
	HIXbox360Controller(const HIXbox360ControllerDesc& desc = HIXbox360ControllerDesc()){}

	virtual bool Init();
	void Update(float dt);
	void UpdateState();
	void UpdatePose(float dt);
	void Comp6DoF();
	void Comp3DoF();
	void CheckDeadZone();

	virtual Posef GetPose();
	virtual Vec3f GetPosition();
	virtual Quaternionf GetOrientation();
	void SetVibration(Vec2f lr);
};


}
#endif
