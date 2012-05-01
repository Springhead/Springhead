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
	bForce = false;
	bFriction = true;
	bMultiPoints = true;
	bVibration = false;
	rotaionalWeight = 1e5;
	dynamical = false;
	hapticRenderMode = CONSTRAINT;
}
PHHapticPointer::PHHapticPointer(const PHHapticPointer& p){
	*this = p;
}

void PHHapticPointer::UpdateHumanInterface(Posed pose, SpatialVector vel){
	if(bDebugControl) return;
	// HumanInterfaceから状態を取得
	double s = GetPosScale();
	vel.v() = vel.v() * s;
	pose.Pos() = pose.Pos() * s;
	hiSolid.SetVelocity(vel.v());
	hiSolid.SetAngularVelocity(vel.w());
	hiSolid.SetPose(GetDefaultPose() * pose);
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


}