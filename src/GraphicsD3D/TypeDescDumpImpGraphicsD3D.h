//	Do not edit. MakeTypeDesc.bat will update this file.
	
	GRSdkD3DDesc* pGRSdkD3DDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRSdkD3DDesc");
	desc->size = sizeof(GRSdkD3DDesc);
	desc->ifInfo = GRSdkD3DIf::GetIfInfoStatic();
	((IfInfo*)GRSdkD3DIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<GRSdkD3DDesc>;
	db->RegisterDesc(desc);
