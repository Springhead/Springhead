//	Do not edit. MakeTypeDesc.bat will update this file.
	
	CRSimpleDesc* pCRSimpleDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CRSimpleDesc");
	desc->size = sizeof(CRSimpleDesc);
	desc->ifInfo = CRSimpleIf::GetIfInfoStatic();
	((IfInfo*)CRSimpleIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<CRSimpleDesc>;
	field = desc->AddField("", "int", "test", "");
	field->offset = (char*)&(pCRSimpleDesc->test) - (char*)pCRSimpleDesc;
	db->RegisterDesc(desc);
