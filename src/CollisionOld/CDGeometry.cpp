#include "Collision.h"
#pragma hdrstop
namespace Spr {;

SGOBJECTIMPABST(CDGeometry, SGObject);
bool CDGeometry::AddChildObject(SGObject* o, SGScene* s){
	CDPhysicalMaterial* pmat = DCAST(CDPhysicalMaterial, o);
	if (pmat) pmaterial = pmat;
	return pmat != NULL;
}
SGObject* CDGeometry::ChildObject(size_t i){
	return i==0 ? &*pmaterial : NULL; 
}

}
