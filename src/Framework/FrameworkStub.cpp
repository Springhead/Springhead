#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Foundation\SprUTTimer.h"
#include "..\..\include\Framework\SprFWApp.h"
#include "..\..\include\Framework\SprFWBone.h"
#include "..\..\include\Framework\SprFWEditor.h"
#include "..\..\include\Framework\SprFWFemMesh.h"
#include "..\..\include\Framework\SprFWGraphicsAdaptee.h"
#include "..\..\include\Framework\SprFWInteractPointer.h"
#include "..\..\include\Framework\SprFWInteractScene.h"
#include "..\..\include\Framework\SprFWObject.h"
#include "..\..\include\Framework\SprFWScene.h"
#include "..\..\include\Framework\SprFWSdk.h"
#include "..\..\include\Framework\SprFWWin.h"
#include "..\..\include\Base\BaseDebug.h"
#include "..\..\src\Foundation\UTTypeDesc.h"
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
#include "..\Foundation\UTQPTimer.h"
#include "..\Foundation\UTTimer.h"
#include "..\Foundation\UTTypeDesc.h"
#include "..\Framework\Framework.h"
#include "..\Framework\FWBone.h"
#include "..\Framework\FWFemMesh.h"
#include "..\Framework\FWGLUI.h"
#include "..\Framework\FWGLUT.h"
#include "..\Framework\FWGrabCoupling.h"
#include "..\Framework\FWGraphicsAdaptee.h"
#include "..\Framework\FWImpulseHaptic.h"
#include "..\Framework\FWInteractAdaptee.h"
#include "..\Framework\FWInteractInfo.h"
#include "..\Framework\FWInteractPointer.h"
#include "..\Framework\FWInteractScene.h"
#include "..\Framework\FWInteractSolid.h"
#include "..\Framework\FWLDHaptic.h"
#include "..\Framework\FWLDHaptic3D.h"
#include "..\Framework\FWLDHaptic6D.h"
#include "..\Framework\FWMultiRateHaptic.h"
#include "..\Framework\FWObject.h"
#include "..\Framework\FWOldSpringheadNode.h"
#include "..\Framework\FWOldSpringheadNodeHandler.h"
#include "..\Framework\FWScene.h"
#include "..\Framework\FWSdk.h"
#include "..\Framework\FWSprTetgen.h"
#include "..\Framework\FWVirtualCoupling.h"
#include "..\Framework\FWWin.h"
namespace Spr{

void RegisterTypeDescFramework(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::FWBoneDesc* pFWBoneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWBoneDesc");
	desc->size = sizeof(Spr::FWBoneDesc);
	desc->ifInfo = FWBoneIf::GetIfInfoStatic();
	((IfInfo*)FWBoneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWBoneDesc>;
	db->RegisterDesc(desc);
	Spr::FWFemMeshDesc* pFWFemMeshDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWFemMeshDesc");
	desc->size = sizeof(Spr::FWFemMeshDesc);
	desc->ifInfo = FWFemMeshIf::GetIfInfoStatic();
	((IfInfo*)FWFemMeshIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWFemMeshDesc>;
	field = desc->AddBase("FWObjectDesc");
	field->offset = int((char*)(FWObjectDesc*)pFWFemMeshDesc - (char*)pFWFemMeshDesc);
	db->RegisterDesc(desc);
	Spr::FWGraphicsAdapteeDesc* pFWGraphicsAdapteeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWGraphicsAdapteeDesc");
	desc->size = sizeof(Spr::FWGraphicsAdapteeDesc);
	desc->ifInfo = FWGraphicsAdapteeIf::GetIfInfoStatic();
	((IfInfo*)FWGraphicsAdapteeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWGraphicsAdapteeDesc>;
	db->RegisterDesc(desc);
	Spr::FWInteractPointerDesc* pFWInteractPointerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWInteractPointerDesc");
	desc->size = sizeof(Spr::FWInteractPointerDesc);
	desc->ifInfo = FWInteractPointerIf::GetIfInfoStatic();
	((IfInfo*)FWInteractPointerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWInteractPointerDesc>;
	field = desc->AddField("pointer", "PHSolidIf", "pointerSolid", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->pointerSolid) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("pointer", "HIBaseIf", "humanInterface", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->humanInterface) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "springK", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->springK) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "damperD", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->damperD) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "springOriK", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->springOriK) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "damperOriD", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->damperOriD) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "Posed", "defaultPosition", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->defaultPosition) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "Posed", "pointersCalibPosition", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->pointersCalibPosition) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "posScale", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->posScale) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "worldScale", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->worldScale) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "forceScale", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->forceScale) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "torqueScale", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->torqueScale) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "double", "localRange", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->localRange) - (char*)pFWInteractPointerDesc);
	field = desc->AddField("", "Quaterniond", "cameraOri", "");
	field->offset = int((char*)&(pFWInteractPointerDesc->cameraOri) - (char*)pFWInteractPointerDesc);
	db->RegisterDesc(desc);
	Spr::FWInteractSceneDesc* pFWInteractSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWInteractSceneDesc");
	desc->size = sizeof(Spr::FWInteractSceneDesc);
	desc->ifInfo = FWInteractSceneIf::GetIfInfoStatic();
	((IfInfo*)FWInteractSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWInteractSceneDesc>;
	field = desc->AddField("pointer", "FWSceneIf", "fwScene", "");
	field->offset = int((char*)&(pFWInteractSceneDesc->fwScene) - (char*)pFWInteractSceneDesc);
	field = desc->AddField("", "FWInteractMode", "iaMode", "");
	field->offset = int((char*)&(pFWInteractSceneDesc->iaMode) - (char*)pFWInteractSceneDesc);
	field = desc->AddField("", "FWHapticMode", "hMode", "");
	field->offset = int((char*)&(pFWInteractSceneDesc->hMode) - (char*)pFWInteractSceneDesc);
	field = desc->AddField("", "double", "hdt", "");
	field->offset = int((char*)&(pFWInteractSceneDesc->hdt) - (char*)pFWInteractSceneDesc);
	db->RegisterDesc(desc);
	Spr::FWObjectDesc* pFWObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWObjectDesc");
	desc->size = sizeof(Spr::FWObjectDesc);
	desc->ifInfo = FWObjectIf::GetIfInfoStatic();
	((IfInfo*)FWObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWObjectDesc>;
	db->RegisterDesc(desc);
	Spr::FWBoneObjectDesc* pFWBoneObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWBoneObjectDesc");
	desc->size = sizeof(Spr::FWBoneObjectDesc);
	desc->ifInfo = FWBoneObjectIf::GetIfInfoStatic();
	((IfInfo*)FWBoneObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWBoneObjectDesc>;
	field = desc->AddField("", "bool", "modifyShapeLength", "");
	field->offset = int((char*)&(pFWBoneObjectDesc->modifyShapeLength) - (char*)pFWBoneObjectDesc);
	field = desc->AddField("", "bool", "modifyShapeWidth", "");
	field->offset = int((char*)&(pFWBoneObjectDesc->modifyShapeWidth) - (char*)pFWBoneObjectDesc);
	field = desc->AddField("", "bool", "modifyShapePose", "");
	field->offset = int((char*)&(pFWBoneObjectDesc->modifyShapePose) - (char*)pFWBoneObjectDesc);
	field = desc->AddField("", "bool", "modifyPlugSocketPose", "");
	field->offset = int((char*)&(pFWBoneObjectDesc->modifyPlugSocketPose) - (char*)pFWBoneObjectDesc);
	field = desc->AddField("FWBoneObjectAdaptType", "enum", "AdaptType", "");
	field->AddEnumConst("GRFRAME_TO_PHSOLID", Spr::FWBoneObjectDesc::GRFRAME_TO_PHSOLID);
	field->AddEnumConst("PHSOLID_TO_GRFRAME", Spr::FWBoneObjectDesc::PHSOLID_TO_GRFRAME);
	field->offset = int((char*)&(pFWBoneObjectDesc->AdaptType) - (char*)pFWBoneObjectDesc);
	db->RegisterDesc(desc);
	Spr::FWStructureDesc* pFWStructureDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWStructureDesc");
	desc->size = sizeof(Spr::FWStructureDesc);
	desc->ifInfo = FWStructureIf::GetIfInfoStatic();
	((IfInfo*)FWStructureIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWStructureDesc>;
	db->RegisterDesc(desc);
	Spr::FWSceneDesc* pFWSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWSceneDesc");
	desc->size = sizeof(Spr::FWSceneDesc);
	desc->ifInfo = FWSceneIf::GetIfInfoStatic();
	((IfInfo*)FWSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWSceneDesc>;
	db->RegisterDesc(desc);
	Spr::FWSdkDesc* pFWSdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWSdkDesc");
	desc->size = sizeof(Spr::FWSdkDesc);
	desc->ifInfo = FWSdkIf::GetIfInfoStatic();
	((IfInfo*)FWSdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWSdkDesc>;
	db->RegisterDesc(desc);
	Spr::FWWinDesc* pFWWinDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWWinDesc");
	desc->size = sizeof(Spr::FWWinDesc);
	desc->ifInfo = FWWinIf::GetIfInfoStatic();
	((IfInfo*)FWWinIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWWinDesc>;
	field = desc->AddField("", "int", "width", "");
	field->offset = int((char*)&(pFWWinDesc->width) - (char*)pFWWinDesc);
	field = desc->AddField("", "int", "height", "");
	field->offset = int((char*)&(pFWWinDesc->height) - (char*)pFWWinDesc);
	field = desc->AddField("", "int", "left", "");
	field->offset = int((char*)&(pFWWinDesc->left) - (char*)pFWWinDesc);
	field = desc->AddField("", "int", "top", "");
	field->offset = int((char*)&(pFWWinDesc->top) - (char*)pFWWinDesc);
	field = desc->AddField("", "int", "parentWindow", "");
	field->offset = int((char*)&(pFWWinDesc->parentWindow) - (char*)pFWWinDesc);
	field = desc->AddField("", "UTString", "title", "");
	field->offset = int((char*)&(pFWWinDesc->title) - (char*)pFWWinDesc);
	field = desc->AddField("", "bool", "fullscreen", "");
	field->offset = int((char*)&(pFWWinDesc->fullscreen) - (char*)pFWWinDesc);
	field = desc->AddField("", "bool", "useKeyMouse", "");
	field->offset = int((char*)&(pFWWinDesc->useKeyMouse) - (char*)pFWWinDesc);
	field = desc->AddField("", "bool", "useJoyStick", "");
	field->offset = int((char*)&(pFWWinDesc->useJoyStick) - (char*)pFWWinDesc);
	field = desc->AddField("", "bool", "useTrackball", "");
	field->offset = int((char*)&(pFWWinDesc->useTrackball) - (char*)pFWWinDesc);
	field = desc->AddField("", "bool", "useDragger", "");
	field->offset = int((char*)&(pFWWinDesc->useDragger) - (char*)pFWWinDesc);
	db->RegisterDesc(desc);
	Spr::Mobility* pMobility = NULL;
	desc = DBG_NEW UTTypeDesc("Mobility");
	desc->size = sizeof(Spr::Mobility);
	desc->access = DBG_NEW UTAccess<Spr::Mobility>;
	field = desc->AddField("", "Vec3d", "force", "");
	field->offset = int((char*)&(pMobility->force) - (char*)pMobility);
	field = desc->AddField("", "TMatrixRow<(6,3,double)>", "A", "");
	field->offset = int((char*)&(pMobility->A) - (char*)pMobility);
	field = desc->AddField("vector", "Vec3d", "f", "");
	field->offset = int((char*)&(pMobility->f) - (char*)pMobility);
	field = desc->AddField("", "TMatrixRow<(6,6,double)>", "Minv", "");
	field->offset = int((char*)&(pMobility->Minv) - (char*)pMobility);
	db->RegisterDesc(desc);
	Spr::ToHaptic* pToHaptic = NULL;
	desc = DBG_NEW UTTypeDesc("ToHaptic");
	desc->size = sizeof(Spr::ToHaptic);
	desc->access = DBG_NEW UTAccess<Spr::ToHaptic>;
	field = desc->AddField("", "Posed", "pose", "");
	field->offset = int((char*)&(pToHaptic->pose) - (char*)pToHaptic);
	field = desc->AddField("", "Vec3d", "closest_point", "");
	field->offset = int((char*)&(pToHaptic->closest_point) - (char*)pToHaptic);
	field = desc->AddField("", "Vec3d", "last_closest_point", "");
	field->offset = int((char*)&(pToHaptic->last_closest_point) - (char*)pToHaptic);
	field = desc->AddField("", "Vec3d", "pointer_point", "");
	field->offset = int((char*)&(pToHaptic->pointer_point) - (char*)pToHaptic);
	field = desc->AddField("", "Vec3d", "common_point", "");
	field->offset = int((char*)&(pToHaptic->common_point) - (char*)pToHaptic);
	field = desc->AddField("", "Vec3d", "face_normal", "");
	field->offset = int((char*)&(pToHaptic->face_normal) - (char*)pToHaptic);
	field = desc->AddField("", "Vec3d", "last_face_normal", "");
	field->offset = int((char*)&(pToHaptic->last_face_normal) - (char*)pToHaptic);
	field = desc->AddField("", "double", "section_depth", "");
	field->offset = int((char*)&(pToHaptic->section_depth) - (char*)pToHaptic);
	field = desc->AddField("", "double", "last_section_depth", "");
	field->offset = int((char*)&(pToHaptic->last_section_depth) - (char*)pToHaptic);
	field = desc->AddField("vector", "Vec3d", "intersection_vertices", "");
	field->offset = int((char*)&(pToHaptic->intersection_vertices) - (char*)pToHaptic);
	field = desc->AddField("vector", "Vec3d", "pointer_section", "");
	field->offset = int((char*)&(pToHaptic->pointer_section) - (char*)pToHaptic);
	field = desc->AddField("vector", "Vec3d", "solid_section", "");
	field->offset = int((char*)&(pToHaptic->solid_section) - (char*)pToHaptic);
	db->RegisterDesc(desc);
	Spr::ToPhysic* pToPhysic = NULL;
	desc = DBG_NEW UTTypeDesc("ToPhysic");
	desc->size = sizeof(Spr::ToPhysic);
	desc->access = DBG_NEW UTAccess<Spr::ToPhysic>;
	field = desc->AddField("", "Vec3d", "test_force", "");
	field->offset = int((char*)&(pToPhysic->test_force) - (char*)pToPhysic);
	field = desc->AddField("", "Vec3d", "test_torque", "");
	field->offset = int((char*)&(pToPhysic->test_torque) - (char*)pToPhysic);
	field = desc->AddField("", "Vec3d", "impulse", "");
	field->offset = int((char*)&(pToPhysic->impulse) - (char*)pToPhysic);
	db->RegisterDesc(desc);
	Spr::Flag* pFlag = NULL;
	desc = DBG_NEW UTTypeDesc("Flag");
	desc->size = sizeof(Spr::Flag);
	desc->access = DBG_NEW UTAccess<Spr::Flag>;
	field = desc->AddField("", "bool", "blocal", "");
	field->offset = int((char*)&(pFlag->blocal) - (char*)pFlag);
	field = desc->AddField("", "bool", "bfirstlocal", "");
	field->offset = int((char*)&(pFlag->bfirstlocal) - (char*)pFlag);
	db->RegisterDesc(desc);
	Spr::FWInteractInfo* pFWInteractInfo = NULL;
	desc = DBG_NEW UTTypeDesc("FWInteractInfo");
	desc->size = sizeof(Spr::FWInteractInfo);
	desc->access = DBG_NEW UTAccess<Spr::FWInteractInfo>;
	field = desc->AddField("", "Flag", "flag", "");
	field->offset = int((char*)&(pFWInteractInfo->flag) - (char*)pFWInteractInfo);
	field = desc->AddField("", "Mobility", "mobility", "");
	field->offset = int((char*)&(pFWInteractInfo->mobility) - (char*)pFWInteractInfo);
	field = desc->AddField("", "ToHaptic", "toHaptic", "");
	field->offset = int((char*)&(pFWInteractInfo->toHaptic) - (char*)pFWInteractInfo);
	field = desc->AddField("", "ToPhysic", "toPhysic", "");
	field->offset = int((char*)&(pFWInteractInfo->toPhysic) - (char*)pFWInteractInfo);
	db->RegisterDesc(desc);
	Spr::FWInteractSolid* pFWInteractSolid = NULL;
	desc = DBG_NEW UTTypeDesc("FWInteractSolid");
	desc->size = sizeof(Spr::FWInteractSolid);
	desc->access = DBG_NEW UTAccess<Spr::FWInteractSolid>;
	field = desc->AddBase("SceneObject");
	field->offset = int((char*)(SceneObject*)pFWInteractSolid - (char*)pFWInteractSolid);
	field = desc->AddField("", "bool", "bfirstSim", "");
	field->offset = int((char*)&(pFWInteractSolid->bfirstSim) - (char*)pFWInteractSolid);
	field = desc->AddField("", "bool", "bSim", "");
	field->offset = int((char*)&(pFWInteractSolid->bSim) - (char*)pFWInteractSolid);
	field = desc->AddField("pointer", "PHSolid", "sceneSolid", "");
	field->offset = int((char*)&(pFWInteractSolid->sceneSolid) - (char*)pFWInteractSolid);
	field = desc->AddField("", "PHSolid", "copiedSolid", "");
	field->offset = int((char*)&(pFWInteractSolid->copiedSolid) - (char*)pFWInteractSolid);
	field = desc->AddField("", "SpatialVector", "b", "");
	field->offset = int((char*)&(pFWInteractSolid->b) - (char*)pFWInteractSolid);
	field = desc->AddField("", "SpatialVector", "curb", "");
	field->offset = int((char*)&(pFWInteractSolid->curb) - (char*)pFWInteractSolid);
	field = desc->AddField("", "SpatialVector", "lastb", "");
	field->offset = int((char*)&(pFWInteractSolid->lastb) - (char*)pFWInteractSolid);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(FWBone, SceneObject);
Spr::PHSolidIf* Spr::FWBoneIf::GetPHSolid(){
	return	((FWBone*)(Object*)(ObjectIf*)this)->GetPHSolid();
}
void Spr::FWBoneIf::SetPHSolid(Spr::PHSolidIf* s){
	((FWBone*)(Object*)(ObjectIf*)this)->SetPHSolid(s);
}
Spr::GRFrameIf* Spr::FWBoneIf::GetGRFrame(){
	return	((FWBone*)(Object*)(ObjectIf*)this)->GetGRFrame();
}
void Spr::FWBoneIf::SetGRFrame(Spr::GRFrameIf* f){
	((FWBone*)(Object*)(ObjectIf*)this)->SetGRFrame(f);
}
Spr::CDRoundConeIf* Spr::FWBoneIf::GetShape(){
	return	((FWBone*)(Object*)(ObjectIf*)this)->GetShape();
}
void Spr::FWBoneIf::SetShapeRadius(double r1, double r2){
	((FWBone*)(Object*)(ObjectIf*)this)->SetShapeRadius(r1, r2);
}
Spr::PHJointIf* Spr::FWBoneIf::GetJoint(){
	return	((FWBone*)(Object*)(ObjectIf*)this)->GetJoint();
}
void Spr::FWBoneIf::SetJointKDD2(double K, double D, double D2){
	((FWBone*)(Object*)(ObjectIf*)this)->SetJointKDD2(K, D, D2);
}
SPR_IFIMP1(FWFemMesh, FWObject);
SPR_IFIMP1(FWGraphicsAdaptee, Object);
void Spr::FWGraphicsAdapteeIf::Init(int argc, char** argv){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->Init(argc, argv);
}
void Spr::FWGraphicsAdapteeIf::StartMainLoop(){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->StartMainLoop();
}
void Spr::FWGraphicsAdapteeIf::EnableIdleFunc(bool on){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->EnableIdleFunc(on);
}
void Spr::FWGraphicsAdapteeIf::EnterGameMode(){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->EnterGameMode();
}
void Spr::FWGraphicsAdapteeIf::LeaveGameMode(){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->LeaveGameMode();
}
FWWinIf* Spr::FWGraphicsAdapteeIf::CreateWin(const FWWinDesc& d){
	return	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->CreateWin(d);
}
void Spr::FWGraphicsAdapteeIf::DestroyWin(FWWinIf* w){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->DestroyWin(w);
}
void Spr::FWGraphicsAdapteeIf::SetCurrentWin(FWWinIf* w){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->SetCurrentWin(w);
}
FWWinIf* Spr::FWGraphicsAdapteeIf::GetCurrentWin(){
	return	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->GetCurrentWin();
}
int Spr::FWGraphicsAdapteeIf::GetWinFromId(){
	return	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->GetWinFromId();
}
void Spr::FWGraphicsAdapteeIf::PostRedisplay(){
	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->PostRedisplay();
}
int Spr::FWGraphicsAdapteeIf::GetModifiers(){
	return	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->GetModifiers();
}
Spr::GRDeviceIf* Spr::FWGraphicsAdapteeIf::GetGRDevice(){
	return	((FWGraphicsAdaptee*)(Object*)(ObjectIf*)this)->GetGRDevice();
}
SPR_IFIMP1(FWInteractPointer, SceneObject);
void Spr::FWInteractPointerIf::SetPointerSolid(Spr::PHSolidIf* solid){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetPointerSolid(solid);
}
Spr::PHSolidIf* Spr::FWInteractPointerIf::GetPointerSolid(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetPointerSolid();
}
void Spr::FWInteractPointerIf::SetHI(Spr::HIBaseIf* hi){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetHI(hi);
}
Spr::HIBaseIf* Spr::FWInteractPointerIf::GetHI(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetHI();
}
void Spr::FWInteractPointerIf::SetPosScale(double s){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetPosScale(s);
}
double Spr::FWInteractPointerIf::GetPosScale(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetPosScale();
}
void Spr::FWInteractPointerIf::SetForceScale(double s){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetForceScale(s);
}
double Spr::FWInteractPointerIf::GetForceScale(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetForceScale();
}
void Spr::FWInteractPointerIf::SetTorqueScale(double s){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetTorqueScale(s);
}
double Spr::FWInteractPointerIf::GetTorqueScale(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetTorqueScale();
}
void Spr::FWInteractPointerIf::SetLocalRange(double r){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetLocalRange(r);
}
double Spr::FWInteractPointerIf::GetLocalRange(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetLocalRange();
}
void Spr::FWInteractPointerIf::SetPointersCalibPosition(Posed p){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetPointersCalibPosition(p);
}
Posed Spr::FWInteractPointerIf::GetPointersCalibPosition(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetPointersCalibPosition();
}
void Spr::FWInteractPointerIf::SetDefaultPosition(Posed p){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetDefaultPosition(p);
}
Posed Spr::FWInteractPointerIf::GetDefaultPosition(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetDefaultPosition();
}
void Spr::FWInteractPointerIf::SetSpringCoeff(double k){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetSpringCoeff(k);
}
double Spr::FWInteractPointerIf::GetSpringCoeff(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetSpringCoeff();
}
void Spr::FWInteractPointerIf::SetDamperCoeff(double d){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetDamperCoeff(d);
}
double Spr::FWInteractPointerIf::GetDamperCoeff(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetDamperCoeff();
}
void Spr::FWInteractPointerIf::EnableForce(bool b){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->EnableForce(b);
}
void Spr::FWInteractPointerIf::EnableVibration(bool b){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->EnableVibration(b);
}
void Spr::FWInteractPointerIf::Calibration(){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->Calibration();
}
void Spr::FWInteractPointerIf::SetGrabFlag(int f){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetGrabFlag(f);
}
int Spr::FWInteractPointerIf::GetGrabFlag(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetGrabFlag();
}
void Spr::FWInteractPointerIf::SetCameraOri(Quaterniond q){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetCameraOri(q);
}
Quaterniond Spr::FWInteractPointerIf::GetCameraOri(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetCameraOri();
}
void Spr::FWInteractPointerIf::SetSpringK(double k){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetSpringK(k);
}
double Spr::FWInteractPointerIf::GetSpringK(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetSpringK();
}
void Spr::FWInteractPointerIf::SetDamperD(double d){
	((FWInteractPointer*)(Object*)(ObjectIf*)this)->SetDamperD(d);
}
double Spr::FWInteractPointerIf::GetDamperD(){
	return	((FWInteractPointer*)(Object*)(ObjectIf*)this)->GetDamperD();
}
SPR_IFIMP1(FWInteractScene, Scene);
Spr::FWSceneIf* Spr::FWInteractSceneIf::GetScene(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetScene();
}
void Spr::FWInteractSceneIf::CreateIAAdaptee(Spr::FWInteractMode mode){
	((FWInteractScene*)(Object*)(ObjectIf*)this)->CreateIAAdaptee(mode);
}
Spr::FWInteractAdaptee* Spr::FWInteractSceneIf::GetIAAdaptee(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetIAAdaptee();
}
Spr::FWInteractPointerIf* Spr::FWInteractSceneIf::CreateIAPointer(const Spr::FWInteractPointerDesc& desc){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->CreateIAPointer(desc);
}
Spr::FWInteractPointerIf* Spr::FWInteractSceneIf::GetIAPointer(int i){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetIAPointer(i);
}
int Spr::FWInteractSceneIf::NIAPointers(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->NIAPointers();
}
Spr::FWInteractMode Spr::FWInteractSceneIf::GetIAMode(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetIAMode();
}
Spr::FWHapticMode Spr::FWInteractSceneIf::GetHMode(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetHMode();
}
void Spr::FWInteractSceneIf::SetHMode(Spr::FWHapticMode hMode){
	((FWInteractScene*)(Object*)(ObjectIf*)this)->SetHMode(hMode);
}
void Spr::FWInteractSceneIf::Init(){
	((FWInteractScene*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::FWInteractSceneIf::Clear(){
	((FWInteractScene*)(Object*)(ObjectIf*)this)->Clear();
}
void Spr::FWInteractSceneIf::Step(){
	((FWInteractScene*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::FWInteractSceneIf::CallBackHapticLoop(){
	((FWInteractScene*)(Object*)(ObjectIf*)this)->CallBackHapticLoop();
}
bool Spr::FWInteractSceneIf::GetContactFlag(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetContactFlag();
}
Vec3d Spr::FWInteractSceneIf::GetPicCenterPosition(){
	return	((FWInteractScene*)(Object*)(ObjectIf*)this)->GetPicCenterPosition();
}
SPR_IFIMP1(FWObject, SceneObject);
Spr::PHSolidIf* Spr::FWObjectIf::GetPHSolid(){
	return	((FWObject*)(Object*)(ObjectIf*)this)->GetPHSolid();
}
void Spr::FWObjectIf::SetPHSolid(Spr::PHSolidIf* s){
	((FWObject*)(Object*)(ObjectIf*)this)->SetPHSolid(s);
}
Spr::GRFrameIf* Spr::FWObjectIf::GetGRFrame(){
	return	((FWObject*)(Object*)(ObjectIf*)this)->GetGRFrame();
}
void Spr::FWObjectIf::SetGRFrame(Spr::GRFrameIf* f){
	((FWObject*)(Object*)(ObjectIf*)this)->SetGRFrame(f);
}
bool Spr::FWObjectIf::LoadMesh(const char* filename, const IfInfo* ii, Spr::GRFrameIf* frame){
	return	((FWObject*)(Object*)(ObjectIf*)this)->LoadMesh(filename, ii, frame);
}
void Spr::FWObjectIf::GenerateCDMesh(Spr::GRFrameIf* frame, const PHMaterial& mat){
	((FWObject*)(Object*)(ObjectIf*)this)->GenerateCDMesh(frame, mat);
}
void Spr::FWObjectIf::Sync(bool ph_to_gr){
	((FWObject*)(Object*)(ObjectIf*)this)->Sync(ph_to_gr);
}
double Spr::FWObjectIf::GetSolidLength(){
	return	((FWObject*)(Object*)(ObjectIf*)this)->GetSolidLength();
}
void Spr::FWObjectIf::SetSolidLength(double l){
	((FWObject*)(Object*)(ObjectIf*)this)->SetSolidLength(l);
}
SPR_IFIMP1(FWBoneObject, FWObject);
Spr::PHJointIf* Spr::FWBoneObjectIf::GetPHJoint(){
	return	((FWBoneObject*)(Object*)(ObjectIf*)this)->GetPHJoint();
}
void Spr::FWBoneObjectIf::SetPHJoint(Spr::PHJointIf* j){
	((FWBoneObject*)(Object*)(ObjectIf*)this)->SetPHJoint(j);
}
Spr::GRFrameIf* Spr::FWBoneObjectIf::GetEndFrame(){
	return	((FWBoneObject*)(Object*)(ObjectIf*)this)->GetEndFrame();
}
void Spr::FWBoneObjectIf::SetEndFrame(Spr::GRFrameIf* f){
	((FWBoneObject*)(Object*)(ObjectIf*)this)->SetEndFrame(f);
}
void Spr::FWBoneObjectIf::SetAdaptType(int t){
	((FWBoneObject*)(Object*)(ObjectIf*)this)->SetAdaptType(t);
}
SPR_IFIMP1(FWStructure, SceneObject);
Spr::FWBoneObjectIf* Spr::FWStructureIf::GetBone(int n){
	return	((FWStructure*)(Object*)(ObjectIf*)this)->GetBone(n);
}
Spr::FWBoneObjectIf* Spr::FWStructureIf::GetBone(char* n){
	return	((FWStructure*)(Object*)(ObjectIf*)this)->GetBone(n);
}
int Spr::FWStructureIf::GetBoneSize(){
	return	((FWStructure*)(Object*)(ObjectIf*)this)->GetBoneSize();
}
void Spr::FWStructureIf::SetPose(Posed p){
	((FWStructure*)(Object*)(ObjectIf*)this)->SetPose(p);
}
void Spr::FWStructureIf::AddBone(Spr::FWBoneObjectIf* o){
	((FWStructure*)(Object*)(ObjectIf*)this)->AddBone(o);
}
SPR_IFIMP1(FWScene, Scene);
Spr::PHSceneIf* Spr::FWSceneIf::GetPHScene(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetPHScene();
}
void Spr::FWSceneIf::SetPHScene(Spr::PHSceneIf* s){
	((FWScene*)(Object*)(ObjectIf*)this)->SetPHScene(s);
}
Spr::GRSceneIf* Spr::FWSceneIf::GetGRScene(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetGRScene();
}
void Spr::FWSceneIf::SetGRScene(Spr::GRSceneIf* s){
	((FWScene*)(Object*)(ObjectIf*)this)->SetGRScene(s);
}
Spr::FWObjectIf* Spr::FWSceneIf::CreateFWObject(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->CreateFWObject();
}
int Spr::FWSceneIf::NObject()const{
	return	((FWScene*)(Object*)(ObjectIf*)this)->NObject();
}
Spr::FWObjectIf** Spr::FWSceneIf::GetObjects(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetObjects();
}
void Spr::FWSceneIf::Sync(bool ph_to_gr){
	((FWScene*)(Object*)(ObjectIf*)this)->Sync(ph_to_gr);
}
void Spr::FWSceneIf::Step(){
	((FWScene*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::FWSceneIf::Draw(Spr::GRRenderIf* grRender, bool debug){
	((FWScene*)(Object*)(ObjectIf*)this)->Draw(grRender, debug);
}
void Spr::FWSceneIf::DrawPHScene(Spr::GRRenderIf* render){
	((FWScene*)(Object*)(ObjectIf*)this)->DrawPHScene(render);
}
void Spr::FWSceneIf::DrawSolid(Spr::GRRenderIf* render, Spr::PHSolidIf* solid, bool solid_or_wire){
	((FWScene*)(Object*)(ObjectIf*)this)->DrawSolid(render, solid, solid_or_wire);
}
void Spr::FWSceneIf::DrawShape(Spr::GRRenderIf* render, Spr::CDShapeIf* shape, bool solid){
	((FWScene*)(Object*)(ObjectIf*)this)->DrawShape(render, shape, solid);
}
void Spr::FWSceneIf::DrawConstraint(Spr::GRRenderIf* render, Spr::PHConstraintIf* con){
	((FWScene*)(Object*)(ObjectIf*)this)->DrawConstraint(render, con);
}
void Spr::FWSceneIf::DrawContact(Spr::GRRenderIf* render, Spr::PHContactPointIf* con){
	((FWScene*)(Object*)(ObjectIf*)this)->DrawContact(render, con);
}
void Spr::FWSceneIf::DrawIK(Spr::GRRenderIf* render, Spr::PHIKEngineIf* ikEngine){
	((FWScene*)(Object*)(ObjectIf*)this)->DrawIK(render, ikEngine);
}
void Spr::FWSceneIf::SetRenderMode(bool solid, bool wire){
	((FWScene*)(Object*)(ObjectIf*)this)->SetRenderMode(solid, wire);
}
void Spr::FWSceneIf::EnableRender(Spr::ObjectIf* obj, bool enable){
	((FWScene*)(Object*)(ObjectIf*)this)->EnableRender(obj, enable);
}
void Spr::FWSceneIf::SetSolidMaterial(int mat, Spr::PHSolidIf* solid){
	((FWScene*)(Object*)(ObjectIf*)this)->SetSolidMaterial(mat, solid);
}
void Spr::FWSceneIf::SetWireMaterial(int mat, Spr::PHSolidIf* solid){
	((FWScene*)(Object*)(ObjectIf*)this)->SetWireMaterial(mat, solid);
}
void Spr::FWSceneIf::EnableRenderAxis(bool world, bool solid, bool con){
	((FWScene*)(Object*)(ObjectIf*)this)->EnableRenderAxis(world, solid, con);
}
void Spr::FWSceneIf::SetAxisMaterial(int matX, int matY, int matZ){
	((FWScene*)(Object*)(ObjectIf*)this)->SetAxisMaterial(matX, matY, matZ);
}
void Spr::FWSceneIf::SetAxisScale(float scaleWorld, float scaleSolid, float scaleCon){
	((FWScene*)(Object*)(ObjectIf*)this)->SetAxisScale(scaleWorld, scaleSolid, scaleCon);
}
void Spr::FWSceneIf::SetAxisStyle(int style){
	((FWScene*)(Object*)(ObjectIf*)this)->SetAxisStyle(style);
}
void Spr::FWSceneIf::EnableRenderForce(bool solid, bool constraint){
	((FWScene*)(Object*)(ObjectIf*)this)->EnableRenderForce(solid, constraint);
}
void Spr::FWSceneIf::SetForceMaterial(int matForce, int matMoment){
	((FWScene*)(Object*)(ObjectIf*)this)->SetForceMaterial(matForce, matMoment);
}
void Spr::FWSceneIf::SetForceScale(float scaleForce, float scaleMoment){
	((FWScene*)(Object*)(ObjectIf*)this)->SetForceScale(scaleForce, scaleMoment);
}
void Spr::FWSceneIf::EnableRenderContact(bool enable){
	((FWScene*)(Object*)(ObjectIf*)this)->EnableRenderContact(enable);
}
void Spr::FWSceneIf::SetContactMaterial(int mat){
	((FWScene*)(Object*)(ObjectIf*)this)->SetContactMaterial(mat);
}
void Spr::FWSceneIf::EnableRenderGrid(bool x, bool y, bool z){
	((FWScene*)(Object*)(ObjectIf*)this)->EnableRenderGrid(x, y, z);
}
void Spr::FWSceneIf::SetGridOption(char axis, float offset, float size, int slice){
	((FWScene*)(Object*)(ObjectIf*)this)->SetGridOption(axis, offset, size, slice);
}
void Spr::FWSceneIf::SetGridMaterial(int matX, int matY, int matZ){
	((FWScene*)(Object*)(ObjectIf*)this)->SetGridMaterial(matX, matY, matZ);
}
void Spr::FWSceneIf::EnableRenderIK(bool enable){
	((FWScene*)(Object*)(ObjectIf*)this)->EnableRenderIK(enable);
}
void Spr::FWSceneIf::SetIKMaterial(int mat){
	((FWScene*)(Object*)(ObjectIf*)this)->SetIKMaterial(mat);
}
void Spr::FWSceneIf::SetIKScale(float scale){
	((FWScene*)(Object*)(ObjectIf*)this)->SetIKScale(scale);
}
void Spr::FWSceneIf::AddHumanInterface(Spr::HIForceDevice6D* d){
	((FWScene*)(Object*)(ObjectIf*)this)->AddHumanInterface(d);
}
void Spr::FWSceneIf::SetFWBones(Spr::FWBoneIf* b){
	((FWScene*)(Object*)(ObjectIf*)this)->SetFWBones(b);
}
std::vector< UTRef< Spr::FWBoneIf > > Spr::FWSceneIf::GetFWBones(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetFWBones();
}
void Spr::FWSceneIf::CreateFWStructure(){
	((FWScene*)(Object*)(ObjectIf*)this)->CreateFWStructure();
}
void Spr::FWSceneIf::AddFWStructure(Spr::FWStructureIf* o){
	((FWScene*)(Object*)(ObjectIf*)this)->AddFWStructure(o);
}
Spr::FWStructureIf* Spr::FWSceneIf::GetFWStructure(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetFWStructure();
}
Spr::FWStructureIf* Spr::FWSceneIf::GetFWStructure(int n){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetFWStructure(n);
}
size_t Spr::FWSceneIf::NFWStructure(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->NFWStructure();
}
SPR_IFIMP1(FWSdk, Sdk);
Spr::FWSceneIf* Spr::FWSdkIf::CreateScene(const PHSceneDesc& phdesc, const GRSceneDesc& grdesc){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->CreateScene(phdesc, grdesc);
}
bool Spr::FWSdkIf::LoadScene(UTString filename, Spr::ImportIf* ex, const IfInfo* ii, Spr::ObjectIfs* objs){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->LoadScene(filename, ex, ii, objs);
}
bool Spr::FWSdkIf::SaveScene(UTString filename, Spr::ImportIf* ex, const IfInfo* ii, Spr::ObjectIfs* objs){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->SaveScene(filename, ex, ii, objs);
}
int Spr::FWSdkIf::NScene()const{
	return	((FWSdk*)(Object*)(ObjectIf*)this)->NScene();
}
Spr::FWSceneIf* Spr::FWSdkIf::GetScene(int index){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetScene(index);
}
void Spr::FWSdkIf::MergeScene(Spr::FWSceneIf* scene0, Spr::FWSceneIf* scene1){
	((FWSdk*)(Object*)(ObjectIf*)this)->MergeScene(scene0, scene1);
}
Spr::FWInteractSceneIf* Spr::FWSdkIf::CreateIAScene(const Spr::FWInteractSceneDesc& desc){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->CreateIAScene(desc);
}
Spr::FWInteractSceneIf* Spr::FWSdkIf::GetIAScene(int index){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetIAScene(index);
}
int Spr::FWSdkIf::NIAScenes(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->NIAScenes();
}
void Spr::FWSdkIf::ClearIAScenes(){
	((FWSdk*)(Object*)(ObjectIf*)this)->ClearIAScenes();
}
PHSdkIf* Spr::FWSdkIf::GetPHSdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetPHSdk();
}
GRSdkIf* Spr::FWSdkIf::GetGRSdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetGRSdk();
}
Spr::FISdkIf* Spr::FWSdkIf::GetFISdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetFISdk();
}
Spr::HISdkIf* Spr::FWSdkIf::GetHISdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetHISdk();
}
bool Spr::FWSdkIf::GetDebugMode(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetDebugMode();
}
void Spr::FWSdkIf::SetDebugMode(bool debug){
	((FWSdk*)(Object*)(ObjectIf*)this)->SetDebugMode(debug);
}
void Spr::FWSdkIf::Step(){
	((FWSdk*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::FWSdkIf::Draw(){
	((FWSdk*)(Object*)(ObjectIf*)this)->Draw();
}
void Spr::FWSdkIf::Reshape(int w, int h){
	((FWSdk*)(Object*)(ObjectIf*)this)->Reshape(w, h);
}
Spr::GRRenderIf* Spr::FWSdkIf::GetRender(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetRender();
}
SPR_IFIMP1(FWWin, Object);
void Spr::FWWinIf::SetPosition(int left, int top){
	((FWWin*)(Object*)(ObjectIf*)this)->SetPosition(left, top);
}
void Spr::FWWinIf::SetSize(int width, int height){
	((FWWin*)(Object*)(ObjectIf*)this)->SetSize(width, height);
}
void Spr::FWWinIf::SetTitle(UTString title){
	((FWWin*)(Object*)(ObjectIf*)this)->SetTitle(title);
}
UTString Spr::FWWinIf::GetTitle(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetTitle();
}
void Spr::FWWinIf::SetFullScreen(bool full){
	((FWWin*)(Object*)(ObjectIf*)this)->SetFullScreen(full);
}
bool Spr::FWWinIf::GetFullScreen(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetFullScreen();
}
Vec2i Spr::FWWinIf::GetPosition(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetPosition();
}
int Spr::FWWinIf::GetWidth(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetWidth();
}
int Spr::FWWinIf::GetHeight(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetHeight();
}
int Spr::FWWinIf::GetID(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetID();
}
void Spr::FWWinIf::SetID(int newID){
	((FWWin*)(Object*)(ObjectIf*)this)->SetID(newID);
}
Spr::GRRenderIf* Spr::FWWinIf::GetRender(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetRender();
}
void Spr::FWWinIf::SetRender(Spr::GRRenderIf* data){
	((FWWin*)(Object*)(ObjectIf*)this)->SetRender(data);
}
Spr::FWSceneIf* Spr::FWWinIf::GetScene(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetScene();
}
void Spr::FWWinIf::SetScene(Spr::FWSceneIf* s){
	((FWWin*)(Object*)(ObjectIf*)this)->SetScene(s);
}
Spr::DVKeyMouseIf* Spr::FWWinIf::GetKeyMouse(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetKeyMouse();
}
void Spr::FWWinIf::SetKeyMouse(Spr::DVKeyMouseIf* dv){
	((FWWin*)(Object*)(ObjectIf*)this)->SetKeyMouse(dv);
}
Spr::DVJoyStickIf* Spr::FWWinIf::GetJoyStick(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetJoyStick();
}
void Spr::FWWinIf::SetJoyStick(Spr::DVJoyStickIf* dv){
	((FWWin*)(Object*)(ObjectIf*)this)->SetJoyStick(dv);
}
Spr::HITrackballIf* Spr::FWWinIf::GetTrackball(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetTrackball();
}
void Spr::FWWinIf::SetTrackball(Spr::HITrackballIf* dv){
	((FWWin*)(Object*)(ObjectIf*)this)->SetTrackball(dv);
}
Spr::HIDraggerIf* Spr::FWWinIf::GetDragger(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetDragger();
}
void Spr::FWWinIf::SetDragger(Spr::HIDraggerIf* dv){
	((FWWin*)(Object*)(ObjectIf*)this)->SetDragger(dv);
}
void Spr::FWWinIf::SetDebugMode(bool ph_or_gr){
	((FWWin*)(Object*)(ObjectIf*)this)->SetDebugMode(ph_or_gr);
}
bool Spr::FWWinIf::GetDebugMode(){
	return	((FWWin*)(Object*)(ObjectIf*)this)->GetDebugMode();
}
void Spr::FWWinIf::Display(){
	((FWWin*)(Object*)(ObjectIf*)this)->Display();
}
void Spr::FWWinIf::Reshape(int w, int h){
	((FWWin*)(Object*)(ObjectIf*)this)->Reshape(w, h);
}
SPR_OBJECTIMP1(FWBone, FWObject);
SPR_OBJECTIMP1(FWFemMesh, FWObject);
SPR_OBJECTIMP1(FWWinGLUT, FWWin);
SPR_OBJECTIMP1(FWGLUT, FWGraphicsAdaptee);
SPR_OBJECTIMP_ABST1(FWGraphicsAdaptee, Object);
SPR_OBJECTIMP1(FWInteractPointer, SceneObject);
SPR_OBJECTIMP1(FWInteractScene, Scene);
SPR_OBJECTIMP1(FWObject, SceneObject);
SPR_OBJECTIMP1(FWBoneObject, FWObject);
SPR_OBJECTIMP1(FWStructure, SceneObject);
SPR_OBJECTIMP1(FWScene, Scene);
SPR_OBJECTIMP1(FWSdk, Sdk);
SPR_OBJECTIMP1(FWWin, Object);
}
