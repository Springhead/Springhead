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
			std::fstream log("log_hapticForce.txt", std::ios::app);
			log << "hapticForce(" << hapticForce << ") called." << std::endl;
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
		static Posed debug_pose = Posed();//現在の位置を取得
		Vec3d debug_vel = Vec3d(0.0, 0.0, 0.0);//右に進む速さの指定
		if (debug_pose.PosY() > -0.01) {
			//最初は下方向に進む
			debug_vel = Vec3d(0.0, -0.01, 0.0);
		}
		debug_pose.Pos() = debug_pose.Pos() + debug_vel * dt;
		pose = debug_pose;
		vel.v() = debug_vel;

		hpLocal->UpdateHumanInterface(pose, vel);
	}
}


}