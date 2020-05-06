/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
//#include <Springhead.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/HumanInterface/DRTokin3D.cpp"
#else
#include <HumanInterface/DRTokin3D.h>
namespace Spr {;

DRTokin3D::DRTokin3D(){}

DRTokin3D::~DRTokin3D(){}

bool DRTokin3D::Init(){
	SetName("Tokin 3D motion sensor");
	return false;
}
void DRTokin3D::GetMatrix(Matrix3f& ori){}
Vec3f DRTokin3D::GetEuler(){return Vec3f(-FLT_MAX,-FLT_MAX,-FLT_MAX);}
int DRTokin3D::GetPort(int ch){return -1;}
}	//	namespace Spr
#endif
