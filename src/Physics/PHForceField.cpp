/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "PHForceField.h"

using namespace std;
namespace Spr{

OBJECT_IMP(PHGravityEngine, PHEngine);

void PHGravityEngine::Step(){
	PHSolids::iterator it;
	PHSolid* solid;
	for(it = solids.begin(); it != solids.end(); it++){
		solid = *it;
		if(solid->gravity)
			solid->AddForce((float)solid->GetMass() * accel);
	}
}

bool PHGravityEngine::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s){
		solids.push_back(s);
		return true;
	}
	return false;
}
<<<<<<< .mine

void PHGravityEngine::RemoveSolid(PHSolid* solid){
	PHSolids::iterator it;

	for(it = solids.begin(); it != solids.end(); it++){
		if((*it) == solid){
			solids.erase((it));
			return;
		}
	}
=======
>>>>>>> .r2206
}
<<<<<<< .mine

}
=======
>>>>>>> .r2206
