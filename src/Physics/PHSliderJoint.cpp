#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHSliderJoint
//OBJECTIMP(PHSliderJoint, PHJoint1D)
IF_IMP(PHSliderJoint, PHJoint1D)
void PHSliderJoint::CompDof(){
	dim_v = 2;
	dim_w = 3;
	dim_q = 3;
}

}