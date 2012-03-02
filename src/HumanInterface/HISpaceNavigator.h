/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef HISPACENAVIGATOR_H
#define HISPACENAVIGATOR_H

#include <HumanInterface/HIBase.h>

#include <vector>

namespace Spr{;

// 3DConnexion SpaceNavigator. 6自由度入力デバイス
class HISpaceNavigator: public HIPose{
public:
	SPR_OBJECTDEF(HISpaceNavigator);

	HISpaceNavigator(const HISpaceNavigatorDesc& desc = HISpaceNavigatorDesc()) { hWnd=NULL; hDevice=NULL; }

	virtual bool			Init(const void* desc);
	virtual bool			Calibration();
	virtual bool			SetPose(Posef pose);
	virtual Vec3f			GetPosition();
	virtual Quaternionf		GetOrientation();
	virtual Posef			GetPose();
	virtual Affinef			GetAffine();
	virtual Vec3f			GetTrnDelta() { return dTrn; }
	virtual Vec3f			GetRotDelta() { return dRot; }

	virtual bool			PreviewMessage(void *m);

private:
	static std::vector<void*> deviceHandles;

	static const int VENDOR_ID = 0x046d;
	static const int PID_BEGIN = 0xc625;
	static const int PID_END   = 0xc629;

	void *hWnd, *hDevice;
	Posef currPose;
	Vec3d dTrn,dRot;

	void Translate(Vec3f trn);
	void Rotate(Vec3f rot);
};

}

#endif//HISPACENAVIGATOR_H