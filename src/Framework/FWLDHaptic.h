/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWLDHaptic_H 
#define FWLDHaptic_H

#include <Framework/FWMultiRateHaptic.h>
#include <Framework/FWInteractAdaptee.h>

class FWLDHapticLoop : public FWHapticLoopBase{
private:
	Vec3d vibV;
	double vibT;
	double proK;
	double proD;
	double proM;
	Vec3d pLastPoint[2][100];
	Vec3d proxy[2][100];
	Vec3d proVel[2][100];
	Vec3d dproxy;
public:
	FWLDHapticLoop();
	virtual void Step();
	virtual void UpdateInterface();
	virtual void HapticRendering();
	virtual void Proxy();
	virtual void ProxySimulation();
	virtual Vec3d Vibration(FWInteractSolid*, FWInteractPointer*);
	virtual void LocalDynamics();
};


class FWLDHaptic : public FWMultiRateHaptic{
protected:
	UTRef<ObjectStatesIf> states, states2;
	FWLDHapticLoop LDHapticLoop;
public:
	FWLDHaptic();
	virtual void Init();
	virtual void Clear();
	void CallBackHapticLoop();
	virtual void Step();
	virtual void PhysicsStep();
	virtual void UpdatePointer();
	virtual void TestSimulation();

};

#endif