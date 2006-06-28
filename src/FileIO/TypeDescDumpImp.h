//	Do not edit. MakeTypeDesc.bat will update this file.
	
	FIInactiveSolids* pFIInactiveSolids = NULL;
	desc = DBG_NEW UTTypeDesc("FIInactiveSolids");
	desc->size = sizeof(FIInactiveSolids);
	desc->access = DBG_NEW UTAccess<FIInactiveSolids>;
	field = desc->AddField("vector", "string", "solids", "");
	field->offset = (char*)&(pFIInactiveSolids->solids) - (char*)pFIInactiveSolids;
	db->RegisterDesc(desc);
