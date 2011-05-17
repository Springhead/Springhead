/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWFEMMESH_H
#define FWFEMMESH_H

#include <Springhead.h>
#include <Framework/FWObject.h>

namespace Spr{;

class FWFemMesh: public FWObject, public FWFemMeshDesc{
public:
	SPR_OBJECTDEF(FWFemMesh);
	ACCESS_DESC(FWFemMesh);
	FWFemMesh(const FWFemMeshDesc& d=FWFemMeshDesc());
};

}

#endif
