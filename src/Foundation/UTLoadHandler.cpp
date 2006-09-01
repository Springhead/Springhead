/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "UTLoadHandler.h"
#include "UTLoadContext.h"
namespace Spr{;

UTRef<UTLoadHandlerDb> UTLoadHandlerDb::handlerDb;
UTLoadHandlers* SPR_CDECL UTLoadHandlerDb::GetHandlers(const char* gp){
	if (!handlerDb) handlerDb = DBG_NEW UTLoadHandlerDb;
	return handlerDb->GetHandlersImp(gp);
}

UTLoadHandlers* UTLoadHandlerDb::GetHandlersImp(const char* gp){
	UTRef<UTLoadHandlers> key = DBG_NEW UTLoadHandlers;
	key->group = gp;
	std::pair<iterator, bool> r = insert(key);
	return *r.first;
}
}
