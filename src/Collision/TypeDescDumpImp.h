//	Do not edit. MakeTypeDesc.bat will update this file.
	
	PHMaterial* pPHMaterial = NULL;
	desc = DBG_NEW UTTypeDesc("PHMaterial");
	desc->size = sizeof(PHMaterial);
	desc->access = DBG_NEW FIAccess<PHMaterial>;
	field = desc->AddField("", "float", "mu", "");
	field->offset = (char*)&(pPHMaterial->mu) - (char*)pPHMaterial;
	field = desc->AddField("", "float", "mu0", "");
	field->offset = (char*)&(pPHMaterial->mu0) - (char*)pPHMaterial;
	field = desc->AddField("", "float", "e", "");
	field->offset = (char*)&(pPHMaterial->e) - (char*)pPHMaterial;
	db->RegisterDesc(desc);
	
	CDShapeDesc* pCDShapeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDShapeDesc");
	desc->size = sizeof(CDShapeDesc);
	desc->ifInfo = CDShapeIf::GetIfInfoStatic();
	desc->access = DBG_NEW FIAccess<CDShapeDesc>;
	field = desc->AddField("ShapeType", "enum", "type",  "");
	field->AddEnumConst("CONVEX");
	field->AddEnumConst("CONVEXMESH");
	field->AddEnumConst("SPHERE");
	field->AddEnumConst("BOX");
	field->AddEnumConst("BOX");
	field->offset = (char*)(&pCDShapeDesc->type) - (char*)pCDShapeDesc;
	db->RegisterDesc(desc);
	
	CDConvexMeshDesc* pCDConvexMeshDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDConvexMeshDesc");
	desc->size = sizeof(CDConvexMeshDesc);
	desc->ifInfo = CDConvexMeshIf::GetIfInfoStatic();
	desc->access = DBG_NEW FIAccess<CDConvexMeshDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = (char*)(CDShapeDesc*)pCDConvexMeshDesc - (char*)pCDConvexMeshDesc;
	field = desc->AddField("vector", "Vec3f", "vertices", "");
	field->offset = (char*)&(pCDConvexMeshDesc->vertices) - (char*)pCDConvexMeshDesc;
	field = desc->AddField("", "PHMaterial", "material", "");
	field->offset = (char*)&(pCDConvexMeshDesc->material) - (char*)pCDConvexMeshDesc;
	db->RegisterDesc(desc);
	
	CDSphereDesc* pCDSphereDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDSphereDesc");
	desc->size = sizeof(CDSphereDesc);
	desc->ifInfo = CDSphereIf::GetIfInfoStatic();
	desc->access = DBG_NEW FIAccess<CDSphereDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = (char*)(CDShapeDesc*)pCDSphereDesc - (char*)pCDSphereDesc;
	field = desc->AddField("", "float", "radius", "");
	field->offset = (char*)&(pCDSphereDesc->radius) - (char*)pCDSphereDesc;
	field = desc->AddField("", "PHMaterial", "material", "");
	field->offset = (char*)&(pCDSphereDesc->material) - (char*)pCDSphereDesc;
	db->RegisterDesc(desc);
	
	CDBoxDesc* pCDBoxDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDBoxDesc");
	desc->size = sizeof(CDBoxDesc);
	desc->ifInfo = CDBoxIf::GetIfInfoStatic();
	desc->access = DBG_NEW FIAccess<CDBoxDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = (char*)(CDShapeDesc*)pCDBoxDesc - (char*)pCDBoxDesc;
	field = desc->AddField("", "Vec3f", "boxsize", "");
	field->offset = (char*)&(pCDBoxDesc->boxsize) - (char*)pCDBoxDesc;
	field = desc->AddField("", "PHMaterial", "material", "");
	field->offset = (char*)&(pCDBoxDesc->material) - (char*)pCDBoxDesc;
	db->RegisterDesc(desc);
