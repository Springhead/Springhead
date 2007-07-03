/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CREyeController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

IF_OBJECT_IMP(CREyeController, SceneObject);

void CREyeController::SetLeftEyeSolid(PHSolidIf* eyeSolid){
	soLeftEye = eyeSolid;
}

void CREyeController::SetRightEyeSolid(PHSolidIf* eyeSolid){
	soRightEye = eyeSolid;
}

void CREyeController::Step(){
}

}