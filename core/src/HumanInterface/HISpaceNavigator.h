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
class HISpaceNavigator: public HISpaceNavigatorDesc, public HIPose{
public:
	SPR_OBJECTDEF(HISpaceNavigator);

	HISpaceNavigator(const HISpaceNavigatorDesc& desc = HISpaceNavigatorDesc()) {
		hWnd=desc.hWnd;
		maxVelocity = desc.maxVelocity;
		maxAngularVelocity = desc.maxAngularVelocity;
		
		hDevice=NULL;
		velocity = Vec3d(); angularVelocity = Vec3d();
	}

	virtual bool			Init(const void* desc);
	virtual bool			Calibration();

	virtual void			Update(float dt);

	virtual bool			SetPose(Posef pose);
	virtual Posef			GetPose();

	virtual Vec3f			GetPosition();
	virtual Quaternionf		GetOrientation();
	virtual Affinef			GetAffine();

	virtual Vec3f			GetVelocity()		{ return velocity; }
	virtual Vec3f			GetAngularVelocity()	{ return angularVelocity; }

	virtual void			SetMaxVelocity(float mV)         { maxVelocity = mV; }
	virtual void			SetMaxAngularVelocity(float mAV) { maxAngularVelocity = mAV; }

	virtual void			SetViewMatrix(Affinef view) { mView = view; }
	virtual void			SetPersMatrix(Affinef pers) {
		for (int i=0; i<4; ++i) {
			for (int j=0; j<4; ++j) {
				mPers[i][j] = pers[i][j];
			}
		}
	}

	virtual bool			PreviewMessage(void *m);

private:
	static std::vector<void*> deviceHandles;

	static const int VENDOR_ID = 0x046d;
	static const int PID_BEGIN = 0xc625;
	static const int PID_END   = 0xc629;

	static const int INPUT_ABS_MAX = 350; // SpaceNavigatorの取り得る絶対値の最大値

	void *hDevice;
	Posef basePose, currPose;
	Vec3d velocity, angularVelocity, v, aV;
	Affinef mView;
	typedef PTM::TMatrixRow<4,4,float> Matrix4f;
	Matrix4f mPers;
};

}

#endif//HISPACENAVIGATOR_H
