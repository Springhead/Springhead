/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#ifdef USE_HDRSTOP
#include <Foundation/Scene.h>
#pragma hdrstop
#endif

namespace Spr{;

PHMaterial::PHMaterial(){
	mu = mu0 = 0;
	e = 0.4f;
}

}
