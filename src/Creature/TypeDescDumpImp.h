//	Do not edit. MakeTypeDesc.bat will update this file.
	
	CRSimpleDesc* pCRSimpleDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRSimpleDesc");
	desc->size = sizeof(CRSimpleDesc);
	desc->ifInfo = CRSimpleIf::GetIfInfoStatic();
	((IfInfo*)CRSimpleIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<CRSimpleDesc>;
	field = desc->AddField("", "string", "solidname", "");
	field->offset = (char*)&(pCRSimpleDesc->solidname) - (char*)pCRSimpleDesc;
	field = desc->AddField("", "int", "torque", "");
	field->offset = (char*)&(pCRSimpleDesc->torque) - (char*)pCRSimpleDesc;
	db->RegisterDesc(desc);
