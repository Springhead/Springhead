/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FW_LD_Haptic_App_H 
#define FW_LD_Haptic_App_H

#include <Framework/FWHapticProcessBase.h>
#include <Framework/FWMultiRateHapticApp.h>

class FWLDHapticProcess : public FWHapticProcessBase{
public:
	FWExpandedPHSolids expandedPHSolids;
	void Step();
	void LocalDynamics();
};

class FWLDHapticApp : public FWMultiRateHapticApp{
public:
	UTRef<ObjectStatesIf> states, states2;
	FWLDHapticProcess hprocess;
	FWLDHapticApp();
	virtual void ResetScene();
	virtual void CallBack();
	virtual void Step();
	virtual void TestSimulation();
};


#endif