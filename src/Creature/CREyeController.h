/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CREYECONTROLLER_H
#define CREYECONTROLLER_H

#include <SprCreature.h>
#include <SprFoundation.h>
#include <SprPhysics.h>

#include <Foundation/Object.h>
#include "IfStubDumpCreature.h"

namespace Spr{;

class CREyeController : public SceneObject, CREyeControllerIfInit{
	OBJECTDEF(CREyeController, SceneObject);
private:
	PHSolidIf* soLEye;
	PHSolidIf* soREye;
	PHSolidIf* soHead;
	Vec3f      lookatPoint;

	// Value for Controlling
	Vec3f      targetDirL, targetDirR;

	void ControlEyeToTargetDir(PHSolidIf* soEye, Vec3f aim);

public:
	CREyeController(){}
	~CREyeController(){}
	virtual void SetLeftEyeSolid(PHSolidIf* eyeSolid);
	virtual void SetRightEyeSolid(PHSolidIf* eyeSolid);
	virtual void SetHeadSolid(PHSolidIf* headSolid);
	virtual void LookAt(Vec3f point);
	virtual void Step();
};

}

#endif//CREYECONTROLLER_H