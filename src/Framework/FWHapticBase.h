/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWHAPTIC_BASE_H
#define FWHAPTIC_BASE_H

#include <Springhead.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class FWHapticBase{
protected:
	UTRef<HIForceInterface6DIf> hapticInterface;
	PHSolid hapticPointer;
	int loopCounter;
	float posScale;
	double hapticTimeStep;
	double physicTimeStep;
public:
	FWHapticBase();
	void InitHumanInterface(const IfInfo* info, void* desc);	
	virtual void UpdateHumanInterface();
	virtual void HapticRendering();
	virtual void Keyboard(int key, int x, int y);
	
	void SetHapticTimeStep(double dt);
	double GetHapticTimeStep();
	void SetPhysicTimeStep(double dt);
	double GetPhysicTimeStep();
	HIForceInterface6DIf* GetHumanInterface();
	int GetLoopCount();
};

}
#endif