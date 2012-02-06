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

class HIXbox360Controller :public HIXbox360ControllerDesc, public HIHaptic{
protected:
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
	float ts, rs;
	float vibScale;
	Posef currPose;
public:
	SPR_OBJECTDEF(HIXbox360Controller);
	HIXbox360Controller(const HIXbox360ControllerDesc& desc = HIXbox360ControllerDesc()){
		controllerID = -1;
	}


	virtual bool Init();
	virtual void Update(float dt);
	virtual void UpdateState();
	virtual void UpdatePose(float dt);
	virtual void Comp6DoF();
	virtual void Comp3DoF();
	virtual void CheckDeadZone();
	virtual void SetMaxVelocity(float v);
	virtual void SetMaxAngularVelocity(float v);
	virtual Posef GetPose();
	virtual Vec3f GetPosition();
	virtual Quaternionf GetOrientation();
	virtual Vec3f GetVelocity();
	virtual Vec3f GetAngularVelocity();
	virtual void SetVibration(Vec2f lr);
	//virtual 

};


}
#endif
