#include <Physics/PHHapticPointer.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;
PHHapticPointerDesc::PHHapticPointerDesc() {
	renderMode = CONSTRAINT;
}

PHHapticPointerHapticSt::PHHapticPointerHapticSt() {
}
PHHapticPointerPhysicsSt::PHHapticPointerPhysicsSt() {
	bRotation = true;
	bForce = false;
	bFriction = true;
	bTimeVaryFriction = false;
	bMultiPoints = false;
	bVibration = false;
	posScale = 1.0;
	localRange = 1.0;
	frictionSpring = 400.0f;
	reflexSpring = 1000.0f;
	reflexDamper = 0.0f;
	rotationReflexSpring = 10.0f;
	rotationReflexDamper = 0.0f;
	rotaionalWeight = 1e5;

	//GMS用
	bSimulation = false;
	bMultiproxy = false;
	proxyN = 3;
	totalSlipState = 0;
}

//----------------------------------------------------------------------------
// PHHapticPointer
PHHapticPointer::PHHapticPointer(){
	SetDynamical(false);
	renderMode = CONSTRAINT;
}

float PHHapticPointer::GetContactForce(int i){
		float rv = 0.0f;
		int j = neighborSolidIDs[i];
		PHHapticEngine* he = DCAST(PHSceneIf,GetScene())->GetHapticEngine()->Cast();
		PHSolidPairForHaptic* sop = he->GetSolidPairTemp(j,0)->Cast();
		for (int m=0; m<sop->body[0]->NShape(); ++m) {
			for (int n=0; n<sop->body[0]->NShape(); ++n) {
				PHShapePairForHaptic* shp = sop->GetShapePair(m,n)->Cast();
				Vec3d p0 = (shp->shapePoseW[0]*shp->closestPoint[0]);
				Vec3d p1 = (shp->shapePoseW[1]*shp->closestPoint[1]);
				Vec3d di = (p0-p1);
				if (di.norm()!=0  &&  PTM::dot(di.unit(), shp->normal.unit()) > 0) {
					// 仮実装<!!>
					// これはdepthであってforceではない．
					// あとでちゃんと力が取得できるようにすること． mitake
					rv += di.norm();
				}
			}
		}
		return rv;
}

void PHHapticPointer::UpdateHumanInterface(const Posed& pose, const SpatialVector& vel){
	// HumanInterfaceから状態を取得
	double s = GetPosScale();
	hiSolidSt.velocity = s * vel.v();
	if (bRotation) {
		hiSolidSt.angVelocity = vel.w();
		hiSolidSt.pose = GetDefaultPose() * Posed(s * pose.Pos(), pose.Ori());
	}
	else {
		hiSolidSt.pose.Pos() = GetDefaultPose() * (s * pose.Pos());
	}
}
SpatialVector PHHapticPointer::GetHapticForce() {
	SpatialVector rv = hapticForce;
	rv.v() = GetDefaultPose().Ori().Inv() * rv.v();
	if (bRotation) rv.w() = GetDefaultPose().Ori().Inv() * rv.w();
	return rv;
}

void PHHapticPointer::AddHapticForce(const SpatialVector& f){
	hapticForce += f;
}
void	PHHapticPointer::ClearHapticForce() {
	hapticForce.clear();
}


}