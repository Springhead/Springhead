/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemMeshThermo.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

PHFemMeshThermoDesc::PHFemMeshThermoDesc(){
	Init();
}
void PHFemMeshThermoDesc::Init(){	
}

///////////////////////////////////////////////////////////////////
//	PHFemMeshThermo


PHFemMeshThermo::PHFemMeshThermo(const PHFemMeshThermoDesc& desc, SceneIf* s){
	SetDesc(&desc);
	if (s){ SetScene(s); }
}

void PHFemMeshThermo::SetDesc(const void* p) {
	PHFemMeshThermoDesc* d = (PHFemMeshThermoDesc*)p;
	PHFemMesh::SetDesc(d);
}

bool PHFemMeshThermo::GetDesc(void* p) const {
	PHFemMeshThermoDesc* d = (PHFemMeshThermoDesc*)p;
	return PHFemMesh::GetDesc(d);
}

}
