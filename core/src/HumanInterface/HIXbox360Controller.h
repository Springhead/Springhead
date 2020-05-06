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

class HIXbox360ControllerImpl;

class HIXbox360Controller :public HIXbox360ControllerDesc, public HIHaptic{
	friend HIXbox360ControllerImpl;

protected:
	// no UTRef because gcc will require full type information of HIXbox360ControllerImpl 
	HIXbox360ControllerImpl* impl;
	
	int controllerID;
	bool key[14];
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

	HIXbox360Controller(const HIXbox360ControllerDesc& desc = HIXbox360ControllerDesc());
	~HIXbox360Controller();

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
	
	bool UP(){ return key[0]; }
	bool DOWN(){ return key[1]; }
	bool LEFT(){ return key[2]; }
	bool RIGHT(){ return key[3]; };
	bool START(){ return key[4]; };
	bool BACK(){ return key[5]; };
	bool LTHUMB(){ return key[6]; };
	bool RTHUMB(){ return key[7]; };
	bool LSHOULDER(){ return key[8]; };
	bool RSHOULDER(){ return key[9]; };
	bool A(){ return key[10]; };
	bool B(){ return key[11]; };
	bool X(){ return key[12]; };
	bool Y(){ return key[13]; };

};


}
#endif
