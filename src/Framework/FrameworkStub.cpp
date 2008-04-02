#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Framework\SprFWApp.h"
#include "..\..\include\Framework\SprFWAppGL.h"
#include "..\..\include\Framework\SprFWAppGLUI.h"
#include "..\..\include\Framework\SprFWAppGLUT.h"
#include "..\..\include\Framework\SprFWObject.h"
#include "..\..\include\Framework\SprFWScene.h"
#include "..\..\include\Framework\SprFWSdk.h"
#include "..\..\src\Base\BaseDebug.h"
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
#include "..\Foundation\UTTypeDesc.h"
#include "..\Framework\Framework.h"
#include "..\Framework\FWObject.h"
#include "..\Framework\FWOldSpringheadNode.h"
#include "..\Framework\FWOldSpringheadNodeHandler.h"
#include "..\Framework\FWScene.h"
#include "..\Framework\FWSdk.h"
namespace Spr{

void RegisterTypeDescFramework(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::FWObjectDesc* pFWObjectDesc = NULL;
	desc = DBG_NEW UTTypeDesc("FWObjectDesc");
	desc->size = sizeof(Spr::FWObjectDesc);
	desc->ifInfo = FWObjectIf::GetIfInfoStatic();
	((IfInfo*)FWObjectIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::FWObjectDesc>;
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
}

SPR_IFIMP1(FWObject, SceneObject);
PHSolidIf* Spr::FWObjectIf::GetPHSolid(){
	return	((FWObject*)(Object*)(ObjectIf*)this)->GetPHSolid();
}
void Spr::FWObjectIf::SetPHSolid(PHSolidIf* s){
	((FWObject*)(Object*)(ObjectIf*)this)->SetPHSolid(s);
}
GRFrameIf* Spr::FWObjectIf::GetGRFrame(){
	return	((FWObject*)(Object*)(ObjectIf*)this)->GetGRFrame();
}
void Spr::FWObjectIf::SetGRFrame(GRFrameIf* f){
	((FWObject*)(Object*)(ObjectIf*)this)->SetGRFrame(f);
}
SPR_IFIMP1(FWScene, Scene);
PHSceneIf* Spr::FWSceneIf::GetPHScene(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetPHScene();
}
void Spr::FWSceneIf::SetPHScene(PHSceneIf* s){
	((FWScene*)(Object*)(ObjectIf*)this)->SetPHScene(s);
}
GRSceneIf* Spr::FWSceneIf::GetGRScene(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetGRScene();
}
void Spr::FWSceneIf::SetGRScene(GRSceneIf* s){
	((FWScene*)(Object*)(ObjectIf*)this)->SetGRScene(s);
}
Spr::FWObjectIf* Spr::FWSceneIf::CreateObject(const PHSolidDesc& soliddesc, const GRFrameDesc& framedesc){
	return	((FWScene*)(Object*)(ObjectIf*)this)->CreateObject(soliddesc, framedesc);
}
int Spr::FWSceneIf::NObject()const{
	return	((FWScene*)(Object*)(ObjectIf*)this)->NObject();
}
Spr::FWObjectIf** Spr::FWSceneIf::GetObjects(){
	return	((FWScene*)(Object*)(ObjectIf*)this)->GetObjects();
}
void Spr::FWSceneIf::Sync(){
	((FWScene*)(Object*)(ObjectIf*)this)->Sync();
}
void Spr::FWSceneIf::Step(){
	((FWScene*)(Object*)(ObjectIf*)this)->Step();
}
void Spr::FWSceneIf::Draw(GRRenderIf* grRender, bool debug){
	((FWScene*)(Object*)(ObjectIf*)this)->Draw(grRender, debug);
}
void Spr::FWSceneIf::AddHumanInterface(Spr::HIForceDevice6D* d){
	((FWScene*)(Object*)(ObjectIf*)this)->AddHumanInterface(d);
}
SPR_IFIMP1(FWSdk, Sdk);
Spr::FWSceneIf* Spr::FWSdkIf::CreateScene(const PHSceneDesc& phdesc, const GRSceneDesc& grdesc){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->CreateScene(phdesc, grdesc);
}
bool Spr::FWSdkIf::LoadScene(UTString filename){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->LoadScene(filename);
}
bool Spr::FWSdkIf::SaveScene(UTString filename){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->SaveScene(filename);
}
int Spr::FWSdkIf::NScene()const{
	return	((FWSdk*)(Object*)(ObjectIf*)this)->NScene();
}
void Spr::FWSdkIf::SwitchScene(Spr::FWSceneIf* scene){
	((FWSdk*)(Object*)(ObjectIf*)this)->SwitchScene(scene);
}
Spr::FWSceneIf* Spr::FWSdkIf::GetScene(int index){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetScene(index);
}
void Spr::FWSdkIf::MergeScene(Spr::FWSceneIf* scene0, Spr::FWSceneIf* scene1){
	((FWSdk*)(Object*)(ObjectIf*)this)->MergeScene(scene0, scene1);
}
GRRenderIf* Spr::FWSdkIf::CreateRender(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->CreateRender();
}
int Spr::FWSdkIf::NRender()const{
	return	((FWSdk*)(Object*)(ObjectIf*)this)->NRender();
}
GRRenderIf* Spr::FWSdkIf::GetRender(int index){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetRender(index);
}
void Spr::FWSdkIf::SwitchRender(GRRenderIf* render){
	((FWSdk*)(Object*)(ObjectIf*)this)->SwitchRender(render);
}
PHSdkIf* Spr::FWSdkIf::GetPHSdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetPHSdk();
}
GRSdkIf* Spr::FWSdkIf::GetGRSdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetGRSdk();
}
FISdkIf* Spr::FWSdkIf::GetFISdk(){
	return	((FWSdk*)(Object*)(ObjectIf*)this)->GetFISdk();
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
SPR_OBJECTIMP1(FWObject, SceneObject);
SPR_OBJECTIMP1(FWScene, Scene);
SPR_OBJECTIMP1(FWSdk, Sdk);
}
