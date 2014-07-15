/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRGazeController.h>

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRGazeController::Init(){
}

void CRGazeController::Step(){
}

void CRGazeController::Reset(){
}

int CRGazeController::GetStatus(){
	return CRControllerDesc::CS_WAITING;
}

void CRGazeController::SetTargetPosition(Vec3d pos){
}

Vec3d CRGazeController::GetTargetPosition(){
	return Vec3d();
}
}
