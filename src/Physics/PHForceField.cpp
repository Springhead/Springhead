/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHForceField.h>

using namespace std;
namespace Spr{

void PHGravityEngine::Step(){
	PHSolids::iterator it;
	PHSolid* solid;
	for(it = solids.begin(); it != solids.end(); it++){
		solid = *it;
		PHHapticPointerIf* hp = solid->Cast();
		if (!hp) {	//	力覚ポインタには重力は加えない方が良いと思う(hase)
			solid->AddForce((float)solid->GetMass() * accel);
		}
	}
}

void PHGravityEngine::Clear(){
	solids.clear();
}

bool PHGravityEngine::AddChildObject(ObjectIf* o){
	PHSolid* s = o->Cast();
	if(s && std::find(solids.begin(), solids.end(), s) == solids.end()){
		solids.push_back(s);
		return true;
	}
	return false;
}

bool PHGravityEngine::DelChildObject(ObjectIf* o){
	PHSolid* s = o->Cast();
	if(s){
		PHSolids::iterator it = std::find(solids.begin(), solids.end(), s);
		if(it != solids.end()){
			solids.erase(it);
			return true;
		}
	}
	return false;
}

}
