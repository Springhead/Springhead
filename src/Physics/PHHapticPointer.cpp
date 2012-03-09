#include <Physics/PHHapticPointer.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticPointer
PHHapticPointer::PHHapticPointer(){
	reflexSpring = 300;
	reflexDamper = 0.1f;
	localRange = 1.0; 
	posScale = 1.0;
	bDebugControl = false;
	bDirectControl = false;
	bForce = false;
	bFriction = true;
	bMultiPoints = true;
	bVibration = false;
	rotaionalWeight = 1e5;
	dynamical = false;
	integrate = false;
	hapticRenderMode = CONSTRAINT;
}
PHHapticPointer::PHHapticPointer(const PHHapticPointer& p){
	*this = p;
}

void PHHapticPointer::UpdateInterface(float dt){
	if(bDebugControl) return;
	// HumanInterfaceから状態を取得
	double s = GetPosScale();
	HIHapticIf* hif = DCAST(HIHapticIf, humanInterface);
	hif->Update(dt);
	hiSolid.SetVelocity((Vec3d)hif->GetVelocity() * s);
	hiSolid.SetAngularVelocity((Vec3d)hif->GetAngularVelocity());
	Posed pose;
	pose.Pos() = (Vec3d)hif->GetPosition() * s;
	pose.Ori() =    hif->GetOrientation();
	hiSolid.SetPose(GetDefaultPose() * pose);
	
	if(!bDirectControl) return;
	// デバックのための擬似入力
	Vec3d debug_vel = Vec3d(-0.1, 0.0, 0.0);
	static Posed debug_pose = defaultPose;
	debug_pose.Pos() = debug_pose.Pos() + debug_vel * dt;
	hiSolid.SetVelocity(debug_vel);
	hiSolid.SetPose(debug_pose);
}

void PHHapticPointer::UpdateDirect(){
	// ポインタの状態更新
	SetVelocity(hiSolid.GetVelocity());
	SetAngularVelocity(hiSolid.GetAngularVelocity());
	SetFramePosition(hiSolid.GetFramePosition());
	SetOrientation(hiSolid.GetOrientation());
	
	// プロキシの状態の保存と更新
	lastProxyPose = proxyPose;
	proxyPose = GetPose();
	targetProxy = GetPose();
}

void PHHapticPointer::AddHapticForce(SpatialVector f){
	hapticForce += f;
}

void PHHapticPointer::DisplayHapticForce(){
	HIHapticIf* hif = DCAST(HIHapticIf, humanInterface);
	if(bForce)
		 hif->SetForce(hapticForce.v(), hapticForce.w());
	else hif->SetForce(Vec3f(), Vec3f());
	//CSVOUT << hapticForce.v().x << std::endl;
	hapticForce.clear(0.0);
}


}