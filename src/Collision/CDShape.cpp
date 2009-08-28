/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#include <Foundation/Scene.h>
#pragma hdrstop

namespace Spr{;

PHMaterial::PHMaterial(){
	mu = 0.4f;
	mu0 = 0.4f;
	e = 0.4f;
	density = 1.0f;

	springK = 20;
	damperD = 0.0001f;
	vibA = 0;
	vibB = 0;
	vibW = 0;
	vibT = 0;
	vibContact = false;
}

}
