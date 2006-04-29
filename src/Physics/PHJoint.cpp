#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHJoint
IF_IMP(PHJoint, PHConstraint);

//----------------------------------------------------------------------------
// PHJoint1D
IF_IMP(PHJoint1D, PHJoint);

void PHJoint1D::SetDesc(const PHJointDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHJoint1DDesc& desc1D = (const PHJoint1DDesc&)desc;
	lower = desc1D.lower;
	upper = desc1D.upper;
	spring = desc1D.spring;
	origin = desc1D.origin;
	damper = desc1D.damper;
	SetMotorTorque(desc1D.torque);
}

void PHJoint1D::CompDof(){
	on_lower = on_upper = false;
	if(lower < upper){
		double theta = GetPosition();
		on_lower = (theta <= lower);
		on_upper = (theta >= upper);
	}
	if(on_lower || on_upper || mode == MODE_VELOCITY || spring != 0.0 || damper != 0.0){
		dim_d = 6;
		dim_c = 5;
	}
	else{
		dim_d = 5;
		dim_c = 5;
	}
}
	
}
