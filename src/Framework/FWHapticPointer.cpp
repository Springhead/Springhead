#include "Framework/FWHapticPointer.h"
#include "Physics/PHHapticPointer.h"

namespace Spr{;

void FWHapticPointer::UpdateHumanInterface(PHHapticPointer* hpLocal, float dt){
	HIHapticIf* hiHaptic = DCAST(HIHapticIf, humanInterface);
	HIPoseIf*   hiPose   = DCAST(HIPoseIf, humanInterface);

	// 力の出力
	if (hiHaptic) {
		hapticForce  = hpLocal->GetHapticForce();
		if(hpLocal->bForce){
			hiHaptic->SetForce(hapticForce.v(), hapticForce.w());
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

		//// デバックのための擬似入力
		//Vec3d debug_vel = Vec3d(-0.001, 0.0, 0.0);
		//static Posed debug_pose = Posed();
		//debug_pose.Pos() = debug_pose.Pos() + debug_vel * dt;
		//pose = debug_pose;
		//vel.v() = debug_vel;

		hpLocal->UpdateHumanInterface(pose, vel);
	}
}


}