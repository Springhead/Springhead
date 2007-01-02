//	Do not edit. MakeTypeDesc.bat will update this file.
	
	DRUsb20SimpleDesc* pDRUsb20SimpleDesc = NULL;
	desc = DBG_NEW UTTypeDesc("DRUsb20SimpleDesc");
	desc->size = sizeof(DRUsb20SimpleDesc);
	desc->ifInfo = DRUsb20SimpleIf::GetIfInfoStatic();
	((IfInfo*)DRUsb20SimpleIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<DRUsb20SimpleDesc>;
	field = desc->AddField("", "int", "number", "");
	field->offset = int((char*)&(pDRUsb20SimpleDesc->number) - (char*)pDRUsb20SimpleDesc);
	db->RegisterDesc(desc);
	
	HISdkDesc* pHISdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("HISdkDesc");
	desc->size = sizeof(HISdkDesc);
	desc->ifInfo = HISdkIf::GetIfInfoStatic();
	((IfInfo*)HISdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<HISdkDesc>;
	db->RegisterDesc(desc);
