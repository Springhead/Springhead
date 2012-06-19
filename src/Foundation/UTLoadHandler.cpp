/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "UTLoadHandler.h"
#include "UTLoadContext.h"
namespace Spr{;

UTRef<UTLoadHandlerDbPool> UTLoadHandlerDbPool::pool;
UTLoadHandlerDbPool* SPR_CDECL UTLoadHandlerDbPool::GetPool(){
	if (!pool) pool = DBG_NEW UTLoadHandlerDbPool;
	return pool;
}

UTLoadHandlerDb* SPR_CDECL UTLoadHandlerDbPool::Get(const char* gp){
	UTRef<UTLoadHandlerDb> key = DBG_NEW UTLoadHandlerDb;
	key->group=gp;
	std::pair<UTLoadHandlerDbPool::iterator, bool> r = GetPool()->insert(key);
	return *r.first;
}

}
