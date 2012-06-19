/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIFILECOLLADAHANDLER_H
#define FIFILECOLLADAHANDLER_H
#include "FIFile.h"

namespace SprCOLLADA{;
using namespace Spr;

struct physics_materialCommon{
	float dynamic_friction;
	float restitution;
	float static_friction;
};
struct physics_material{
	physics_materialCommon technique_common;
};


}
#endif
