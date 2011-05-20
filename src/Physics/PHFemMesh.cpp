/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#include "PHFemMesh.h"
#pragma hdrstop
#include <float.h>

using namespace PTM;
namespace Spr{;

///////////////////////////////////////////////////////////////////
//	PHFemMesh
PHFemMesh::PHFemMesh(const PHFemMeshDesc& desc, SceneIf* s):PHFemMeshDesc(desc){
	if (s){ SetScene(s); }
}

}
