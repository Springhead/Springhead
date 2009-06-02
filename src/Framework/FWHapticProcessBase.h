/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FW_HAPTIC_PROCESS_BASE_H
#define FW_HAPTIC_PROCESS_BASE_H

#include <Springhead.h>
#include <Physics/PHSolid.h>
#include <Framework/FWExpandedPHSolid.h>

namespace Spr{;

class FWHapticProcessBase{
protected:
	UTRef<HIForceInterface6DIf> hapticInterface;
	PHSolid hapticPointer;
	float posScale;
	double hapticTimeStep;
	double physicTimeStep;
public:
	FWExpandedPHSolids expandedPHSolids;
	int loopCounter;
	
	FWHapticProcessBase();

	void SetHapticTimeStep(double dt);
	double GetHapticTimeStep();
	void SetPhysicTimeStep(double dt);
	double GetPhysicTimeStep();
	void SetHapticInterface(HISpidarGIf* s);
	HIForceInterface6DIf* GetHapticInterface();
	PHSolid GetHapticPointer();
	int GetNExpandedPHSolids();
	FWExpandedPHSolids* GetFWExpandedPHSolids();

	int GetLoopCount();

	void InitHumanInterface(const IfInfo* info, void* desc);
	void ResetHapticProcess();
	virtual void UpdateHumanInterface();
	virtual void HapticRendering();
	virtual void Keyboard(int key, int x, int y);
	
};

}
#endif
