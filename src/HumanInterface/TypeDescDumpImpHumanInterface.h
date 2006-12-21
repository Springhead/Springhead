//	Do not edit. MakeTypeDesc.bat will update this file.
	
	HISdkDesc* pHISdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("HISdkDesc");
	desc->size = sizeof(HISdkDesc);
	desc->ifInfo = HISdkIf::GetIfInfoStatic();
	((IfInfo*)HISdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<HISdkDesc>;
	db->RegisterDesc(desc);
