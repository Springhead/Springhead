#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHBallJoint
//OBJECTIMP(PHBallJoint, PHJoint1D)
IF_IMP(PHBallJoint, PHJoint1D)
void PHBallJoint::CompDof(){
	dim_v = 3;
	dim_w = 0;
	dim_q = 0;
}

}