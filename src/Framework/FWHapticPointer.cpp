#include "Framework/FWHapticPointer.h"
#include "Physics/PHHapticPointer.h"

namespace Spr{;

void FWHapticPointer::UpdateHumanInterface(PHHapticPointer* hpLocal, float dt){
	HIHapticIf* hif = DCAST(HIHapticIf, humanInterface);
	// 力の出力
	hapticForce  = hpLocal->GetHapticForce();
	if(hpLocal->bForce){
		hif->SetForce(hapticForce.v(), hapticForce.w());
	}else{
		hif->SetForce(Vec3f(), Vec3f());
	}

	// インタフェースの状態更新
	hif->Update(dt);
	SpatialVector vel;
	vel.v() = (Vec3d)hif->GetVelocity();
	vel.w() = (Vec3d)hif->GetAngularVelocity();
	Posed pose = hif->GetPose();

	//// デバックのための擬似入力
	//Vec3d debug_vel = Vec3d(-0.001, 0.0, 0.0);
	//static Posed debug_pose = Posed();
	//debug_pose.Pos() = debug_pose.Pos() + debug_vel * dt;
	//pose = debug_pose;
	//vel.v() = debug_vel;

	hpLocal->UpdateHumanInterface(pose, vel);
}


}