//	Do not edit. MakeTypeDesc.bat will update this file.
	
	GRAnimationMeshDesc* pGRAnimationMeshDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRAnimationMeshDesc");
	desc->size = sizeof(GRAnimationMeshDesc);
	desc->ifInfo = GRAnimationMeshIf::GetIfInfoStatic();
	((IfInfo*)GRAnimationMeshIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<GRAnimationMeshDesc>;
	field = desc->AddField("", "string", "filename", "");
	field->offset = int((char*)&(pGRAnimationMeshDesc->filename) - (char*)pGRAnimationMeshDesc);
	db->RegisterDesc(desc);
	
	GRAnimationMeshBlendData* pGRAnimationMeshBlendData = NULL;
	desc = DBG_NEW UTTypeDesc("GRAnimationMeshBlendData");
	desc->size = sizeof(GRAnimationMeshBlendData);
	desc->access = DBG_NEW UTAccess<GRAnimationMeshBlendData>;
	field = desc->AddField("", "string", "name", "");
	field->offset = int((char*)&(pGRAnimationMeshBlendData->name) - (char*)pGRAnimationMeshBlendData);
	field = desc->AddField("", "double", "time", "");
	field->offset = int((char*)&(pGRAnimationMeshBlendData->time) - (char*)pGRAnimationMeshBlendData);
	field = desc->AddField("", "float", "weight", "");
	field->offset = int((char*)&(pGRAnimationMeshBlendData->weight) - (char*)pGRAnimationMeshBlendData);
	db->RegisterDesc(desc);
	
	GRSdkD3DDesc* pGRSdkD3DDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRSdkD3DDesc");
	desc->size = sizeof(GRSdkD3DDesc);
	desc->ifInfo = GRSdkD3DIf::GetIfInfoStatic();
	((IfInfo*)GRSdkD3DIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<GRSdkD3DDesc>;
	db->RegisterDesc(desc);
