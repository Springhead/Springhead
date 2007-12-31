/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HumanInterface.h"
#include "HIVirtualDevicePool.h"
#pragma hdrstop

namespace Spr {

HIVirtualDevice* HIVirtualDevicePool::Rent(const char* type, const char* name){
	for(iterator it = begin(); it != end(); ++it){
		if (strcmp((*it)->Type(), type) == 0 && !(*it)->GetUsed()){
			if (!name || strcmp((*it)->Name(), name) == 0){
				(*it)->SetUsed(true);
				return *it;
			}
		}
	}
	return NULL;
}
bool HIVirtualDevicePool::Return(HIVirtualDevice* d){
	assert(d->GetUsed());
	for(iterator it = begin(); it != end(); ++it){
		if (*it == d){
			d->SetUsed(false);
			return true;
		}
	}
	return false;
}
void HIVirtualDevicePool::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<HIVirtualDevicePool>" << std::endl;
	o.width(w+2);
	for(const_iterator it = begin(); it != end(); ++it){
		o << **it;
	}
	o.width(0);
	o << UTPadding(w) << "</HIVirtualDevicePool>" << std::endl;
	o.width(w);
}

}	//	namespace Spr
