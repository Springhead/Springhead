#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\FileIO\SprFIFile.h"
#include "..\..\include\FileIO\SprFISdk.h"
#include "..\..\src\Base\BaseDebug.h"
#include "..\..\src\Foundation\UTTypeDesc.h"
#include "..\Foundation\AccessDescDumpFoundation.h"
#include "..\Foundation\Foundation.h"
#include "..\Foundation\Object.h"
#include "..\Foundation\Scene.h"
#include "..\Foundation\UTBaseType.h"
#include "..\Foundation\UTDllLoader.h"
#include "..\Foundation\UTDllLoaderImpl.h"
#include "..\Foundation\UTLoadContext.h"
#include "..\Foundation\UTLoadHandler.h"
#include "..\Foundation\UTMMTimer.h"
#include "..\Foundation\UTPath.h"
#include "..\Foundation\UTPreciseTimer.h"
#include "..\Foundation\UTTypeDesc.h"
#include "..\FileIO\AccessDescDumpFileIO.h"
#include "..\FileIO\FIFile.h"
#include "..\FileIO\FIFileCOLLADA.h"
#include "..\FileIO\FIFileCOLLADAHandler.h"
#include "..\FileIO\FIFileX.h"
#include "..\FileIO\FileIO.h"
#include "..\FileIO\FISaveContext.h"
#include "..\FileIO\FISdk.h"
#include "..\FileIO\FISpirit.h"
namespace Spr{

void RegisterTypeDescFileIO(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	SprCOLLADA::physics_materialCommon* pphysics_materialCommon = NULL;
	desc = DBG_NEW UTTypeDesc("physics_materialCommon");
	desc->size = sizeof(SprCOLLADA::physics_materialCommon);
	desc->access = DBG_NEW UTAccess<SprCOLLADA::physics_materialCommon>;
	field = desc->AddField("", "float", "dynamic_friction", "");
	field->offset = int((char*)&(pphysics_materialCommon->dynamic_friction) - (char*)pphysics_materialCommon);
	field = desc->AddField("", "float", "restitution", "");
	field->offset = int((char*)&(pphysics_materialCommon->restitution) - (char*)pphysics_materialCommon);
	field = desc->AddField("", "float", "static_friction", "");
	field->offset = int((char*)&(pphysics_materialCommon->static_friction) - (char*)pphysics_materialCommon);
	db->RegisterDesc(desc);
	SprCOLLADA::physics_material* pphysics_material = NULL;
	desc = DBG_NEW UTTypeDesc("physics_material");
	desc->size = sizeof(SprCOLLADA::physics_material);
	desc->access = DBG_NEW UTAccess<SprCOLLADA::physics_material>;
	field = desc->AddField("", "physics_materialCommon", "technique_common", "");
	field->offset = int((char*)&(pphysics_material->technique_common) - (char*)pphysics_material);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(FIFile, Object);
bool Spr::FIFileIf::Load(Spr::ObjectIfs& objs, const char* fn){
	return	((FIFile*)(Object*)(ObjectIf*)this)->Load(objs, fn);
}
bool Spr::FIFileIf::Save(const Spr::ObjectIfs& objs, const char* fn){
	return	((FIFile*)(Object*)(ObjectIf*)this)->Save(objs, fn);
}
SPR_IFIMP1(FIFileX, FIFile);
bool Spr::FIFileXIf::Load(Spr::ObjectIfs& objs, const char* fn){
	return	((FIFileX*)(Object*)(ObjectIf*)this)->Load(objs, fn);
}
bool Spr::FIFileXIf::Save(const Spr::ObjectIfs& objs, const char* fn){
	return	((FIFileX*)(Object*)(ObjectIf*)this)->Save(objs, fn);
}
SPR_IFIMP1(FIFileCOLLADA, FIFile);
bool Spr::FIFileCOLLADAIf::Load(Spr::ObjectIfs& objs, const char* fn){
	return	((FIFileCOLLADA*)(Object*)(ObjectIf*)this)->Load(objs, fn);
}
bool Spr::FIFileCOLLADAIf::Save(const Spr::ObjectIfs& objs, const char* fn){
	return	((FIFileCOLLADA*)(Object*)(ObjectIf*)this)->Save(objs, fn);
}
SPR_IFIMP1(FISdk, Sdk);
Spr::FIFileXIf* Spr::FISdkIf::CreateFileX(){
	return	((FISdk*)(Object*)(ObjectIf*)this)->CreateFileX();
}
Spr::FIFileCOLLADAIf* Spr::FISdkIf::CreateFileCOLLADA(){
	return	((FISdk*)(Object*)(ObjectIf*)this)->CreateFileCOLLADA();
}
SPR_OBJECTIMP_ABST1(FIFile, Object);
SPR_OBJECTIMP1(FIFileCOLLADA, FIFile);
SPR_OBJECTIMP1(FIFileX, FIFile);
SPR_OBJECTIMP1(FISdk, Sdk);
}
