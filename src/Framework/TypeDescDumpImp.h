//	Do not edit. MakeTypeDesc.bat will update this file.
	
	FWAppGLDesc* pFWAppGLDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWAppGLDesc");
	desc->size = sizeof(FWAppGLDesc);
	desc->ifInfo = FWAppGLIf::GetIfInfoStatic();
	((IfInfo*)FWAppGLIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<FWAppGLDesc>;
	db->RegisterDesc(desc);
	
	FWObjectDesc* pFWObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWObjectDesc");
	desc->size = sizeof(FWObjectDesc);
	desc->ifInfo = FWObjectIf::GetIfInfoStatic();
	((IfInfo*)FWObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<FWObjectDesc>;
	field = desc->AddField("", "string", "phSolidName", "");
	field->offset = (char*)&(pFWObjectDesc->phSolidName) - (char*)pFWObjectDesc;
	field = desc->AddField("", "string", "grFrameName", "");
	field->offset = (char*)&(pFWObjectDesc->grFrameName) - (char*)pFWObjectDesc;
	db->RegisterDesc(desc);
	
	FWSceneDesc* pFWSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWSceneDesc");
	desc->size = sizeof(FWSceneDesc);
	desc->ifInfo = FWSceneIf::GetIfInfoStatic();
	((IfInfo*)FWSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<FWSceneDesc>;
	db->RegisterDesc(desc);
