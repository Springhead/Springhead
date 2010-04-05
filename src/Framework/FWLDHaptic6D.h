/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWLDHaptic6D_H 
#define FWLDHaptic6D_H

#include <Framework/FWLDHaptic3D.h>
#include <Framework/FWInteractScene.h>

class FWLDHaptic6DLoop : public FWLDHaptic3DLoop{
public:
	FWLDHaptic6DLoop();
	virtual void Step();
	virtual void ConstraintBasedRendering();
	virtual void LocalDynamics6D();
};


class FWLDHaptic6D : public FWLDHaptic3D{
protected:
	FWLDHaptic6DLoop ldLoop6D;
public:
	FWLDHaptic6D();
	virtual void Step();
	virtual void TestSimulation6D();

};

#endif