/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CREngine.h>

using namespace std;

namespace Spr{;

bool CREngine::AddChildObject(ObjectIf* o){
	FWObjectGroupIf* grp = o->Cast();
	if (grp) {
		groups.push_back(grp);
		AssignObject();
		return true;
	}
	return false;
}

ObjectIf* CREngine::GetChildObject(size_t pos){
	if (0<=pos && pos<groups.size()) {
		return DCAST(ObjectIf,groups[pos]);
	}
	return NULL;
}

size_t CREngine::NChildObject() const {
	return groups.size();
}

}
