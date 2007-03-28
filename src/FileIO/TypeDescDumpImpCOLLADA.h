//	Do not edit. MakeTypeDescCOLLADA.bat will update this file.
	
	physics_materialCommon* pphysics_materialCommon = NULL;
	desc = DBG_NEW UTTypeDesc("physics_materialCommon");
	desc->size = sizeof(physics_materialCommon);
	desc->access = DBG_NEW UTAccess<physics_materialCommon>;
	field = desc->AddField("", "float", "dynamic_friction", "");
	field->offset = int((char*)&(pphysics_materialCommon->dynamic_friction) - (char*)pphysics_materialCommon);
	field = desc->AddField("", "float", "restitution", "");
	field->offset = int((char*)&(pphysics_materialCommon->restitution) - (char*)pphysics_materialCommon);
	field = desc->AddField("", "float", "static_friction", "");
	field->offset = int((char*)&(pphysics_materialCommon->static_friction) - (char*)pphysics_materialCommon);
	db->RegisterDesc(desc);
	
	physics_material* pphysics_material = NULL;
	desc = DBG_NEW UTTypeDesc("physics_material");
	desc->size = sizeof(physics_material);
	desc->access = DBG_NEW UTAccess<physics_material>;
	field = desc->AddField("", "physics_materialCommon", "technique_common", "");
	field->offset = int((char*)&(pphysics_material->technique_common) - (char*)pphysics_material);
	db->RegisterDesc(desc);
