#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

//----------------------------------------------------------------------------
//	CDShape
IF_OBJECT_IMP_ABST(CDShape, NamedObject);
double CDShape::epsilon = 1e-8;

}
