/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWHAPTIC_APP_BASE_H
#define FWHAPTIC_APP_BASE_H

#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>

namespace Spr{;

class FWHapticAppBase : public FWAppGLUT, public UTRefCount{
public:

	FWHapticAppBase();
	virtual void Init(int argc, char* argv[]);
	virtual void InitCameraView();
	virtual void ResetScene();
	virtual void BuildScene();
	virtual void CallBack();
	virtual void Start();
	virtual void Idle();
	virtual void Step();
	virtual void Display();
	virtual void SyncHapticProcess();
	virtual void ExpandPHSolidInfo();
	virtual void FindNearestObjectFromHapticPointer();
	virtual void PredictSimulation();
	virtual void Keyboard(int key, int x, int y);
};

}
#endif