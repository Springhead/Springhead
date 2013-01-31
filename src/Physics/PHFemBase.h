/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef PH_FEM_BASE_NEW_H
#define PH_FEM_BASE_NEW_H

#include "Physics/PHFemMeshNew.h"
#include "Foundation/Object.h"
#include "PHScene.h"

namespace Spr{;

/// åvéZÉÇÉWÉÖÅ[ÉãÇÃã§í ïîï™
class PHFemMeshNew;
class PHFem: public SceneObject{
public:
	SPR_OBJECTDEF_ABST(PHFem);
	SPR_DECLMEMBEROF_PHFemDesc;
protected:
	UTRef< PHFemMeshNew > phFemMesh;
public:
	PHFem(){}
	virtual void Init(){}
	virtual void Step(){}

	void SetPHFemMesh(PHFemMeshNew* m);
	PHFemMeshNew* GetPHFemMesh();
	int NVertices();
};

}

#endif