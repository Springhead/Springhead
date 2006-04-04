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

void PHJoint1D::Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc){
	PHConstraint::Init(lhs, rhs, desc);
	const PHJoint1DDesc& desc1D = (const PHJoint1DDesc&)desc;
	lower = desc1D.lower;
	upper = desc1D.upper;
	spring = desc1D.spring;
	origin = desc1D.origin;
	damper = desc1D.damper;
	torque = desc1D.torque;
}
	
}