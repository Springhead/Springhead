#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Graphics\SprGRBlendMesh.h"
#include "..\..\include\Graphics\SprGRFrame.h"
#include "..\..\include\Graphics\SprGRMesh.h"
#include "..\..\include\Graphics\SprGRRender.h"
#include "..\..\include\Graphics\SprGRScene.h"
#include "..\..\include\Graphics\SprGRSdk.h"
#include "..\..\include\Graphics\SprGRShader.h"
#include "..\..\include\Graphics\SprGRSphere.h"
#include "..\..\include\Graphics\SprGRVertex.h"
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
#include "..\Graphics\Graphics.h"
#include "..\Graphics\GRBlendMesh.h"
#include "..\Graphics\GRDebugRender.h"
#include "..\Graphics\GRDeviceGL.h"
#include "..\Graphics\GRFrame.h"
#include "..\Graphics\GRLoadBmp.h"
#include "..\Graphics\GRMesh.h"
#include "..\Graphics\GRRender.h"
#include "..\Graphics\GRScene.h"
#include "..\Graphics\GRSdk.h"
#include "..\Graphics\GRSphere.h"
namespace Spr{

void RegisterTypeDescGraphics(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::GRBlendMeshDesc* pGRBlendMeshDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRBlendMeshDesc");
	desc->size = sizeof(Spr::GRBlendMeshDesc);
	desc->ifInfo = GRBlendMeshIf::GetIfInfoStatic();
	((IfInfo*)GRBlendMeshIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRBlendMeshDesc>;
	field = desc->AddField("vector", "Vec3f", "positions", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->positions) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "Vec3f", "normals", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->normals) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "Vec4f", "colors", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->colors) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "Vec2f", "texCoords", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->texCoords) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "size_t", "faces", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->faces) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "Vec4f", "blends", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->blends) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "Vec4f", "matrixIndices", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->matrixIndices) - (char*)pGRBlendMeshDesc);
	field = desc->AddField("vector", "Vec4f", "numMatrix", "");
	field->offset = int((char*)&(pGRBlendMeshDesc->numMatrix) - (char*)pGRBlendMeshDesc);
	db->RegisterDesc(desc);
	Spr::GRVisualDesc* pGRVisualDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRVisualDesc");
	desc->size = sizeof(Spr::GRVisualDesc);
	desc->ifInfo = GRVisualIf::GetIfInfoStatic();
	((IfInfo*)GRVisualIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRVisualDesc>;
	db->RegisterDesc(desc);
	Spr::GRFrameTransformMatrix* pGRFrameTransformMatrix = NULL;
	desc = DBG_NEW UTTypeDesc("GRFrameTransformMatrix");
	desc->size = sizeof(Spr::GRFrameTransformMatrix);
	desc->access = DBG_NEW UTAccess<Spr::GRFrameTransformMatrix>;
	field = desc->AddField("", "Affinef", "transform", "");
	field->offset = int((char*)&(pGRFrameTransformMatrix->transform) - (char*)pGRFrameTransformMatrix);
	db->RegisterDesc(desc);
	Spr::GRFrameDesc* pGRFrameDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRFrameDesc");
	desc->size = sizeof(Spr::GRFrameDesc);
	desc->ifInfo = GRFrameIf::GetIfInfoStatic();
	((IfInfo*)GRFrameIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRFrameDesc>;
	field = desc->AddBase("GRVisualDesc");
	field->offset = int((char*)(GRVisualDesc*)pGRFrameDesc - (char*)pGRFrameDesc);
	field = desc->AddField("", "Affinef", "transform", "");
	field->offset = int((char*)&(pGRFrameDesc->transform) - (char*)pGRFrameDesc);
	db->RegisterDesc(desc);
	Spr::GRDummyFrameDesc* pGRDummyFrameDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRDummyFrameDesc");
	desc->size = sizeof(Spr::GRDummyFrameDesc);
	desc->ifInfo = GRDummyFrameIf::GetIfInfoStatic();
	((IfInfo*)GRDummyFrameIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRDummyFrameDesc>;
	field = desc->AddBase("GRVisualDesc");
	field->offset = int((char*)(GRVisualDesc*)pGRDummyFrameDesc - (char*)pGRDummyFrameDesc);
	db->RegisterDesc(desc);
	Spr::GRKey* pGRKey = NULL;
	desc = DBG_NEW UTTypeDesc("GRKey");
	desc->size = sizeof(Spr::GRKey);
	desc->access = DBG_NEW UTAccess<Spr::GRKey>;
	field = desc->AddField("", "unsigned int", "time", "");
	field->offset = int((char*)&(pGRKey->time) - (char*)pGRKey);
	field = desc->AddField("vector", "float", "values", "");
	field->offset = int((char*)&(pGRKey->values) - (char*)pGRKey);
	db->RegisterDesc(desc);
	Spr::GRAnimationKey* pGRAnimationKey = NULL;
	desc = DBG_NEW UTTypeDesc("GRAnimationKey");
	desc->size = sizeof(Spr::GRAnimationKey);
	desc->access = DBG_NEW UTAccess<Spr::GRAnimationKey>;
	field = desc->AddField("", "int", "keyType", "");
	field->offset = int((char*)&(pGRAnimationKey->keyType) - (char*)pGRAnimationKey);
	field = desc->AddField("vector", "GRKey", "keys", "");
	field->offset = int((char*)&(pGRAnimationKey->keys) - (char*)pGRAnimationKey);
	db->RegisterDesc(desc);
	Spr::GRAnimationDesc* pGRAnimationDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRAnimationDesc");
	desc->size = sizeof(Spr::GRAnimationDesc);
	desc->ifInfo = GRAnimationIf::GetIfInfoStatic();
	((IfInfo*)GRAnimationIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRAnimationDesc>;
	field = desc->AddField("vector", "GRAnimationKey", "keys", "");
	field->offset = int((char*)&(pGRAnimationDesc->keys) - (char*)pGRAnimationDesc);
	db->RegisterDesc(desc);
	Spr::GRAnimationSetDesc* pGRAnimationSetDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRAnimationSetDesc");
	desc->size = sizeof(Spr::GRAnimationSetDesc);
	desc->ifInfo = GRAnimationSetIf::GetIfInfoStatic();
	((IfInfo*)GRAnimationSetIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRAnimationSetDesc>;
	db->RegisterDesc(desc);
	Spr::GRAnimationControllerDesc* pGRAnimationControllerDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRAnimationControllerDesc");
	desc->size = sizeof(Spr::GRAnimationControllerDesc);
	desc->ifInfo = GRAnimationControllerIf::GetIfInfoStatic();
	((IfInfo*)GRAnimationControllerIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRAnimationControllerDesc>;
	db->RegisterDesc(desc);
	Spr::GRMeshDesc* pGRMeshDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRMeshDesc");
	desc->size = sizeof(Spr::GRMeshDesc);
	desc->ifInfo = GRMeshIf::GetIfInfoStatic();
	((IfInfo*)GRMeshIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRMeshDesc>;
	field = desc->AddBase("GRVisualDesc");
	field->offset = int((char*)(GRVisualDesc*)pGRMeshDesc - (char*)pGRMeshDesc);
	field = desc->AddField("vector", "Vec3f", "positions", "");
	field->offset = int((char*)&(pGRMeshDesc->positions) - (char*)pGRMeshDesc);
	field = desc->AddField("vector", "Vec3f", "normals", "");
	field->offset = int((char*)&(pGRMeshDesc->normals) - (char*)pGRMeshDesc);
	field = desc->AddField("vector", "Vec4f", "colors", "");
	field->offset = int((char*)&(pGRMeshDesc->colors) - (char*)pGRMeshDesc);
	field = desc->AddField("vector", "Vec2f", "texCoords", "");
	field->offset = int((char*)&(pGRMeshDesc->texCoords) - (char*)pGRMeshDesc);
	field = desc->AddField("vector", "size_t", "faces", "");
	field->offset = int((char*)&(pGRMeshDesc->faces) - (char*)pGRMeshDesc);
	db->RegisterDesc(desc);
	Spr::GRLightDesc* pGRLightDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRLightDesc");
	desc->size = sizeof(Spr::GRLightDesc);
	desc->ifInfo = GRLightIf::GetIfInfoStatic();
	((IfInfo*)GRLightIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRLightDesc>;
	field = desc->AddBase("GRVisualDesc");
	field->offset = int((char*)(GRVisualDesc*)pGRLightDesc - (char*)pGRLightDesc);
	field = desc->AddField("", "Vec4f", "ambient", "");
	field->offset = int((char*)&(pGRLightDesc->ambient) - (char*)pGRLightDesc);
	field = desc->AddField("", "Vec4f", "diffuse", "");
	field->offset = int((char*)&(pGRLightDesc->diffuse) - (char*)pGRLightDesc);
	field = desc->AddField("", "Vec4f", "specular", "");
	field->offset = int((char*)&(pGRLightDesc->specular) - (char*)pGRLightDesc);
	field = desc->AddField("", "Vec4f", "position", "");
	field->offset = int((char*)&(pGRLightDesc->position) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "range", "");
	field->offset = int((char*)&(pGRLightDesc->range) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "attenuation0", "");
	field->offset = int((char*)&(pGRLightDesc->attenuation0) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "attenuation1", "");
	field->offset = int((char*)&(pGRLightDesc->attenuation1) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "attenuation2", "");
	field->offset = int((char*)&(pGRLightDesc->attenuation2) - (char*)pGRLightDesc);
	field = desc->AddField("", "Vec3f", "spotDirection", "");
	field->offset = int((char*)&(pGRLightDesc->spotDirection) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "spotFalloff", "");
	field->offset = int((char*)&(pGRLightDesc->spotFalloff) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "spotInner", "");
	field->offset = int((char*)&(pGRLightDesc->spotInner) - (char*)pGRLightDesc);
	field = desc->AddField("", "float", "spotCutoff", "");
	field->offset = int((char*)&(pGRLightDesc->spotCutoff) - (char*)pGRLightDesc);
	db->RegisterDesc(desc);
	Spr::GRMaterialDesc* pGRMaterialDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRMaterialDesc");
	desc->size = sizeof(Spr::GRMaterialDesc);
	desc->ifInfo = GRMaterialIf::GetIfInfoStatic();
	((IfInfo*)GRMaterialIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRMaterialDesc>;
	field = desc->AddBase("GRVisualDesc");
	field->offset = int((char*)(GRVisualDesc*)pGRMaterialDesc - (char*)pGRMaterialDesc);
	field = desc->AddField("", "Vec4f", "ambient", "");
	field->offset = int((char*)&(pGRMaterialDesc->ambient) - (char*)pGRMaterialDesc);
	field = desc->AddField("", "Vec4f", "diffuse", "");
	field->offset = int((char*)&(pGRMaterialDesc->diffuse) - (char*)pGRMaterialDesc);
	field = desc->AddField("", "Vec4f", "specular", "");
	field->offset = int((char*)&(pGRMaterialDesc->specular) - (char*)pGRMaterialDesc);
	field = desc->AddField("", "Vec4f", "emissive", "");
	field->offset = int((char*)&(pGRMaterialDesc->emissive) - (char*)pGRMaterialDesc);
	field = desc->AddField("", "float", "power", "");
	field->offset = int((char*)&(pGRMaterialDesc->power) - (char*)pGRMaterialDesc);
	field = desc->AddField("", "string", "texname", "");
	field->offset = int((char*)&(pGRMaterialDesc->texname) - (char*)pGRMaterialDesc);
	db->RegisterDesc(desc);
	Spr::GRCameraDesc* pGRCameraDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRCameraDesc");
	desc->size = sizeof(Spr::GRCameraDesc);
	desc->ifInfo = GRCameraIf::GetIfInfoStatic();
	((IfInfo*)GRCameraIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRCameraDesc>;
	field = desc->AddBase("GRVisualDesc");
	field->offset = int((char*)(GRVisualDesc*)pGRCameraDesc - (char*)pGRCameraDesc);
	field = desc->AddField("", "Vec2f", "size", "");
	field->offset = int((char*)&(pGRCameraDesc->size) - (char*)pGRCameraDesc);
	field = desc->AddField("", "Vec2f", "center", "");
	field->offset = int((char*)&(pGRCameraDesc->center) - (char*)pGRCameraDesc);
	field = desc->AddField("", "float", "front", "");
	field->offset = int((char*)&(pGRCameraDesc->front) - (char*)pGRCameraDesc);
	field = desc->AddField("", "float", "back", "");
	field->offset = int((char*)&(pGRCameraDesc->back) - (char*)pGRCameraDesc);
	db->RegisterDesc(desc);
	Spr::GRSceneDesc* pGRSceneDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRSceneDesc");
	desc->size = sizeof(Spr::GRSceneDesc);
	desc->ifInfo = GRSceneIf::GetIfInfoStatic();
	((IfInfo*)GRSceneIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRSceneDesc>;
	db->RegisterDesc(desc);
	Spr::GRSdkDesc* pGRSdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRSdkDesc");
	desc->size = sizeof(Spr::GRSdkDesc);
	desc->ifInfo = GRSdkIf::GetIfInfoStatic();
	((IfInfo*)GRSdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRSdkDesc>;
	db->RegisterDesc(desc);
	Spr::SFBlendLocation* pSFBlendLocation = NULL;
	desc = DBG_NEW UTTypeDesc("SFBlendLocation");
	desc->size = sizeof(Spr::SFBlendLocation);
	desc->access = DBG_NEW UTAccess<Spr::SFBlendLocation>;
	field = desc->AddField("", "unsigned int", "uBlendMatrix", "");
	field->offset = int((char*)&(pSFBlendLocation->uBlendMatrix) - (char*)pSFBlendLocation);
	field = desc->AddField("", "unsigned int", "aWeight", "");
	field->offset = int((char*)&(pSFBlendLocation->aWeight) - (char*)pSFBlendLocation);
	field = desc->AddField("", "unsigned int", "aMatrixIndices", "");
	field->offset = int((char*)&(pSFBlendLocation->aMatrixIndices) - (char*)pSFBlendLocation);
	field = desc->AddField("", "unsigned int", "aNumMatrix", "");
	field->offset = int((char*)&(pSFBlendLocation->aNumMatrix) - (char*)pSFBlendLocation);
	db->RegisterDesc(desc);
	Spr::GRShaderFormat* pGRShaderFormat = NULL;
	desc = DBG_NEW UTTypeDesc("GRShaderFormat");
	desc->size = sizeof(Spr::GRShaderFormat);
	desc->access = DBG_NEW UTAccess<Spr::GRShaderFormat>;
	db->RegisterDesc(desc);
	Spr::GRSphereDesc* pGRSphereDesc = NULL;
	desc = DBG_NEW UTTypeDesc("GRSphereDesc");
	desc->size = sizeof(Spr::GRSphereDesc);
	desc->ifInfo = GRSphereIf::GetIfInfoStatic();
	((IfInfo*)GRSphereIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::GRSphereDesc>;
	field = desc->AddField("", "float", "radius", "");
	field->offset = int((char*)&(pGRSphereDesc->radius) - (char*)pGRSphereDesc);
	field = desc->AddField("", "int", "slices", "");
	field->offset = int((char*)&(pGRSphereDesc->slices) - (char*)pGRSphereDesc);
	field = desc->AddField("", "int", "stacks", "");
	field->offset = int((char*)&(pGRSphereDesc->stacks) - (char*)pGRSphereDesc);
	db->RegisterDesc(desc);
	Spr::GRVertexElement* pGRVertexElement = NULL;
	desc = DBG_NEW UTTypeDesc("GRVertexElement");
	desc->size = sizeof(Spr::GRVertexElement);
	desc->access = DBG_NEW UTAccess<Spr::GRVertexElement>;
	field = desc->AddField("", "short", "stream", "");
	field->offset = int((char*)&(pGRVertexElement->stream) - (char*)pGRVertexElement);
	field = desc->AddField("", "short", "offset", "");
	field->offset = int((char*)&(pGRVertexElement->offset) - (char*)pGRVertexElement);
	field = desc->AddField("", "unsigned char", "type", "");
	field->offset = int((char*)&(pGRVertexElement->type) - (char*)pGRVertexElement);
	field = desc->AddField("", "unsigned char", "method", "");
	field->offset = int((char*)&(pGRVertexElement->method) - (char*)pGRVertexElement);
	field = desc->AddField("", "unsigned char", "usage", "");
	field->offset = int((char*)&(pGRVertexElement->usage) - (char*)pGRVertexElement);
	field = desc->AddField("", "unsigned char", "usageIndex", "");
	field->offset = int((char*)&(pGRVertexElement->usageIndex) - (char*)pGRVertexElement);
	db->RegisterDesc(desc);
	Spr::GRVertexArray* pGRVertexArray = NULL;
	desc = DBG_NEW UTTypeDesc("GRVertexArray");
	desc->size = sizeof(Spr::GRVertexArray);
	desc->access = DBG_NEW UTAccess<Spr::GRVertexArray>;
	field = desc->AddField("", "GRVertexElement", "format", "");
	field->offset = int((char*)&(pGRVertexArray->format) - (char*)pGRVertexArray);
	field = desc->AddField("pointer", "void", "buffer", "");
	field->offset = int((char*)&(pGRVertexArray->buffer) - (char*)pGRVertexArray);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(GRBlendMesh, GRVisual);
SPR_IFIMP1(GRVisual, NamedObject);
void Spr::GRVisualIf::Render(Spr::GRRenderIf* r){
	((GRVisual*)(Object*)(ObjectIf*)this)->Render(r);
}
void Spr::GRVisualIf::Rendered(Spr::GRRenderIf* r){
	((GRVisual*)(Object*)(ObjectIf*)this)->Rendered(r);
}
SPR_IFIMP1(GRFrame, GRVisual);
Spr::GRFrameIf* Spr::GRFrameIf::GetParent(){
	return	((GRFrame*)(Object*)(ObjectIf*)this)->GetParent();
}
void Spr::GRFrameIf::SetParent(Spr::GRFrameIf* fr){
	((GRFrame*)(Object*)(ObjectIf*)this)->SetParent(fr);
}
int Spr::GRFrameIf::NChildren(){
	return	((GRFrame*)(Object*)(ObjectIf*)this)->NChildren();
}
Spr::GRVisualIf** Spr::GRFrameIf::GetChildren(){
	return	((GRFrame*)(Object*)(ObjectIf*)this)->GetChildren();
}
Affinef Spr::GRFrameIf::GetTransform(){
	return	((GRFrame*)(Object*)(ObjectIf*)this)->GetTransform();
}
Affinef Spr::GRFrameIf::GetWorldTransform(){
	return	((GRFrame*)(Object*)(ObjectIf*)this)->GetWorldTransform();
}
void Spr::GRFrameIf::SetTransform(const Affinef& af){
	((GRFrame*)(Object*)(ObjectIf*)this)->SetTransform(af);
}
void Spr::GRFrameIf::Print(std::ostream& os)const{
	((GRFrame*)(Object*)(ObjectIf*)this)->Print(os);
}
SPR_IFIMP1(GRDummyFrame, GRVisual);
SPR_IFIMP1(GRAnimation, SceneObject);
void Spr::GRAnimationIf::BlendPose(float time, float weight){
	((GRAnimation*)(Object*)(ObjectIf*)this)->BlendPose(time, weight);
}
void Spr::GRAnimationIf::ResetPose(){
	((GRAnimation*)(Object*)(ObjectIf*)this)->ResetPose();
}
void Spr::GRAnimationIf::LoadInitialPose(){
	((GRAnimation*)(Object*)(ObjectIf*)this)->LoadInitialPose();
}
SPR_IFIMP1(GRAnimationSet, SceneObject);
void Spr::GRAnimationSetIf::BlendPose(float time, float weight){
	((GRAnimationSet*)(Object*)(ObjectIf*)this)->BlendPose(time, weight);
}
void Spr::GRAnimationSetIf::ResetPose(){
	((GRAnimationSet*)(Object*)(ObjectIf*)this)->ResetPose();
}
void Spr::GRAnimationSetIf::LoadInitialPose(){
	((GRAnimationSet*)(Object*)(ObjectIf*)this)->LoadInitialPose();
}
SPR_IFIMP1(GRAnimationController, SceneObject);
void Spr::GRAnimationControllerIf::BlendPose(UTString name, float time, float weight){
	((GRAnimationController*)(Object*)(ObjectIf*)this)->BlendPose(name, time, weight);
}
void Spr::GRAnimationControllerIf::ResetPose(){
	((GRAnimationController*)(Object*)(ObjectIf*)this)->ResetPose();
}
void Spr::GRAnimationControllerIf::LoadInitialPose(){
	((GRAnimationController*)(Object*)(ObjectIf*)this)->LoadInitialPose();
}
SPR_IFIMP1(GRMesh, GRVisual);
SPR_IFIMP1(GRLight, GRVisual);
SPR_IFIMP1(GRMaterial, GRVisual);
bool Spr::GRMaterialIf::IsOpaque()const{
	return	((GRMaterial*)(Object*)(ObjectIf*)this)->IsOpaque();
}
SPR_IFIMP1(GRCamera, GRVisual);
Spr::GRFrameIf* Spr::GRCameraIf::GetFrame(){
	return	((GRCamera*)(Object*)(ObjectIf*)this)->GetFrame();
}
void Spr::GRCameraIf::SetFrame(Spr::GRFrameIf* fr){
	((GRCamera*)(Object*)(ObjectIf*)this)->SetFrame(fr);
}
SPR_IFIMP1(GRRenderBase, Object);
void Spr::GRRenderBaseIf::SetViewport(Vec2f pos, Vec2f sz){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetViewport(pos, sz);
}
void Spr::GRRenderBaseIf::ClearBuffer(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->ClearBuffer();
}
void Spr::GRRenderBaseIf::BeginScene(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->BeginScene();
}
void Spr::GRRenderBaseIf::EndScene(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->EndScene();
}
void Spr::GRRenderBaseIf::SetViewMatrix(const Affinef& afv){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetViewMatrix(afv);
}
void Spr::GRRenderBaseIf::SetProjectionMatrix(const Affinef& afp){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetProjectionMatrix(afp);
}
void Spr::GRRenderBaseIf::GetProjectionMatrix(const Affinef& afp){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->GetProjectionMatrix(afp);
}
void Spr::GRRenderBaseIf::SetModelMatrix(const Affinef& afw){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetModelMatrix(afw);
}
void Spr::GRRenderBaseIf::MultModelMatrix(const Affinef& afw){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->MultModelMatrix(afw);
}
void Spr::GRRenderBaseIf::PushModelMatrix(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->PushModelMatrix();
}
void Spr::GRRenderBaseIf::PopModelMatrix(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->PopModelMatrix();
}
void Spr::GRRenderBaseIf::ClearBlendMatrix(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->ClearBlendMatrix();
}
bool Spr::GRRenderBaseIf::SetBlendMatrix(const Affinef& afb, unsigned int id){
	return	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetBlendMatrix(afb, id);
}
void Spr::GRRenderBaseIf::SetVertexFormat(const GRVertexElement* e){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetVertexFormat(e);
}
void Spr::GRRenderBaseIf::SetVertexShader(void* shader){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetVertexShader(shader);
}
void Spr::GRRenderBaseIf::DrawDirect(Spr::GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawDirect(ty, vtx, count, stride);
}
void Spr::GRRenderBaseIf::DrawIndexed(Spr::GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawIndexed(ty, idx, vtx, count, stride);
}
void Spr::GRRenderBaseIf::DrawArrays(Spr::GRRenderBaseIf::TPrimitiveType ty, GRVertexArray* arrays, size_t count){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawArrays(ty, arrays, count);
}
void Spr::GRRenderBaseIf::DrawArrays(Spr::GRRenderBaseIf::TPrimitiveType ty, size_t* idx, GRVertexArray* arrays, size_t count){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawArrays(ty, idx, arrays, count);
}
void Spr::GRRenderBaseIf::DrawSphere(float radius, int slices, int stacks, bool solid){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawSphere(radius, slices, stacks, solid);
}
void Spr::GRRenderBaseIf::DrawCone(float radius, float height, int slice, bool solid){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawCone(radius, height, slice, solid);
}
void Spr::GRRenderBaseIf::DrawCylinder(float radius, float height, int slice, bool solid){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawCylinder(radius, height, slice, solid);
}
int Spr::GRRenderBaseIf::StartList(){
	return	((GRRenderBase*)(Object*)(ObjectIf*)this)->StartList();
}
void Spr::GRRenderBaseIf::EndList(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->EndList();
}
void Spr::GRRenderBaseIf::DrawList(int i){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawList(i);
}
void Spr::GRRenderBaseIf::ReleaseList(int i){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->ReleaseList(i);
}
void Spr::GRRenderBaseIf::DrawFont(Vec2f pos, const std::string str, const Spr::GRFont& font){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawFont(pos, str, font);
}
void Spr::GRRenderBaseIf::DrawFont(Vec3f pos, const std::string str, const Spr::GRFont& font){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->DrawFont(pos, str, font);
}
void Spr::GRRenderBaseIf::SetMaterial(const Spr::GRMaterialDesc& mat){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetMaterial(mat);
}
void Spr::GRRenderBaseIf::SetMaterial(const Spr::GRMaterialIf* mat){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetMaterial(mat);
}
void Spr::GRRenderBaseIf::SetLineWidth(float w){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetLineWidth(w);
}
void Spr::GRRenderBaseIf::PushLight(const Spr::GRLightDesc& light){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->PushLight(light);
}
void Spr::GRRenderBaseIf::PushLight(const Spr::GRLightIf* light){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->PushLight(light);
}
void Spr::GRRenderBaseIf::PopLight(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->PopLight();
}
void Spr::GRRenderBaseIf::SetDepthWrite(bool b){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetDepthWrite(b);
}
void Spr::GRRenderBaseIf::SetDepthTest(bool b){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetDepthTest(b);
}
void Spr::GRRenderBaseIf::SetDepthFunc(Spr::GRRenderBaseIf::TDepthFunc f){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetDepthFunc(f);
}
void Spr::GRRenderBaseIf::SetAlphaTest(bool b){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetAlphaTest(b);
}
void Spr::GRRenderBaseIf::SetAlphaMode(Spr::GRRenderBaseIf::TBlendFunc src, Spr::GRRenderBaseIf::TBlendFunc dest){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetAlphaMode(src, dest);
}
void Spr::GRRenderBaseIf::SetLighting(bool l){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetLighting(l);
}
unsigned int Spr::GRRenderBaseIf::LoadTexture(const std::string filename){
	return	((GRRenderBase*)(Object*)(ObjectIf*)this)->LoadTexture(filename);
}
void Spr::GRRenderBaseIf::InitShader(){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->InitShader();
}
void Spr::GRRenderBaseIf::SetShaderFormat(GRShaderFormat::ShaderType type){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->SetShaderFormat(type);
}
bool Spr::GRRenderBaseIf::CreateShader(std::string vShaderFile, std::string fShaderFile, Spr::GRHandler& shader){
	return	((GRRenderBase*)(Object*)(ObjectIf*)this)->CreateShader(vShaderFile, fShaderFile, shader);
}
Spr::GRHandler Spr::GRRenderBaseIf::CreateShader(){
	return	((GRRenderBase*)(Object*)(ObjectIf*)this)->CreateShader();
}
bool Spr::GRRenderBaseIf::ReadShaderSource(Spr::GRHandler shader, std::string file){
	return	((GRRenderBase*)(Object*)(ObjectIf*)this)->ReadShaderSource(shader, file);
}
void Spr::GRRenderBaseIf::GetShaderLocation(Spr::GRHandler shader, void* location){
	((GRRenderBase*)(Object*)(ObjectIf*)this)->GetShaderLocation(shader, location);
}
SPR_IFIMP1(GRRender, GRRenderBase);
void Spr::GRRenderIf::SetDevice(Spr::GRDeviceIf* dev){
	((GRRender*)(Object*)(ObjectIf*)this)->SetDevice(dev);
}
void Spr::GRRenderIf::SetCamera(const Spr::GRCameraDesc& cam){
	((GRRender*)(Object*)(ObjectIf*)this)->SetCamera(cam);
}
const Spr::GRCameraDesc& Spr::GRRenderIf::GetCamera(){
	return	((GRRender*)(Object*)(ObjectIf*)this)->GetCamera();
}
void Spr::GRRenderIf::Reshape(Vec2f pos, Vec2f screenSize){
	((GRRender*)(Object*)(ObjectIf*)this)->Reshape(pos, screenSize);
}
SPR_IFIMP1(GRDevice, GRRenderBase);
void Spr::GRDeviceIf::Init(){
	((GRDevice*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::GRDeviceIf::Print(std::ostream& os)const{
	((GRDevice*)(Object*)(ObjectIf*)this)->Print(os);
}
SPR_IFIMP1(GRDeviceGL, GRDevice);
SPR_IFIMP1(GRDebugRender, GRRender);
void Spr::GRDebugRenderIf::DrawScene(PHSceneIf* scene){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->DrawScene(scene);
}
void Spr::GRDebugRenderIf::DrawWorldAxis(PHSceneIf* scene){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->DrawWorldAxis(scene);
}
void Spr::GRDebugRenderIf::DrawSolid(PHSolidIf* solid){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->DrawSolid(solid);
}
void Spr::GRDebugRenderIf::SetMaterialSample(Spr::GRDebugRenderIf::TMaterialSample matname){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->SetMaterialSample(matname);
}
void Spr::GRDebugRenderIf::SetRenderMode(bool solid, bool wire){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->SetRenderMode(solid, wire);
}
void Spr::GRDebugRenderIf::EnableRenderAxis(bool enable, float scale){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->EnableRenderAxis(enable, scale);
}
void Spr::GRDebugRenderIf::EnableRenderForce(bool enable, float scale){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->EnableRenderForce(enable, scale);
}
void Spr::GRDebugRenderIf::EnableRenderContact(bool enable){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->EnableRenderContact(enable);
}
void Spr::GRDebugRenderIf::DrawFaceSolid(CDFaceIf* face, Vec3f* base){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->DrawFaceSolid(face, base);
}
void Spr::GRDebugRenderIf::DrawFaceWire(CDFaceIf* face, Vec3f* base){
	((GRDebugRender*)(Object*)(ObjectIf*)this)->DrawFaceWire(face, base);
}
SPR_IFIMP1(GRScene, Scene);
void Spr::GRSceneIf::Render(Spr::GRRenderIf* r){
	((GRScene*)(Object*)(ObjectIf*)this)->Render(r);
}
Spr::GRFrameIf* Spr::GRSceneIf::GetWorld(){
	return	((GRScene*)(Object*)(ObjectIf*)this)->GetWorld();
}
Spr::GRCameraIf* Spr::GRSceneIf::GetCamera(){
	return	((GRScene*)(Object*)(ObjectIf*)this)->GetCamera();
}
Spr::GRAnimationControllerIf* Spr::GRSceneIf::GetAnimationController(){
	return	((GRScene*)(Object*)(ObjectIf*)this)->GetAnimationController();
}
void Spr::GRSceneIf::SetCamera(const Spr::GRCameraDesc& desc){
	((GRScene*)(Object*)(ObjectIf*)this)->SetCamera(desc);
}
Spr::GRVisualIf* Spr::GRSceneIf::CreateVisual(const IfInfo* info, const Spr::GRVisualDesc& desc, Spr::GRFrameIf* parent){
	return	((GRScene*)(Object*)(ObjectIf*)this)->CreateVisual(info, desc, parent);
}
Spr::GRSdkIf* Spr::GRSceneIf::GetSdk(){
	return	((GRScene*)(Object*)(ObjectIf*)this)->GetSdk();
}
SPR_IFIMP1(GRSdk, Sdk);
Spr::GRDebugRenderIf* Spr::GRSdkIf::CreateDebugRender(){
	return	((GRSdk*)(Object*)(ObjectIf*)this)->CreateDebugRender();
}
Spr::GRDeviceGLIf* Spr::GRSdkIf::CreateDeviceGL(){
	return	((GRSdk*)(Object*)(ObjectIf*)this)->CreateDeviceGL();
}
Spr::GRSceneIf* Spr::GRSdkIf::CreateScene(const Spr::GRSceneDesc& desc){
	return	((GRSdk*)(Object*)(ObjectIf*)this)->CreateScene(desc);
}
Spr::GRSceneIf* Spr::GRSdkIf::GetScene(size_t i){
	return	((GRSdk*)(Object*)(ObjectIf*)this)->GetScene(i);
}
size_t Spr::GRSdkIf::NScene(){
	return	((GRSdk*)(Object*)(ObjectIf*)this)->NScene();
}
void Spr::GRSdkIf::MergeScene(Spr::GRSceneIf* scene0, Spr::GRSceneIf* scene1){
	((GRSdk*)(Object*)(ObjectIf*)this)->MergeScene(scene0, scene1);
}
SPR_IFIMP1(GRSphere, GRVisual);
SPR_OBJECTIMP1(GRBlendMesh, GRVisual);
SPR_OBJECTIMP1(GRDebugRender, GRRender);
SPR_OBJECTIMP1(GRDeviceGL, GRDevice);
SPR_OBJECTIMP_ABST1(GRVisual, SceneObject);
SPR_OBJECTIMP1(GRFrame, GRVisual);
SPR_OBJECTIMP1(GRDummyFrame, GRVisual);
SPR_OBJECTIMP1(GRAnimation, SceneObject);
SPR_OBJECTIMP1(GRAnimationSet, SceneObject);
SPR_OBJECTIMP1(GRAnimationController, SceneObject);
SPR_OBJECTIMP1(GRMesh, GRVisual);
SPR_OBJECTIMP1(GRCamera, GRVisual);
SPR_OBJECTIMP1(GRLight, GRVisual);
SPR_OBJECTIMP1(GRMaterial, GRVisual);
SPR_OBJECTIMP_ABST1(GRRenderBase, Object);
SPR_OBJECTIMP1(GRRender, GRRenderBase);
SPR_OBJECTIMP_ABST1(GRDevice, GRRenderBase);
SPR_OBJECTIMP1(GRScene, Scene);
SPR_OBJECTIMP1(GRSdk, Sdk);
SPR_OBJECTIMP1(GRSphere, GRVisual);
}
