/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
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
