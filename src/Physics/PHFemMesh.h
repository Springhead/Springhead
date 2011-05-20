/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFEMMESH_H
#define PHFEMMESH_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>

namespace Spr{;

///	有限要素法シミュレーションのためのメッシュ(4面体メッシュ)
class PHFemMesh: public SceneObject, public PHFemMeshDesc {
public:
	SPR_OBJECTDEF(PHFemMesh);
	PHFemMesh(const PHFemMeshDesc& desc=PHFemMeshDesc(), SceneIf* s=NULL);
};


}	//	namespace Spr
#endif
