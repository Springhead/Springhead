#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
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
namespace Spr{

void RegisterTypeDescFoundation(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::ObjectIfs* pObjectIfs = NULL;
	desc = DBG_NEW UTTypeDesc("ObjectIfs");
	desc->size = sizeof(Spr::ObjectIfs);
	desc->access = DBG_NEW UTAccess<Spr::ObjectIfs>;
	db->RegisterDesc(desc);
	Vec2f* pVec2f = NULL;
	desc = DBG_NEW UTTypeDesc("Vec2f");
	desc->size = sizeof(Vec2f);
	desc->access = DBG_NEW UTAccess<Vec2f>;
	field = desc->AddField("", "float", "x", "");
	field->offset = int((char*)&(pVec2f->x) - (char*)pVec2f);
	field = desc->AddField("", "float", "y", "");
	field->offset = int((char*)&(pVec2f->y) - (char*)pVec2f);
	db->RegisterDesc(desc);
	Vec2d* pVec2d = NULL;
	desc = DBG_NEW UTTypeDesc("Vec2d");
	desc->size = sizeof(Vec2d);
	desc->access = DBG_NEW UTAccess<Vec2d>;
	field = desc->AddField("", "double", "x", "");
	field->offset = int((char*)&(pVec2d->x) - (char*)pVec2d);
	field = desc->AddField("", "double", "y", "");
	field->offset = int((char*)&(pVec2d->y) - (char*)pVec2d);
	db->RegisterDesc(desc);
	Vec3f* pVec3f = NULL;
	desc = DBG_NEW UTTypeDesc("Vec3f");
	desc->size = sizeof(Vec3f);
	desc->access = DBG_NEW UTAccess<Vec3f>;
	field = desc->AddField("", "float", "x", "");
	field->offset = int((char*)&(pVec3f->x) - (char*)pVec3f);
	field = desc->AddField("", "float", "y", "");
	field->offset = int((char*)&(pVec3f->y) - (char*)pVec3f);
	field = desc->AddField("", "float", "z", "");
	field->offset = int((char*)&(pVec3f->z) - (char*)pVec3f);
	db->RegisterDesc(desc);
	Vec3d* pVec3d = NULL;
	desc = DBG_NEW UTTypeDesc("Vec3d");
	desc->size = sizeof(Vec3d);
	desc->access = DBG_NEW UTAccess<Vec3d>;
	field = desc->AddField("", "double", "x", "");
	field->offset = int((char*)&(pVec3d->x) - (char*)pVec3d);
	field = desc->AddField("", "double", "y", "");
	field->offset = int((char*)&(pVec3d->y) - (char*)pVec3d);
	field = desc->AddField("", "double", "z", "");
	field->offset = int((char*)&(pVec3d->z) - (char*)pVec3d);
	db->RegisterDesc(desc);
	Vec4f* pVec4f = NULL;
	desc = DBG_NEW UTTypeDesc("Vec4f");
	desc->size = sizeof(Vec4f);
	desc->access = DBG_NEW UTAccess<Vec4f>;
	field = desc->AddField("", "float", "x", "");
	field->offset = int((char*)&(pVec4f->x) - (char*)pVec4f);
	field = desc->AddField("", "float", "y", "");
	field->offset = int((char*)&(pVec4f->y) - (char*)pVec4f);
	field = desc->AddField("", "float", "z", "");
	field->offset = int((char*)&(pVec4f->z) - (char*)pVec4f);
	field = desc->AddField("", "float", "w", "");
	field->offset = int((char*)&(pVec4f->w) - (char*)pVec4f);
	db->RegisterDesc(desc);
	Vec4d* pVec4d = NULL;
	desc = DBG_NEW UTTypeDesc("Vec4d");
	desc->size = sizeof(Vec4d);
	desc->access = DBG_NEW UTAccess<Vec4d>;
	field = desc->AddField("", "double", "x", "");
	field->offset = int((char*)&(pVec4d->x) - (char*)pVec4d);
	field = desc->AddField("", "double", "y", "");
	field->offset = int((char*)&(pVec4d->y) - (char*)pVec4d);
	field = desc->AddField("", "double", "z", "");
	field->offset = int((char*)&(pVec4d->z) - (char*)pVec4d);
	field = desc->AddField("", "double", "w", "");
	field->offset = int((char*)&(pVec4d->w) - (char*)pVec4d);
	db->RegisterDesc(desc);
	Quaternionf* pQuaternionf = NULL;
	desc = DBG_NEW UTTypeDesc("Quaternionf");
	desc->size = sizeof(Quaternionf);
	desc->access = DBG_NEW UTAccess<Quaternionf>;
	field = desc->AddField("", "float", "x", "");
	field->offset = int((char*)&(pQuaternionf->x) - (char*)pQuaternionf);
	field = desc->AddField("", "float", "y", "");
	field->offset = int((char*)&(pQuaternionf->y) - (char*)pQuaternionf);
	field = desc->AddField("", "float", "z", "");
	field->offset = int((char*)&(pQuaternionf->z) - (char*)pQuaternionf);
	field = desc->AddField("", "float", "w", "");
	field->offset = int((char*)&(pQuaternionf->w) - (char*)pQuaternionf);
	db->RegisterDesc(desc);
	Quaterniond* pQuaterniond = NULL;
	desc = DBG_NEW UTTypeDesc("Quaterniond");
	desc->size = sizeof(Quaterniond);
	desc->access = DBG_NEW UTAccess<Quaterniond>;
	field = desc->AddField("", "double", "x", "");
	field->offset = int((char*)&(pQuaterniond->x) - (char*)pQuaterniond);
	field = desc->AddField("", "double", "y", "");
	field->offset = int((char*)&(pQuaterniond->y) - (char*)pQuaterniond);
	field = desc->AddField("", "double", "z", "");
	field->offset = int((char*)&(pQuaterniond->z) - (char*)pQuaterniond);
	field = desc->AddField("", "double", "w", "");
	field->offset = int((char*)&(pQuaterniond->w) - (char*)pQuaterniond);
	db->RegisterDesc(desc);
	Posef* pPosef = NULL;
	desc = DBG_NEW UTTypeDesc("Posef");
	desc->size = sizeof(Posef);
	desc->access = DBG_NEW UTAccess<Posef>;
	field = desc->AddField("", "float", "w", "");
	field->offset = int((char*)&(pPosef->w) - (char*)pPosef);
	field = desc->AddField("", "float", "x", "");
	field->offset = int((char*)&(pPosef->x) - (char*)pPosef);
	field = desc->AddField("", "float", "y", "");
	field->offset = int((char*)&(pPosef->y) - (char*)pPosef);
	field = desc->AddField("", "float", "z", "");
	field->offset = int((char*)&(pPosef->z) - (char*)pPosef);
	field = desc->AddField("", "float", "px", "");
	field->offset = int((char*)&(pPosef->px) - (char*)pPosef);
	field = desc->AddField("", "float", "py", "");
	field->offset = int((char*)&(pPosef->py) - (char*)pPosef);
	field = desc->AddField("", "float", "pz", "");
	field->offset = int((char*)&(pPosef->pz) - (char*)pPosef);
	db->RegisterDesc(desc);
	Posed* pPosed = NULL;
	desc = DBG_NEW UTTypeDesc("Posed");
	desc->size = sizeof(Posed);
	desc->access = DBG_NEW UTAccess<Posed>;
	field = desc->AddField("", "double", "w", "");
	field->offset = int((char*)&(pPosed->w) - (char*)pPosed);
	field = desc->AddField("", "double", "x", "");
	field->offset = int((char*)&(pPosed->x) - (char*)pPosed);
	field = desc->AddField("", "double", "y", "");
	field->offset = int((char*)&(pPosed->y) - (char*)pPosed);
	field = desc->AddField("", "double", "z", "");
	field->offset = int((char*)&(pPosed->z) - (char*)pPosed);
	field = desc->AddField("", "double", "px", "");
	field->offset = int((char*)&(pPosed->px) - (char*)pPosed);
	field = desc->AddField("", "double", "py", "");
	field->offset = int((char*)&(pPosed->py) - (char*)pPosed);
	field = desc->AddField("", "double", "pz", "");
	field->offset = int((char*)&(pPosed->pz) - (char*)pPosed);
	db->RegisterDesc(desc);
	Matrix3f* pMatrix3f = NULL;
	desc = DBG_NEW UTTypeDesc("Matrix3f");
	desc->size = sizeof(Matrix3f);
	desc->access = DBG_NEW UTAccess<Matrix3f>;
	field = desc->AddField("", "float", "data", "9");
	field->offset = int((char*)&(pMatrix3f->data) - (char*)pMatrix3f);
	db->RegisterDesc(desc);
	Matrix3d* pMatrix3d = NULL;
	desc = DBG_NEW UTTypeDesc("Matrix3d");
	desc->size = sizeof(Matrix3d);
	desc->access = DBG_NEW UTAccess<Matrix3d>;
	field = desc->AddField("", "double", "data", "9");
	field->offset = int((char*)&(pMatrix3d->data) - (char*)pMatrix3d);
	db->RegisterDesc(desc);
	Affinef* pAffinef = NULL;
	desc = DBG_NEW UTTypeDesc("Affinef");
	desc->size = sizeof(Affinef);
	desc->access = DBG_NEW UTAccess<Affinef>;
	field = desc->AddField("", "float", "data", "16");
	field->offset = int((char*)&(pAffinef->data) - (char*)pAffinef);
	db->RegisterDesc(desc);
	Affined* pAffined = NULL;
	desc = DBG_NEW UTTypeDesc("Affined");
	desc->size = sizeof(Affined);
	desc->access = DBG_NEW UTAccess<Affined>;
	field = desc->AddField("", "double", "data", "16");
	field->offset = int((char*)&(pAffined->data) - (char*)pAffined);
	db->RegisterDesc(desc);
}

SPR_IFIMP0(Object);
void Spr::ObjectIf::Print(std::ostream& os)const{
	((Object*)(Object*)(ObjectIf*)this)->Print(os);
}
void Spr::ObjectIf::PrintShort(std::ostream& os)const{
	((Object*)(Object*)(ObjectIf*)this)->PrintShort(os);
}
int Spr::ObjectIf::AddRef(){
	return	((Object*)(Object*)(ObjectIf*)this)->AddRef();
}
int Spr::ObjectIf::RefCount(){
	return	((Object*)(Object*)(ObjectIf*)this)->RefCount();
}
size_t Spr::ObjectIf::NChildObject()const{
	return	((Object*)(Object*)(ObjectIf*)this)->NChildObject();
}
Spr::ObjectIf* Spr::ObjectIf::GetChildObject(size_t pos){
	return	((Object*)(Object*)(ObjectIf*)this)->GetChildObject(pos);
}
const Spr::ObjectIf* Spr::ObjectIf::GetChildObject(size_t pos)const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetChildObject(pos);
}
bool Spr::ObjectIf::AddChildObject(Spr::ObjectIf* o){
	return	((Object*)(Object*)(ObjectIf*)this)->AddChildObject(o);
}
bool Spr::ObjectIf::DelChildObject(Spr::ObjectIf* o){
	return	((Object*)(Object*)(ObjectIf*)this)->DelChildObject(o);
}
void Spr::ObjectIf::Clear(){
	((Object*)(Object*)(ObjectIf*)this)->Clear();
}
Spr::ObjectIf* Spr::ObjectIf::CreateObject(const IfInfo* info, const void* desc){
	return	((Object*)(Object*)(ObjectIf*)this)->CreateObject(info, desc);
}
const void* Spr::ObjectIf::GetDescAddress()const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetDescAddress();
}
bool Spr::ObjectIf::GetDesc(void* desc)const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetDesc(desc);
}
void Spr::ObjectIf::SetDesc(const void* desc){
	((Object*)(Object*)(ObjectIf*)this)->SetDesc(desc);
}
size_t Spr::ObjectIf::GetDescSize()const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetDescSize();
}
const void* Spr::ObjectIf::GetStateAddress()const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetStateAddress();
}
bool Spr::ObjectIf::GetState(void* state)const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetState(state);
}
void Spr::ObjectIf::SetState(const void* state){
	((Object*)(Object*)(ObjectIf*)this)->SetState(state);
}
size_t Spr::ObjectIf::GetStateSize()const{
	return	((Object*)(Object*)(ObjectIf*)this)->GetStateSize();
}
void Spr::ObjectIf::ConstructState(void* m)const{
	((Object*)(Object*)(ObjectIf*)this)->ConstructState(m);
}
void Spr::ObjectIf::DestructState(void* m)const{
	((Object*)(Object*)(ObjectIf*)this)->DestructState(m);
}
SPR_IFIMP1(NamedObject, Object);
const char* Spr::NamedObjectIf::GetName()const{
	return	((NamedObject*)(Object*)(ObjectIf*)this)->GetName();
}
void Spr::NamedObjectIf::SetName(const char* n){
	((NamedObject*)(Object*)(ObjectIf*)this)->SetName(n);
}
Spr::NameManagerIf* Spr::NamedObjectIf::GetNameManager(){
	return	((NamedObject*)(Object*)(ObjectIf*)this)->GetNameManager();
}
SPR_IFIMP1(SceneObject, NamedObject);
Spr::SceneIf* Spr::SceneObjectIf::GetScene(){
	return	((SceneObject*)(Object*)(ObjectIf*)this)->GetScene();
}
SPR_IFIMP1(ObjectStates, Object);
void Spr::ObjectStatesIf::AllocateState(Spr::ObjectIf* o){
	((ObjectStates*)(Object*)(ObjectIf*)this)->AllocateState(o);
}
void Spr::ObjectStatesIf::ReleaseState(Spr::ObjectIf* o){
	((ObjectStates*)(Object*)(ObjectIf*)this)->ReleaseState(o);
}
size_t Spr::ObjectStatesIf::CalcStateSize(Spr::ObjectIf* o){
	return	((ObjectStates*)(Object*)(ObjectIf*)this)->CalcStateSize(o);
}
void Spr::ObjectStatesIf::SaveState(Spr::ObjectIf* o){
	((ObjectStates*)(Object*)(ObjectIf*)this)->SaveState(o);
}
void Spr::ObjectStatesIf::LoadState(Spr::ObjectIf* o){
	((ObjectStates*)(Object*)(ObjectIf*)this)->LoadState(o);
}
SPR_IFIMP1(NameManager, NamedObject);
Spr::NamedObjectIf* Spr::NameManagerIf::FindObject(UTString name, UTString cls){
	return	((NameManager*)(Object*)(ObjectIf*)this)->FindObject(name, cls);
}
SPR_IFIMP1(Scene, NameManager);
SPR_IFIMP1(Sdk, NameManager);
SPR_OBJECTIMP0(Object);
SPR_OBJECTIMP1(NamedObject, Object);
SPR_OBJECTIMP1(SceneObject, NamedObject);
SPR_OBJECTIMP1(ObjectStates, Object);
SPR_OBJECTIMP1(NameManager, NamedObject);
SPR_OBJECTIMP1(Scene, NameManager);
SPR_OBJECTIMP1(Sdk, NameManager);
}
