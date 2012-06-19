/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDShape.h>

namespace Spr{;

PHMaterial::PHMaterial(){
	mu = 0.4f;
	mu0 = 0.4f;
	e = 0.4f;
	density = 1.0f;

	reflexSpring = 5.0f;
	reflexDamper = 0.1f;
	frictionSpring = 5.0;
	frictionDamper = 0.1f;
	vibA = -30;
	vibB = 90;
	vibW = 300;
	vibT = 0;
	vibContact = false;
}

}
