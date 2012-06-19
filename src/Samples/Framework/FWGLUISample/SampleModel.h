/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SAMPLE_MODEL_H
#define SAMPLE_MODEL_H

#include <Springhead.h>

using namespace Spr;

PHSolidIf* CreateBox(FWSdkIf* fwSdk);
PHSolidIf* CreateSphere(FWSdkIf* fwSdk);
PHSolidIf* CreateCapsule(FWSdkIf* fwSdk);
PHSolidIf* CreateRoundCone(FWSdkIf* fwSdk);

#endif 