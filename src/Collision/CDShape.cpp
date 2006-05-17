#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

//----------------------------------------------------------------------------
//	CDShape

IF_IMP(CDShape, Object);
OBJECTIMPABST(CDShape, NamedObject);
double CDShape::epsilon = 1e-5;

}
