/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
//	Do not edit. MakeTypeDesc.bat will update this file.
	
	FIInactiveSolids* pFIInactiveSolids = NULL;
	desc = DBG_NEW UTTypeDesc("FIInactiveSolids");
	desc->size = sizeof(FIInactiveSolids);
	desc->access = DBG_NEW UTAccess<FIInactiveSolids>;
	field = desc->AddField("vector", "string", "solids", "");
	field->offset = (char*)&(pFIInactiveSolids->solids) - (char*)pFIInactiveSolids;
	db->RegisterDesc(desc);
