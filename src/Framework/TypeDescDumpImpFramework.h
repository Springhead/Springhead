//	Do not edit. MakeTypeDesc.bat will update this file.
	
	FWObjectDesc* pFWObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWObjectDesc");
	desc->size = sizeof(FWObjectDesc);
	desc->ifInfo = FWObjectIf::GetIfInfoStatic();
	((IfInfo*)FWObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<FWObjectDesc>;
	db->RegisterDesc(desc);
	
	FWSceneDesc* pFWSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWSceneDesc");
	desc->size = sizeof(FWSceneDesc);
	desc->ifInfo = FWSceneIf::GetIfInfoStatic();
	((IfInfo*)FWSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<FWSceneDesc>;
	db->RegisterDesc(desc);
	
	FWSdkDesc* pFWSdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWSdkDesc");
	desc->size = sizeof(FWSdkDesc);
	desc->ifInfo = FWSdkIf::GetIfInfoStatic();
	((IfInfo*)FWSdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<FWSdkDesc>;
	db->RegisterDesc(desc);
