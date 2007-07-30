/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CREyeController.h"

#define CREYECTRL_DEBUG true

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRNeckController, SceneObject);

void CRNeckController::LookAt(Vec3f pos, Vec3f vel){
	this->pos = pos;
	this->vel = vel;
}

void CRNeckController::Step(){
	Quaterniond qLook;
	qLook.RotationArc(Vec3d(0,0,-1), pos.unitize());
	std::cout << "qLook : " << qLook << std::endl;
}
}