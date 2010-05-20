/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWIMPULSE_HAPTIC_H
#define FWIMPULSE_HAPTIC_H

#include <Framework/FWMultiRateHaptic.h>
#include <Framework/FWInteractScene.h>

class FWImpulseHapticLoop : public FWHapticLoopBase{
	Vec3d vibV;
	double vibT;
	// std::vector<Vec3d> oVibForce;
public:
	FWImpulseHapticLoop();
	virtual void Step();
	virtual void HapticRendering();
	virtual Vec3d Vibration(FWInteractSolid* iSolid, FWInteractPointer* iPointer, int n);
	virtual void ConstraintBasedRendering();
};

class FWImpulseHaptic : public FWMultiRateHaptic{
protected:
	FWImpulseHapticLoop impulseLoop;
public:
	FWImpulseHaptic();
	virtual void Init();
	virtual void Clear();
	void CallBackHapticLoop();
	virtual void SyncHaptic2Physic();
	virtual void SyncPhysic2Haptic();
	virtual void Step();
	virtual void UpdatePointer();
	virtual void BeginKeyboard();
	virtual void EndKeyboard();
};


#endif