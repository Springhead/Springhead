/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRSensor.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
void CRSensor::Init(){
	phScene = DCAST(PHSceneIf, GetScene());
}
	
void CRSensor::Step(){
}
}
