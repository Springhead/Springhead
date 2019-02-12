/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include "FWOpHapticHandler.h"
#include "Physics/PHOpHapticController.h"

namespace Spr {
	;

	void FWOpHapticHandler::SetHapticTimeInterval(float dt)
	{
		hapticTimeInterval = dt;
	}
	float FWOpHapticHandler::GetHapticTimeInterval()
	{
		return hapticTimeInterval;
	}
	void FWOpHapticHandler::SetHapticflag(bool flag)
	{
		hcReady = flag;
	}

	bool FWOpHapticHandler::IsHapticEnabled()
	{
		return hcReady;
	}

	void FWOpHapticHandler::UpdateHumanInterface(PHOpHapticController* opHC, float dt)
	{
		HIHapticIf* hiHaptic = DCAST(HIHapticIf, humanInterface);
		HIPoseIf*   hiPose = DCAST(HIPoseIf, humanInterface);

		// 力の出力
		if (hiHaptic) {
			hapticForce = opHC->GetHCOutput();
			if (opHC->GetHCForceReady()){
				hiHaptic->SetForce(hapticForce.v(), hapticForce.w());
				opHC->SetHCForceReady(false);
			}
			else{
				hiHaptic->SetForce(Vec3f(), Vec3f());
			}
		}

		// インタフェースの状態更新
		if (hiPose) {
			hiPose->Update(dt);
			SpatialVector vel;
			vel.v() = hiPose->GetVelocity();
			vel.w() = hiPose->GetAngularVelocity();
			Posed pose = hiPose->GetPose();

			opHC->SetHCPosition(pose.Pos());
			opHC->SetHCPose(pose);
		}
	}

	bool FWOpHapticHandler::doCalibration(float dt)
	{
		HIHapticIf* hiHaptic = DCAST(HIHapticIf, humanInterface);
		
		if (hiHaptic->Calibration())
			{
			hiHaptic->Update(dt);
				return true;
			}
		
		return false;
	}
}//namespace
