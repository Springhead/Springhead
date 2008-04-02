/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

void PHGravityEngine::Step(){
	PHSolids::iterator it;
	PHSolid* solid;
	for(it = solids.begin(); it != solids.end(); it++){
		solid = *it;
		solid->AddForce((float)solid->GetMass() * accel);
	}
}

bool PHGravityEngine::AddChildObject(ObjectIf* o){
	PHSolid* s = DCAST(PHSolid, o);
	if(s && !solids.Find(s)){
		solids.push_back(s);
		return true;
	}
	return false;
}

}
