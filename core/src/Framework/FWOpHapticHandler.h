/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef FWOPHAPTICHANDLER_H
#define FWOPHAPTICHANDLER_H

#include <Framework/SprFWHapticPointer.h>
#include <Foundation/Object.h>
#include <HumanInterface/SprHISpidar.h>
#include <HumanInterface/SprHIDRUsb.h>
#include <HumanInterface/SprHIKeyMouse.h>
#include <HumanInterface/SprHISdk.h>
#include <Physics/PHOpHapticController.h>
#include <Framework/SprFWOpHapticHandler.h>

namespace Spr {
	;

	class FWOpHapticHandler : public SceneObject{
		SPR_OBJECTDEF(FWOpHapticHandler);

	public:
		FWOpHapticHandler()

		{
			hcReady = false;
			hapticTimeInterval = 0.001f;
		}

		HIBaseIf* humanInterface;
		SpatialVector hapticForce;
		
		bool hcReady;
		float hapticTimeInterval;

		void SetHapticflag(bool flag);
		bool IsHapticEnabled();
		void SetHapticTimeInterval(float dt);
		float GetHapticTimeInterval();
		
		bool doCalibration(float dt);
		void UpdateHumanInterface(PHOpHapticController* opHC, float dt);

		void SetHumanInterface(HIBaseIf* hi){ humanInterface = hi; }
		HIBaseIf* GetHumanInterface(){ return humanInterface; }

		Vec3d GetHapticForce(){ return hapticForce.v(); }
		Vec3d GetHapticTorque(){ return hapticForce.w(); }
	};


}//namespace

#endif
