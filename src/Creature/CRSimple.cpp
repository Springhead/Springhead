/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Springhead.h>
#include "CRSimple.h"
#include <Foundation/Object.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	CRSimple
IF_OBJECT_IMP(CRSimple, SceneObject);
//IF_IMP(CRSimple, SceneObject);

void CRSimple::Hoge(){
}

void CRSimple::Step(){
	std::cout << "Hoge!Hoge!Hoge!" << std::endl;
}
}
