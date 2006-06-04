#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;

SGIF_OBJECT_IMPABST(CDGeometry, SGObject);
bool CDGeometry::AddChildObject(SGObject* o, SGScene* s){
	CDPhysicalMaterial* pmat = DCAST(CDPhysicalMaterial, o);
	if (pmat) pmaterial = pmat;
	return pmat != NULL;
}
SGObject* CDGeometry::ChildObject(size_t i){
	return i==0 ? &*pmaterial : NULL; 
}

}
