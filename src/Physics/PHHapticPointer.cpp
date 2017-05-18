#include <Physics/PHHapticPointer.h>
#include <Physics/PHHapticEngine.h>

namespace Spr{;

PHSpringDamperCoeff::PHSpringDamperCoeff() {
	spring = 300;
	damper = 0.1f;
	rotationSpring = 4.0f;
	rotationDamper = 0.001f;
};
PHSpringDamperCoeff operator * (float s, PHSpringDamperCoeff c) {
	PHSpringDamperCoeff rv = c;
	rv.spring *= s;
	rv.damper *= s;
	rv.rotationSpring *= s;
	rv.rotationDamper *= s;
	return rv;
}
PHSpringDamperCoeff operator * (PHSpringDamperCoeff c, float s) {
	return s*c;
}

//----------------------------------------------------------------------------
// PHHapticPointer
PHHapticPointer::PHHapticPointer(){
	localRange = 1.0; 
	posScale = 1.0;
	bForce = false;
	bFriction = true;
	bTimeVaryFriction = true;
	bMultiPoints = true;
	bVibration = false;
	rotaionalWeight = 1e5;
	dynamical = false;
	renderMode = CONSTRAINT;
	bLastContactState = false;
}

float PHHapticPointer::GetContactForce(int i){
		float rv = 0.0f;
		int j = neighborSolidIDs[i];
		PHHapticEngine* he = DCAST(PHSceneIf,GetScene())->GetHapticEngine()->Cast();
		PHSolidPairForHaptic* sop = he->GetSolidPairTemp(j,0);
		for (int m=0; m<sop->solid[0]->NShape(); ++m) {
			for (int n=0; n<sop->solid[0]->NShape(); ++n) {
				PHShapePairForHaptic* shp = sop->GetShapePair(m,n);
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
//	hiSolidSt.angVelocity = vel.w();
//	hiSolidSt.pose = GetDefaultPose() * Posed(s * pose.Pos(), pose.Ori());
	hiSolidSt.pose.Pos() = GetDefaultPose() * (s * pose.Pos());
}
SpatialVector PHHapticPointer::GetHapticForce() {
	SpatialVector rv = hapticForce;
	rv.v() = GetDefaultPose().Ori().Inv() * rv.v();
	rv.w() = GetDefaultPose().Ori().Inv() * rv.w();
	return rv;
}

void PHHapticPointer::AddHapticForce(const SpatialVector& f){
	hapticForce += f;
}


}