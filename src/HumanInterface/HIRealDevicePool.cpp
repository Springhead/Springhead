/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HumanInterface.h"
#include "HIRealDevicePool.h"

namespace Spr {


HIRealDevicePool::HIRealDevicePool(){
}
HIRealDevicePool::~HIRealDevicePool(){
}
///	“o˜^
void HIRealDevicePool::Register(HIRealDevice* dev){
	push_back(dev);
}
void HIRealDevicePool::Init(HISdkIf* sdk){
	for(iterator it = begin(); it != end(); ++it){
		if ((*it)->Init()) (*it)->Register(sdk);
	}
}
void HIRealDevicePool::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<HIRealDevicePool>" << std::endl;
	o.width(w+2);
	for(const_iterator it = begin(); it != end(); ++it){
		o << **it;
	}
	o.width(0);
	o << UTPadding(w) << "</HIRealDevicePool>" << std::endl;
	o.width(w);
}

}	//	namespace Spr
