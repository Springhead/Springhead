/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*
#include "CRSimple.h"
#include <Foundation/Scene.h>
#include <Foundation/Object.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	CRSimple
IF_IMP(CRSimple, PHEngine);
OBJECT_IMP(CRSimple, PHEngine);

CRSimple::CRSimple() : desc(d), solid(NULL) {
}

void CRSimple::Step(){
	if(!solid) {
		this->GetScene()->FindObject(solid, desc.solidname);
		std::cout << "Solid : " << solid->GetName() << std::endl;
	}
	solid->AddTorque(Vec3d(0.0, ((double)desc.torque), 0.0));
}

}
*/

